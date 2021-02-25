/*	io_pin.h
*
*
*/

#ifndef IO_PIN_DEFINED
#define IO_PIN_DEFINED

#include "Arduino.h"
#include "tester.h"

class IOPin {
  public:
	IOPin(byte pin, byte mode, byte pullup_state, byte aux = 0);

        byte     State();
        void     SetMode(byte mode, byte pullup_state);
        void     SetHigh();
        void     SetLow();
        void     SetPullUpOn();
        void     SetPullUpOff();
        
        inline byte GetAux() { return _aux; }

  protected:
  
        byte    _pin;
        byte    _mode;
        byte    _aux;

};

#endif


