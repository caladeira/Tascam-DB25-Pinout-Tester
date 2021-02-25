/*
  TASCAM DA88 D25 Cable Tester
  Carlos Ladeira (c) 2012

  V1.00  20120212 

 */

#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "tester.h"
#include "io_pin.h"
#include "db25_pin.h"
#include "db25_xlr.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

IOPin LCD_BackLight(A6, OUTPUT, HIGH);

IOPin Switch(A7, INPUT, HIGH);

IOPin XLR[WIRES] = {
  IOPin( A8, OUTPUT, HIGH),       // XLR pin 1
  IOPin( A9, OUTPUT, HIGH),       // XLR pin 2
  IOPin(A10, OUTPUT, HIGH)        // XLR pin 3
};

IOPin TestPoint(A11, INPUT, HIGH);        // Free test pin

byte minus_char[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000
};

byte plus_char[8] = {
  B00000,
  B00100,
  B00100,
  B11111,
  B00100,
  B00100,
  B00000,
  B00000
};

byte gnd_char[8] = {
  B00100,
  B00100,
  B11111,
  B00000,
  B01110,
  B00000,
  B00100,
  B00000
};

DB25_pin test_pin;
DB25_xlr test_xlr;
byte last_state, current_state;
byte operation_mode;
unsigned long blink_timeout;
unsigned long refresh_timeout;
byte refresh_count;
PinList pl;


void setup()
{
  //
  // set up the LCD's number of columns and rows: 
  //
  lcd.createChar(GND, gnd_char);
  lcd.createChar(PLUS, plus_char);
  lcd.createChar(MINUS, minus_char);
  lcd.begin(16, 2);
  
  //
  // Print a message to the LCD.
  //
  lcd.print("Tascam DA88 DB25");
  lcd.setCursor(0, 1);
  lcd.print("  Cable Tester");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("  Version 1.00");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Caladeira @ 2012");
  delay(1000);

  lcd.setCursor(0, 1);
  operation_mode = EEPROM.read(MODE_EEPROM_ADDR);
  if (operation_mode == TASCAM_MODE) {
    lcd.print("  TASCAM Mode   ");
    TestPoint.SetMode(OUTPUT, HIGH);
  }
  else if (operation_mode == XLR_MODE) {
    lcd.print("    XLR Mode    ");
    TestPoint.SetMode(INPUT, HIGH);
  }
  else {
    lcd.print("  TASCAM Mode   ");
    operation_mode = TASCAM_MODE;
    EEPROM.write(MODE_EEPROM_ADDR, operation_mode);
    TestPoint.SetMode(OUTPUT, HIGH);
  }
  delay(2000);
  lcd.clear();
  
  //
  // turn off backlight, start idle counter and refresh counter
  //
  LCD_BackLight.SetLow();
  blink_timeout = millis() + BLINK_SCR_IDLE_TIME;
  refresh_timeout = millis() + REFRESH_RATE;
  refresh_count = 0;

  last_state = REFRESH;
  
  #if DEBUG == TRUE
    Serial.begin(115200);
    Serial.println("Tascam DA88 Tester");
  #endif
}

//
// check for connections between DB25 and the test point
//
boolean CheckTestPin()
{
  TestPoint.SetLow();
  test_pin.ReadPins();
  TestPoint.SetHigh();

  if (test_pin.ActivePins() > 0) return true;

  return false;
}

//
// check for connections between DB25 and the XLR
//
boolean CheckXLR()
{
  XLR[GND].SetLow();
  test_xlr.ReadPins_gnd();
  XLR[GND].SetHigh();

  XLR[PLUS].SetLow();
  test_xlr.ReadPins_plus();
  XLR[PLUS].SetHigh();

  XLR[MINUS].SetLow();
  test_xlr.ReadPins_minus();
  XLR[MINUS].SetHigh();

  if (test_xlr.ActivePins() > 0) return true;

  return false;
}

