#include "db25_xlr.h"

void DB25_xlr::ReadPins_gnd()
{
  gnd.ReadPins();
}

void DB25_xlr::ReadPins_plus()
{
  plus.ReadPins();
}

void DB25_xlr::ReadPins_minus()
{
  minus.ReadPins();
}

byte DB25_xlr::Channel()
{
  byte g, p, m, ch = 0;
  
  g = gnd.Channel();
  p = plus.Channel();
  m = minus.Channel();

  if (p) ch = p;

  if (m) {
    if (!ch) ch = m;
    else if (ch != m) return 0;
  }

  if (g) {
    if (!ch) ch = g;
    else if (ch != g && gnd.Short() != GND_SHORT) ch = 0;
  }

  return ch;
}

byte DB25_xlr::ActivePins()
{
  return gnd.ActivePins() + plus.ActivePins() + minus.ActivePins();
}

byte DB25_xlr::Short()
{ 
  if (plus.Short() != NO_SHORT || minus.Short() != NO_SHORT) return SHORT;
  else return gnd.Short();
}

byte DB25_xlr::WellWired()
{
  if (plus.Signal() == PLUS && minus.Signal() == MINUS) {
    switch (gnd.Signal()) {
      case GND: return WIRED_OK; break;
      case UNDEFINED: return WIRED_NO_GND; break;
      default: return WIRED_BAD;
    }
  }
  else return WIRED_BAD;
}

void DB25_xlr::PrintLineOne()
{
  byte ch = Channel();

  switch (Short()) {
    
    case GND_SHORT:
      lcd.print("WARN CH");
      lcd.print(ch);
      break;
    case NO_SHORT:
      if (ch != 0) {
        switch (WellWired()) {
          case WIRED_OK: lcd.print("GOOD CH"); break;
          case WIRED_BAD: lcd.print("*BAD CH"); break;
          case WIRED_NO_GND: lcd.print("WARN CH"); break;
        }
        lcd.print(ch);
      }
      else lcd.print("*BAD CH?");
      break;
      
    case SHORT:
      lcd.print("* PINS SHORTED *");
      return;
  }

  lcd.print(" (");
  lcd.write((uint8_t)GND);
  lcd.print(',');
  lcd.write((uint8_t)PLUS);
  lcd.print(',');
  lcd.write((uint8_t)MINUS);
  lcd.print(')');
}

void DB25_xlr::PrintLineTwo(byte refresh_count)
{
  pin_list.Init();
  
  switch (Short()) {
    case NO_SHORT:
      if (refresh_count & 1) {
        if (gnd.GetPinList(pin_list, GND) == 0) pin_list.Insert(255);
        if (plus.GetPinList(pin_list, PLUS) == 0) pin_list.Insert(255);
        if (minus.GetPinList(pin_list, MINUS) == 0) pin_list.Insert(255);
      }
      else {
        if (gnd.GetPinList(pin_list) == 0) pin_list.Insert(255);
        if (plus.GetPinList(pin_list) == 0) pin_list.Insert(255);
        if (minus.GetPinList(pin_list) == 0) pin_list.Insert(255);
      }
      break;
    case SHORT:
      if (gnd.ActivePins() > 1) gnd.GetShortsList(pin_list);
      if (plus.ActivePins() > 1) plus.GetShortsList(pin_list);
      if (minus.ActivePins() > 1) minus.GetShortsList(pin_list);
      break;
    case GND_SHORT:
      pin_list.Insert(254);
      plus.GetPinList(pin_list);
      minus.GetPinList(pin_list);
      break;
  }
  
  pin_list.RemoveDuplicates();
  
  lcd.setCursor(0, 1);
  lcd.print("DB25: ");
  
  pin_list.Print();
}


