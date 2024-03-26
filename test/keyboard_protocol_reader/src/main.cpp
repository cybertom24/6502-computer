#include <Arduino.h>

/**
 * Testing the PS/2 protocol
 *
 * Documentation:
 * - https://www.burtonsys.com/ps2_chapweske.htm
 * - https://www-ug.eecg.toronto.edu/msl/nios_devices/datasheets/PS2%20Keyboard%20Protocol.htm
 *
 * Connector - Name - Arduino
 *  1 red      DATA   2 (out), 3 (in)
 *     2        NC      x
 *  3 grey     GND     GND
 *  4 brown    Vcc     +5V
 *  5 yellow   CLK    4 (out), 5 (in)
 *     6        NC      x
 *
 * Electrical interface:
 * DATA and CLK lines are open-collector so pull-up resistors are needed to exchange information
 *
 * Command composition:
 * s0 | d0 | d1 | d2 | d3 | d4 | d5 | d6 | d7 | po | s1 | a
 * s0: start bit -> always 0
 * dx: 8 data bits. Least significant bit first
 * po: parity bit -> odd parity
 * s1: stop bit -> always 1
 * a : acknowledgement -> only device-to-host
 *
 * Timing:
 * Data is read on the two different edge of the CLK signal:
 * device-to-host: falling edge (1->0)
 * host-to-device: rising  edge (0->1)
 * CLK freq must be in the range 10-16.7 kHz -> half period between 30 to 50 us
 * Modify and sample the data between 15 to 20 us after the appropriate clock transition
 *
 * CLK:
 * The CLK signal is managed by the device. Host can only inhibit the transmission by pulling
 * the line down. While the line is held "up" (the outputs are oc so no-one is controlling the
 * line) the device can initiate the transmission by pulling down the data line and the clock.
 *
 * Transmission inhibited:
 * The host can block any transmission from occurring and stop any that is currently going by pulling
 * the CLK line down for at least 100us. Any packet that was being sent is retransmitted once the CLK
 * line is left up.
 *
 * device-to-host:
 * While the CLK line is held up the device can initiate the transmission of one 11-bit packet by
 * pulling the CLK line down and sending the first bit (which is always a 0). The data transition after
 * a falling edge of the CLK must be between a 5 to 25 us window.
 *
 * host-to-device:
 * Since its the device' job to provide the CLK signal the host must make a request-to-send.
 * Firstly it has to inhibit the tranmission by pulling down the CLK line for at least 100us.
 * Then it has to pull down the DATA line and release the CLK line.
 * The device will then start the CLK signal pulling it low. The host must modify the data after
 * the falling edge of the CLK. After the parity bit has been sent, the host must release the DATA line
 * and wait for the device to pull it down together with the CLK signaling that the command has been
 * acknowledged.
 *
 * -- Protocol --
 * Each key has its own make and break code. The make code is sent when the key is pressed, whilst
 * the break code is sent when it is released.
 * The code represent the key and not the letter so it's the job of the host to link the code to the letter.
 * There are 3 sets of codes, the standard one it's the number 2.
 *
 */

/**
 * Arduino will take the part of the HOST
 */

#include "KeyboardCommands.h"

#define PIN_CLK_IN 2
#define PIN_DATA_IN 3
#define PIN_DATA_OUT 4
#define PIN_CLK_OUT 5

#define INHIBIT_DELAY 200
#define BUFFER_SIZE 32
#define PACKET_SIZE 11

struct Buffer
{
    uint8_t block[BUFFER_SIZE] = {0};
    int readPtr = 0;
    int writePtr = 0;
} buffer;

volatile struct BitsBuffer
{
    bool bits[BUFFER_SIZE];
    int writePtr;
    int readPtr;
} bitsBuffer;

uint64_t exMillis = 0;
volatile int count = 0;

uint8_t readPacket();
void waitRelease(int pin);
void waitPullDown(int pin);
void pullDown(int pin);
void release(int pin);
bool sendPacket(uint8_t data);
bool parity(uint8_t data);
char convert(uint8_t code);
void readBit();
uint8_t read();
void discard();
int available();
uint8_t peek();
void trash();
bool write(uint8_t data);
uint8_t crunchPacket();
void print(uint8_t data);

void setup()
{
    // Setup the pins on the ardiuno
    pinMode(PIN_DATA_IN, INPUT);
    pinMode(PIN_CLK_IN, INPUT);
    pinMode(PIN_DATA_OUT, OUTPUT);
    pinMode(PIN_CLK_OUT, OUTPUT);

    Serial.begin(115200);

    // Enable interrupts and attach the function
    attachInterrupt(digitalPinToInterrupt(PIN_CLK_IN), readBit, FALLING);
}

