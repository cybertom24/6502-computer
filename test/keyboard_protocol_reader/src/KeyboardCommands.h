/**
 * Doc:
 * - https://wiki.osdev.org/PS/2_Keyboard
*/

// Commands that are sent by the host
#define KBRD_RESET          0xFF
#define KBRD_RESEND         0xFE
#define KBRD_ECHO           0xEE
#define KBRD_LED            0xED
#define KBRD_SET_SCAN_CODE  0xF0
#define KBRD_SET_DEFAULT    0xF6
#define KBRD_ID             0xF2

// Commands sent by the keyboard
#define KBRD_BAT_SUCCESS    0xAA
#define KBRD_BAT_ERROR      0xFC
#define KBRD_ACK            0xFA

#define KBRD_BREAK          0xF0

// Keys
#define KBRD_KEY_A          0x1C
#define KBRD_KEY_B          0xFF
#define KBRD_KEY_C          0x21
#define KBRD_KEY_D          0xFF
#define KBRD_KEY_E          0x24
#define KBRD_KEY_F          0x2B
#define KBRD_KEY_G          0xFF
#define KBRD_KEY_H          0x33
#define KBRD_KEY_I          0x43
#define KBRD_KEY_J          0xFF
#define KBRD_KEY_K          0xFF
#define KBRD_KEY_L          0xFF
#define KBRD_KEY_M          0x3A
#define KBRD_KEY_N          0xFF
#define KBRD_KEY_O          0xFF
#define KBRD_KEY_P          0xFF
#define KBRD_KEY_Q          0x15
#define KBRD_KEY_R          0x2D
#define KBRD_KEY_S          0xFF
#define KBRD_KEY_T          0xFF
#define KBRD_KEY_U          0x3C
#define KBRD_KEY_V          0xFF
#define KBRD_KEY_X          0x22
#define KBRD_KEY_Y          0x35
#define KBRD_KEY_Z          0x1A
#define KBRD_KEY_0          0x45
#define KBRD_KEY_1          0x16
#define KBRD_KEY_2          0xFF
#define KBRD_KEY_3          0xFF
#define KBRD_KEY_4          0xFF
#define KBRD_KEY_5          0x2E
#define KBRD_KEY_6          0x36
#define KBRD_KEY_7          0xFF
#define KBRD_KEY_8          0xFF
#define KBRD_KEY_9          0x46
#define KBRD_KEY_ENTER      0xFF
