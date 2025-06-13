//Useful bitmasks for IV-18 7 seg VFD

#define digit1 0b10000000
#define digit2 0b01000000
#define digit3 0b00100000
#define digit4 0b00010000
#define digit5 0b00001000
#define digit6 0b00000100
#define digit7 0b00000010
#define digit8 0b00000001

#define SEG_A 0b10000000
#define SEG_B 0b01000000
#define SEG_C 0b00100000
#define SEG_D 0b00010000
#define SEG_E 0b00001000
#define SEG_F 0b00000100
#define SEG_G 0b00000010
#define SEG_DOT 0b00000001

#define DIG_0 SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F
#define DIG_1 SEG_B | SEG_C
#define DIG_2 SEG_A | SEG_B | SEG_D | SEG_E | SEG_G
#define DIG_3 SEG_A | SEG_B | SEG_C | SEG_D | SEG_G
#define DIG_4 SEG_B | SEG_C | SEG_F | SEG_G
#define DIG_5 SEG_A | SEG_C | SEG_D | SEG_F | SEG_G
#define DIG_6 SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G
#define DIG_7 SEG_A | SEG_B | SEG_C
#define DIG_8 SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G
#define DIG_9 SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G
#define CHAR_A SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G
#define CHAR_B SEG_C | SEG_D | SEG_E | SEG_F | SEG_G
#define CHAR_C SEG_A | SEG_D | SEG_E | SEG_F
#define CHAR_LOWER_C SEG_D | SEG_E | SEG_G
#define CHAR_D SEG_B | SEG_C | SEG_D | SEG_E | SEG_G
#define CHAR_E SEG_A | SEG_D | SEG_E | SEG_F | SEG_G
#define CHAR_F SEG_A | SEG_E | SEG_F | SEG_G
#define CHAR_G SEG_A | SEG_C | SEG_D | SEG_E | SEG_F
#define CHAR_H SEG_B | SEG_C | SEG_E | SEG_F | SEG_G
#define CHAR_I SEG_E | SEG_F
#define CHAR_J SEG_B | SEG_C | SEG_D | SEG_E
#define CHAR_K SEG_A | SEG_C | SEG_E | SEG_F | SEG_G
#define CHAR_L SEG_D | SEG_E | SEG_F
#define CHAR_M SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G
#define CHAR_N SEG_C | SEG_E | SEG_G
#define CHAR_O SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F
#define CHAR_P SEG_A | SEG_B | SEG_E | SEG_F | SEG_G
#define CHAR_Q SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G
#define CHAR_R SEG_E | SEG_G
#define CHAR_S SEG_A | SEG_C | SEG_D | SEG_F | SEG_G
#define CHAR_T SEG_D | SEG_E | SEG_F | SEG_G
#define CHAR_U SEG_B | SEG_C | SEG_D | SEG_E | SEG_F
#define CHAR_V SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G
#define CHAR_W SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G
#define CHAR_X SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G
#define CHAR_Y SEG_B | SEG_C | SEG_D | SEG_F | SEG_G
#define CHAR_Z SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G
#define CHAR_BANG SEG_B | SEG_C | SEG_DOT