void loop()
{
    // A new packet is ready to be crunched
    if ((bitsBuffer.readPtr > bitsBuffer.writePtr && bitsBuffer.writePtr + BUFFER_SIZE - bitsBuffer.readPtr >= PACKET_SIZE) ||
        (bitsBuffer.writePtr - bitsBuffer.readPtr >= PACKET_SIZE))
    {
        uint8_t data = crunchPacket();
        print(data);
        write(data);
    }

    if (available())
    {
        if (peek() == KBRD_BREAK)
        {
            if (available() >= 2)
            {
                discard();
                Serial.print("\t\t\t[^] Released key: ");
                Serial.println(convert(read()));
            }
        }
        else
        {
            char c = convert(read());
            if (c != 0)
            {
                Serial.print("\t\t\t[v] Pressed key: ");
                Serial.println(c);
            }
        }
    }

    if (Serial.available())
    {
        char c = Serial.read();
        bool ok = false;
        trash();
        switch (c)
        {
        case 'r':
            ok = sendPacket(KBRD_RESET);
            Serial.println(ok, DEC);
            break;
        case 'e':
            ok = sendPacket(KBRD_ECHO);
            Serial.println(ok, DEC);
            break;
        case 's':
            ok = sendPacket(KBRD_RESEND);
            Serial.println(ok, DEC);
            break;
        case 'i':
            ok = sendPacket(KBRD_ID);
            Serial.println(ok, DEC);
            break;
        case 't':
            ok = sendPacket(0b10000001);
            Serial.println(ok, DEC);
            break;
        case '0':
            ok = sendPacket(KBRD_SET_SCAN_CODE);
            Serial.println(ok, DEC);
            break;
        case 'd':
            ok = sendPacket(KBRD_SET_DEFAULT);
            Serial.println(ok, DEC);
            break;
        case 'l':
            ok = sendPacket(KBRD_LED);
            Serial.println(ok, DEC);
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            ok = sendPacket((uint8_t) c - '0');
            Serial.println(ok, DEC);
            break;
        default:
            break;
        }
    }
}

// Starts reading the packet received and returns the byte read
uint8_t readPacket()
{
    // Host reads on the falling edge of the clock

    // bit 0: start bit (always 0)
    bool startBit = digitalRead(PIN_DATA_IN);
    waitRelease(PIN_CLK_IN);

    uint8_t data = 0;
    // bit 1..8: d0...d7
    for (int i = 0; i < 8; i++)
    {
        waitPullDown(PIN_CLK_IN);
        // The falling edge has arrived

        if (digitalRead(PIN_DATA_IN))
            data = data | (1 << i);

        waitRelease(PIN_CLK_IN);
    }

    // bit 9: parity bit
    waitPullDown(PIN_CLK_IN);
    bool parityBit = digitalRead(PIN_DATA_IN);
    waitRelease(PIN_CLK_IN);

    // bit 10: stop bit
    waitPullDown(PIN_CLK_IN);
    bool stopBit = digitalRead(PIN_DATA_IN);
    waitRelease(PIN_CLK_IN);

    // Check odd parity
    bool p = parity(data);
    if (p != parityBit)
        ; // Parity ok
    else
        data = -1; // Parity not ok

    return data;
}

uint8_t crunchPacket()
{
    // Host reads on the falling edge of the clock

    // bit 0: start bit (always 0)
    bool startBit = bitsBuffer.bits[bitsBuffer.readPtr];
    bitsBuffer.readPtr = (bitsBuffer.readPtr + 1) % BUFFER_SIZE;

    uint8_t data = 0;
    // bit 1..8: d0...d7
    for (int i = 0; i < 8; i++)
    {
        if (bitsBuffer.bits[bitsBuffer.readPtr])
            data = data | (1 << i);
        bitsBuffer.readPtr = (bitsBuffer.readPtr + 1) % BUFFER_SIZE;
    }

    // bit 9: parity bit
    bool parityBit = bitsBuffer.bits[bitsBuffer.readPtr];
    bitsBuffer.readPtr = (bitsBuffer.readPtr + 1) % BUFFER_SIZE;

    // bit 10: stop bit
    bool stopBit = bitsBuffer.bits[bitsBuffer.readPtr];
    bitsBuffer.readPtr = (bitsBuffer.readPtr + 1) % BUFFER_SIZE;

    // Check odd parity
    bool par = parity(data);
    if (par != parityBit)
        ; // Parity ok
    else
        data = -1; // Parity not ok

    return data;
}

void waitRelease(int pin)
{
    while (!digitalRead(pin))
        ;
}

void waitPullDown(int pin)
{
    while (digitalRead(pin))
        ;
}

void pullDown(int pin)
{
    digitalWrite(pin, HIGH);
}

void release(int pin)
{
    digitalWrite(pin, LOW);
}

bool sendPacket(uint8_t data)
{
    // Disable interrupt
    detachInterrupt(digitalPinToInterrupt(PIN_CLK_IN));

    // Calculate parity bit
    bool parityBit = !parity(data);

    // Inhibit transmissions
    pullDown(PIN_CLK_OUT);
    delayMicroseconds(INHIBIT_DELAY);

    // Request to send
    pullDown(PIN_DATA_OUT);
    delayMicroseconds(10);
    release(PIN_CLK_OUT);

    // On the next falling edge send the data
    for (int i = 0; i < 8; i++)
    {
        waitPullDown(PIN_CLK_IN);

        if (data & (1 << i))
            release(PIN_DATA_OUT);
        else
            pullDown(PIN_DATA_OUT);

        waitRelease(PIN_CLK_IN);
    }

    // Send the parity bit
    waitPullDown(PIN_CLK_IN);
    if (parityBit)
        release(PIN_DATA_OUT);
    else
        pullDown(PIN_DATA_OUT);
    waitRelease(PIN_CLK_IN);

    // Send stop bit
    waitPullDown(PIN_CLK_IN);
    release(PIN_DATA_OUT);
    waitRelease(PIN_CLK_IN);

    // Receive ack (must be 0)
    waitPullDown(PIN_CLK_IN);
    bool ack = digitalRead(PIN_DATA_IN);
    waitRelease(PIN_CLK_IN);

    // Enable interrupt
    attachInterrupt(digitalPinToInterrupt(PIN_CLK_IN), readBit, FALLING);

    return !ack;
}

