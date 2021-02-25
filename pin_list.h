/*	pin_list.h
*
*
*/

#ifndef PIN_LIST_DEFINED
#define PIN_LIST_DEFINED

#include <LiquidCrystal.h>
#include "Arduino.h"
#include "tester.h"

extern LiquidCrystal lcd;

class PinList {
  public:
	PinList();
        void Init();

        byte Insert(byte pl[CHANNELS*WIRES], byte count);
        byte Insert(byte value);
        
        byte RemoveDuplicates();
        
        byte Total() { return count; }
        
        byte ReadPin(byte index);
        
        void Print();

  protected:
  
        byte pin_list[CHANNELS*WIRES];
        byte count;

};

#endif


