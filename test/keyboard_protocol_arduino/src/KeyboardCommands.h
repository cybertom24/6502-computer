/**
 * Doc:
 * - https://wiki.osdev.org/PS/2_Keyboard
 * 
 * This codes are for an italian layout with windows keys
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
#define KBRD_LONG_CODE      0xE0
#define KBRD_BREAK          0xF0

// Keys
// Short code:
#define KBRD_KEY_A          0x1C
#define KBRD_KEY_B          0x32
#define KBRD_KEY_C          0x21
#define KBRD_KEY_D          0x23
#define KBRD_KEY_E          0x24
#define KBRD_KEY_F          0x2B
#define KBRD_KEY_G          0x34
#define KBRD_KEY_H          0x33
#define KBRD_KEY_I          0x43
#define KBRD_KEY_J          0x3B
#define KBRD_KEY_K          0x42
#define KBRD_KEY_L          0x4B
#define KBRD_KEY_M          0x3A
#define KBRD_KEY_N          0x31
#define KBRD_KEY_O          0x44
#define KBRD_KEY_P          0x4D
#define KBRD_KEY_Q          0x15
#define KBRD_KEY_R          0x2D
#define KBRD_KEY_S          0x1B
#define KBRD_KEY_T          0x2C
#define KBRD_KEY_U          0x3C
#define KBRD_KEY_V          0x2A
#define KBRD_KEY_W          0x1D
#define KBRD_KEY_X          0x22
#define KBRD_KEY_Y          0x35
#define KBRD_KEY_Z          0x1A
#define KBRD_KEY_0          0x45
#define KBRD_KEY_1          0x16
#define KBRD_KEY_2          0x1E
#define KBRD_KEY_3          0x26
#define KBRD_KEY_4          0x25
#define KBRD_KEY_5          0x2E
#define KBRD_KEY_6          0x36
#define KBRD_KEY_7          0x3D
#define KBRD_KEY_8          0x3E
#define KBRD_KEY_9          0x46
#define KBRD_KEY_COMMA      0x41
#define KBRD_KEY_DOT        0x49
#define KBRD_KEY_MINUS      0x4A
#define KBRD_KEY_ANGL_BRKT  0x61
#define KBRD_KEY_BCK_SLASH  0x0E
#define KBRD_KEY_QSTN_MARK  0x4E
#define KBRD_KEY_CAP        0x55
#define KBRD_KEY_L_SQRBRKT  0x54
#define KBRD_KEY_R_PLUS     0x5B
#define KBRD_KEY_AT         0x4C
#define KBRD_KEY_HASHTAG    0x52
#define KBRD_KEY_ACCNT_U    0x5D
#define KBRD_KEY_BKSP       0x66
#define KBRD_KEY_SPACE      0x29
#define KBRD_KEY_TAB        0x0D
#define KBRD_KEY_CAPS       0x58
#define KBRD_KEY_L_SHIFT    0x12
#define KBRD_KEY_L_CTRL     0x14
#define KBRD_KEY_L_ALT      0x11
#define KBRD_KEY_R_SHIFT    0x59
#define KBRD_KEY_ENTER      0x5A
#define KBRD_KEY_ESC        0x76
#define KBRD_KEY_F1         0x05
#define KBRD_KEY_F2         0x06
#define KBRD_KEY_F3         0x04
#define KBRD_KEY_F4         0x0C
#define KBRD_KEY_F5         0x03
#define KBRD_KEY_F6         0x0B
#define KBRD_KEY_F7         0x83
#define KBRD_KEY_F8         0x0A
#define KBRD_KEY_F9         0x01
#define KBRD_KEY_F10        0x09
#define KBRD_KEY_F11        0x78
#define KBRD_KEY_F12        0x07
#define KBRD_KEY_SCROLL     0x7E
#define KBRD_KEY_NUM        0x77
#define KBRD_KEY_KP_ASTX    0x7C
#define KBRD_KEY_KP_MIN     0x7B
#define KBRD_KEY_KP_PLUS    0x79
#define KBRD_KEY_KP_DOT     0x71
#define KBRD_KEY_KP_0       0x70
#define KBRD_KEY_KP_1       0x69
#define KBRD_KEY_KP_2       0x72
#define KBRD_KEY_KP_3       0x7A
#define KBRD_KEY_KP_4       0x6B
#define KBRD_KEY_KP_5       0x73
#define KBRD_KEY_KP_6       0x74
#define KBRD_KEY_KP_7       0x6C
#define KBRD_KEY_KP_8       0x75
#define KBRD_KEY_KP_9       0x7D

// Long code:
#define KBRD_KEY_L_WIN      0xE01F
#define KBRD_KEY_R_CTRL     0xE014
#define KBRD_KEY_R_WIN      0xE027
#define KBRD_KEY_R_ALTGR    0xE011
#define KBRD_KEY_APPS       0xE02F
#define KBRD_KEY_INSERT     0xE070
#define KBRD_KEY_HOME       0xE06C
#define KBRD_KEY_PG_UP      0xE07D
#define KBRD_KEY_DELETE     0xE071
#define KBRD_KEY_END        0xE069
#define KBRD_KEY_PG_DOWN    0xE07A
#define KBRD_KEY_UP         0xE075
#define KBRD_KEY_LEFT       0xE06B
#define KBRD_KEY_DOWN       0xE072
#define KBRD_KEY_RIGHT      0xE074
#define KBRD_KEY_KP_ENTER   0xE05A
#define KBRD_KEY_KP_SLASH   0xE04A
#define KBRD_KEY_SLEEP      0xE03F
#define KBRD_KEY_WAKE       0xE05E
#define KBRD_KEY_POWER      0xE037

// Extra long code:
#define KBRD_KEY_PRINT      0xE012E07C

// Ultra long code:
#define KBRD_KEY_PAUSE      0xE11477E1F014F077

// Bit mask for LEDs
#define KBRD_SCROLL_BIT     0b00000001
#define KBRD_NUM_BIT        0b00000010
#define KBRD_CAPS_BIT       0b00000100