//
//  called by loop function when operation mode is TASCAM
//
void TascamLoop()
{
  if (CheckTestPin()) current_state = PIN_TEST;
  else if (CheckXLR()) current_state = XLR_TEST;
  else current_state = DISCONNECTED;

  if (current_state != last_state) {

    last_state = current_state;
    
    lcd.clear();

    switch (current_state) {
      case DISCONNECTED:
        last_state = current_state;
        lcd.print(" No connection!");
        lcd.setCursor(0, 1);
        lcd.print("Check D25 cable.");
        LCD_BackLight.SetLow();
        break;
        
      case PIN_TEST:
        LCD_BackLight.SetHigh();
        test_pin.PrintLineOne();
        test_pin.PrintLineTwo();
        break;
        
      case XLR_TEST:
        LCD_BackLight.SetHigh();
        test_xlr.PrintLineOne();
        test_xlr.PrintLineTwo(refresh_count);
        break;
    }  
  }
}

//
//  called by loop function when operation mode is XLR
//
void XlrLoop()
{
  byte pin_count;
  
  pl.Init();
  

  XLR[GND].SetLow();
  if (TestPoint.State() == LOW) pl.Insert(1);
  XLR[GND].SetHigh();

  XLR[PLUS].SetLow();
  if (TestPoint.State() == LOW) pl.Insert(2);
  XLR[PLUS].SetHigh();

  XLR[MINUS].SetLow();
  if (TestPoint.State() == LOW) pl.Insert(3);
  XLR[MINUS].SetHigh();

  pin_count = pl.Total();
  if (pin_count > 1) current_state = PIN_SHORT;
  else if (pin_count == 1)  current_state = PIN_TEST;
  else current_state = DISCONNECTED;
  
  if (current_state != last_state) {

    last_state = current_state;
    
    lcd.clear();

    switch (current_state) {
      case DISCONNECTED:
        last_state = current_state;
        lcd.print(" No connection!");
        lcd.setCursor(0, 1);
        lcd.print("Check XLR cable.");
        LCD_BackLight.SetLow();
        break;
        
      case PIN_TEST:
        LCD_BackLight.SetHigh();
        lcd.print("GOOD   XLR   (");
        lcd.write((pl.ReadPin(0)-1));
        lcd.print(')');
        lcd.setCursor(0, 1);
        lcd.print("XLR Pin: ");
        lcd.print(pl.ReadPin(0));
        break;
        
      case PIN_SHORT:
        LCD_BackLight.SetHigh();
        lcd.print("*XLR PINS SHORT*");
        lcd.setCursor(0, 1);
        lcd.print("XLR: ");
        pl.Print();
        break;
    }  
  }
}

void loop()
{
  if (operation_mode == TASCAM_MODE) TascamLoop();
  else XlrLoop();

  //
  //  refresh lcd screen at a deffined rate to avoid flickering
  //
  if (refresh_timeout  < millis()) {
    last_state = REFRESH;
    refresh_timeout = millis() + REFRESH_RATE;
    refresh_count++;
  }

  //
  // if current state is disconnected and the lcd backlight is off
  // flash lcd backlight to indicate power on at a BLINK_SCR_IDLE_TIME rate
  //
  if (current_state == DISCONNECTED && blink_timeout  < millis()) {
    LCD_BackLight.SetHigh();
    delay(50);
    LCD_BackLight.SetLow();
    blink_timeout = millis() + BLINK_SCR_IDLE_TIME;
  }
  
  //
  //  track operation mode change
  //
  if (Switch.State() == LOW) {
    delay(10);
    if (Switch.State() == LOW) {
      lcd.clear();
      LCD_BackLight.SetHigh();
      if (operation_mode == TASCAM_MODE) {
        lcd.print("    XLR Mode    ");
        operation_mode = XLR_MODE;
        TestPoint.SetMode(INPUT, HIGH);
      }
      else {
        lcd.print("  TASCAM Mode   ");
        operation_mode = TASCAM_MODE;
        TestPoint.SetMode(OUTPUT, HIGH);
      }
      EEPROM.write(MODE_EEPROM_ADDR, operation_mode);
      delay(1000);
      LCD_BackLight.SetLow();
      last_state = REFRESH;
    }
  }
}

