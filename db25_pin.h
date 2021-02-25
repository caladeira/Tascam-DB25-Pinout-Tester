/*	DB25_pin.h
*
*
*/

#ifndef DB25_pin_DEFINED
#define DB25_pin_DEFINED

#include <LiquidCrystal.h>
#include "Arduino.h"
#include "tester.h"
#include "io_pin.h"
#include "pin_list.h"

extern LiquidCrystal lcd;

class DB25_pin {
  
  public:
  
	DB25_pin() { Clear(); }

        void Clear();
        void ReadPins();

        byte ActivePins() { return active_pin_count; }
        byte Channel() { return channel; }
        byte Pin() { return pin; }
        char Signal() { return signal; }

        byte Short();
        
        void PrintLineOne();
        void PrintLineTwo();

        byte GetPinList(PinList & pl, byte sig = UNDEFINED);
        byte GetShortsList(PinList & pl);
        
  protected:

        unsigned long active_pins;
        byte active_pin_count;
        byte channel;
        byte pin;
        char signal;
 
};

#endif