/**
 * Returns:
 * false - if there is a even number of '1' inside 'data'
 * true  - if there is a odd number of '1' inside 'data'
 */
bool parity(uint8_t data)
{
    uint8_t p = 0;

    for (int i = 0; i < 8; i++)
    {
        uint8_t bit = data && 1; // Take the last bit
        p = p ^ bit;             // Xor it with the result
        data = data >> 1;        // Shift the data
    }

    return p != 0;
}

void readBit()
{
    // Exit if CLK is not 0
    if (digitalRead(PIN_CLK_IN))
        return;

    bitsBuffer.bits[bitsBuffer.writePtr] = digitalRead(PIN_DATA_IN);
    bitsBuffer.writePtr = (bitsBuffer.writePtr + 1) % BUFFER_SIZE;
    count++;
}

char convert(uint8_t code)
{
    switch (code)
    {
    case KBRD_KEY_0:
        return '0';
    case KBRD_KEY_1:
        return '1';
    case KBRD_KEY_2:
        return '2';
    // case KBRD_KEY_3: return '3';
    // case KBRD_KEY_4: return '4';
    case KBRD_KEY_5:
        return '5';
    case KBRD_KEY_6:
        return '6';
    // case KBRD_KEY_7: return '7';
    // case KBRD_KEY_8: return '8';
    case KBRD_KEY_9:
        return '9';
    case KBRD_KEY_A:
        return 'a';
    // case KBRD_KEY_B: return 'b';
    case KBRD_KEY_C:
        return 'c';
    // case KBRD_KEY_D: return 'd';
    case KBRD_KEY_E:
        return 'e';
    case KBRD_KEY_F:
        return 'f';
    // case KBRD_KEY_G: return 'g';
    case KBRD_KEY_H:
        return 'h';
    case KBRD_KEY_I:
        return 'i';
    // case KBRD_KEY_J: return 'j';
    // case KBRD_KEY_K: return 'k';
    // case KBRD_KEY_L: return 'l';
    case KBRD_KEY_M:
        return 'm';
    // case KBRD_KEY_N: return 'n';
    // case KBRD_KEY_O: return 'o';
    // case KBRD_KEY_P: return 'p';
    case KBRD_KEY_Q:
        return 'q';
    case KBRD_KEY_R:
        return 'r';
    // case KBRD_KEY_S: return 's';
    // case KBRD_KEY_T: return 't';
    case KBRD_KEY_U:
        return 'u';
    // case KBRD_KEY_V: return 'v';
    // case KBRD_KEY_W: return 'w';
    case KBRD_KEY_X:
        return 'x';
    case KBRD_KEY_Y:
        return 'y';
    case KBRD_KEY_Z:
        return 'z';
    default:
        return 0;
    }
}

uint8_t read()
{
    // Read
    uint8_t d = buffer.block[buffer.readPtr];

    // Increase read ptr if needed
    if (buffer.readPtr != buffer.writePtr)
        buffer.readPtr = (buffer.readPtr + 1) % BUFFER_SIZE;

    return d;
}

void discard()
{
    read();
}

uint8_t peek()
{
    return buffer.block[buffer.readPtr];
}

/**
 * Returns:
 * - true = write pointer has been increased
 * - false = write pointer has not been increased
 */
bool write(uint8_t data)
{
    // Write
    buffer.block[buffer.writePtr] = data;

    // Increase pointer if necessary
    if (buffer.writePtr - buffer.readPtr == -1 || buffer.writePtr - buffer.readPtr == (BUFFER_SIZE - 1))
        return false;

    buffer.writePtr = (buffer.writePtr + 1) % BUFFER_SIZE;
    return true;
}

int available()
{
    if (buffer.writePtr >= buffer.readPtr)
        return buffer.writePtr - buffer.readPtr;

    return (buffer.writePtr + BUFFER_SIZE) - buffer.readPtr;
}

void trash()
{
    buffer.readPtr = buffer.writePtr;
}

void print(uint8_t data)
{
    unsigned long tot = millis();
    int min = tot / 60000;
    tot -= min * 60000;
    int sec = tot / 1000;
    tot -= sec * 1000;
    Serial.print("<");
    Serial.print(min, DEC);
    Serial.print(":");
    Serial.print(sec, DEC);
    Serial.print(".");
    Serial.print(tot, DEC);
    Serial.print("> ");
    Serial.println(data, HEX);
}