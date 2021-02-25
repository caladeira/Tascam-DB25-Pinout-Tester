/*	db25_xlr.h
*
*
*/

#ifndef DB25_XLR_DEFINED
#define DB25_XLR_DEFINED

#include <LiquidCrystal.h>
#include "tester.h"

#include "DB25_pin.h"

extern LiquidCrystal lcd;

class DB25_xlr {
  
  public:

	DB25_xlr() { }

        void ReadPins_gnd();
        void ReadPins_plus();
        void ReadPins_minus();

        byte Channel();
        byte ActivePins();
        
        void PrintLineOne();
        void PrintLineTwo(byte refresh_count);

        byte Short();
        byte WellWired();

  protected:

        DB25_pin gnd, plus, minus;
        PinList pin_list;
 
};


#endif


