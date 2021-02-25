#include "io_pin.h"

IOPin::IOPin(byte pin, byte mode, byte pullup_state, byte aux)
{
  _pin = pin;
  _aux = aux;
  _mode = 255;

  SetMode(mode, pullup_state);
}

void IOPin::SetMode(byte mode, byte pullup_state)
{
  if (mode == INPUT || mode == OUTPUT) {
    _mode = mode;
    pinMode(_pin, _mode);
    digitalWrite(_pin, pullup_state);
  }
}

byte IOPin::State()
{
  if (_mode == INPUT) {
    // I had some problems with some pins that read erratic
    // values when touching wires with my hands!
    // I've fixed it with this 100uS delay    
    delayMicroseconds(100);
    return digitalRead(_pin);
  }
  else return 255;
}

void IOPin::SetHigh()
{
    if (_mode == OUTPUT) digitalWrite(_pin, HIGH);
    else return;
}

void IOPin::SetLow()
{
    if (_mode == OUTPUT) digitalWrite(_pin, LOW);
    else return;
}

void IOPin::SetPullUpOn()
{
    if (_mode == INPUT) digitalWrite(_pin, HIGH);
    else return;
}

void IOPin::SetPullUpOff()
{
    if (_mode == INPUT) digitalWrite(_pin, LOW);
    else return;
}



