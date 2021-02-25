/*	tester.h
*
*
*/

#ifndef TESTER_DEFINED
#define TESTER_DEFINED

#define  DEBUG                FALSE
//#define  DEBUG                TRUE

#define GND                  0
#define PLUS                 1
#define MINUS                2
#define UNDEFINED            32
#define NO_SHORT             0
#define SHORT                1
#define GND_SHORT            2

#define CHANNELS             8
#define WIRES                3

#define ALL_GND_MASK         0x00249249        //  binary: 0010 0100 1001 0010 0100 1001

#define  DISCONNECTED        0
#define  PIN_TEST            1
#define  XLR_TEST            2
#define  PIN_SHORT           3
#define  REFRESH             4

#define  MODE_EEPROM_ADDR    0
#define  TASCAM_MODE         1
#define  XLR_MODE            2

#define  BLINK_SCR_IDLE_TIME 5000
#define  REFRESH_RATE        500

#define  WIRED_OK            0
#define  WIRED_BAD           1
#define  WIRED_NO_GND        2



#endif

