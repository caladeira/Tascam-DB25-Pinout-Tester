#include "DB25_pin.h"

extern PinList pl;

IOPin  TascamDB25[CHANNELS][WIRES] = {
  { IOPin(44, INPUT, HIGH, 25), IOPin(42, INPUT, HIGH, 24), IOPin(45, INPUT, HIGH, 12) },    // channel 1 (gnd, +, -)
  { IOPin(43, INPUT, HIGH, 11), IOPin(41, INPUT, HIGH, 10), IOPin(40, INPUT, HIGH, 23) },    // channel 2 (gnd, +, -)
  { IOPin(38, INPUT, HIGH, 22), IOPin(36, INPUT, HIGH, 21), IOPin(39, INPUT, HIGH,  9) },    // channel 3 (gnd, +, -)
  { IOPin(37, INPUT, HIGH,  8), IOPin(35, INPUT, HIGH,  7), IOPin(34, INPUT, HIGH, 20) },    // channel 4 (gnd, +, -)
  { IOPin(32, INPUT, HIGH, 19), IOPin(30, INPUT, HIGH, 18), IOPin(33, INPUT, HIGH,  6) },    // channel 5 (gnd, +, -)
  { IOPin(31, INPUT, HIGH,  5), IOPin(29, INPUT, HIGH,  4), IOPin(28, INPUT, HIGH, 17) },    // channel 6 (gnd, +, -)
  { IOPin(26, INPUT, HIGH, 16), IOPin(24, INPUT, HIGH, 15), IOPin(27, INPUT, HIGH,  3) },    // channel 7 (gnd, +, -)
  { IOPin(25, INPUT, HIGH,  2), IOPin(23, INPUT, HIGH,  1), IOPin(22, INPUT, HIGH, 14) }     // channel 8 (gnd, +, -)
};

IOPin dummy_pin1(47, INPUT, HIGH,  13);  // to avoid problems with unused DB25 pin 13
IOPin dummy_pin2(46, INPUT, HIGH);       // to avoid problems with unused wire in the 26 way flat cable

void DB25_pin::Clear()
{ 
  active_pins = 0; 
  active_pin_count = 0; 
  channel = 0;
  pin = 0;
  signal = UNDEFINED;
}

/*
  Cycle all 24 pins to check which are at low state
*/
void DB25_pin::ReadPins()
{
  Clear();
  
  for (byte i=0; i<CHANNELS; i++) {
    for (byte j=0; j<WIRES; j++) {
      if (TascamDB25[i][j].State() == LOW) {
        active_pin_count++;
        active_pins |= long(1) << (i*3 + j);
        if (!channel) channel = i + 1;
        if (!pin) pin = TascamDB25[i][j].GetAux();
        if (signal == UNDEFINED) signal = j;
      }
    }
  }
}

byte DB25_pin::GetPinList(PinList & pl, byte sig)
{
  for (byte i=0; i<CHANNELS; i++) {
    for (byte j=0; j<WIRES; j++) {
      if ((long(1) << (i*3 + j)) & active_pins) {
        if (signal == sig || sig == UNDEFINED) pl.Insert(TascamDB25[i][j].GetAux());
        else pl.Insert(0);
      }
    }
  }

  return pl.Total();
}

byte DB25_pin::GetShortsList(PinList & pl)
{
  if (active_pin_count > 1) {
    for (byte i=0; i<CHANNELS; i++) {
      for (byte j=0; j<WIRES; j++) {
        if ((long(1) << (i*3 + j)) & active_pins) {
          pl.Insert(TascamDB25[i][j].GetAux());
        }
      }
    }
  }
  
  return pl.Total();
}

byte DB25_pin::Short()
{ 
  if (active_pin_count > 1) {
    if ((~ALL_GND_MASK & active_pins) == 0) return GND_SHORT;
    else return SHORT;
  }
  else return NO_SHORT;
}

void DB25_pin::PrintLineOne()
{
  lcd.setCursor(0, 0);
  
  switch (Short()) {
    case NO_SHORT:
      lcd.print("GOOD  CH");
      lcd.print(channel);
      lcd.print(" (");
      lcd.write(signal);
      lcd.print(')');
      break;
    
    case GND_SHORT:
      lcd.print("WARN GND SHORTED");
      break;

    case SHORT:
      lcd.print("* PINS SHORTED *");
      return;
  }
}


void DB25_pin::PrintLineTwo()
{
  byte p, cnt = 0;

  pl.Init();

  lcd.setCursor(0, 1);
  lcd.print("DB25: ");

  switch (Short()) {
    case NO_SHORT:
      lcd.print(pin);
      break;

    case GND_SHORT:
    case SHORT:
      GetShortsList(pl);
      pl.Print();
      break;
  }
}


