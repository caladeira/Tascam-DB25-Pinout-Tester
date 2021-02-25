#include "pin_list.h"

PinList::PinList()
{
  count = 0;
}

void PinList::Init()
{
  count = 0;
}

byte PinList::Insert(byte pl[CHANNELS*WIRES], byte cnt)
{
  byte i = 0;
  
  while (count < CHANNELS*WIRES && i < cnt) {
    pin_list[count] = pl[i];
    count++;
    i++;
  }
  
  return i;
}

byte PinList::Insert(byte value)
{
  if (count < CHANNELS*WIRES) pin_list[count++] = value;
}

byte PinList::RemoveDuplicates()
{
  byte pl[CHANNELS*WIRES], sz = 0, val, i, j;
  boolean found;
  
  for (i=0; i<count; i++) {
    
    found = false;
    val = pin_list[i];
    
    for (j=0; j<sz; j++) {
      if (val == pl[j] && val != 255 && val != 0) {
        found = true;
        break;
      }
    }

    if (!found) {
      pl[sz] = val;
      sz++;
    }

  }
  
  for (i=0; i<sz; i++) pin_list[i] = pl[i];
  count = sz;

  return count;
}

byte PinList::ReadPin(byte index)
{
  if (index < count) return pin_list[index];
  return 0;
}

void PinList::Print()
{
  for (byte i=0; i<count; i++) {
    if (i) lcd.print(',');
    switch (pin_list[i]) {
      case 255: lcd.print("--"); break;
      case 254: lcd.print("##"); break;
      case 0:   lcd.print("  "); break;
      default:
        if (pin_list[i] < 10) lcd.print('0');
        lcd.print(pin_list[i]);
    }
  }
}

