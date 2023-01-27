
// Arduino windturbine sense AC phase for hz and rpm
//
// https://github.com/Electric-Storm/Arduino_windturbine_load_control/blob/main/Final_Hz_RPM_windturbine.ino
//
// schema circuit drawing
// https://raw.githubusercontent.com/Electric-Storm/Arduino_windturbine_load_control/main/20230126_181058.jpg
// 
// https://youtube.com/shorts/yP8ffLpqSI4?feature=share
//
// inside the cheap chinese windturbine 12 magnets 
// 6pulses per rotation
// https://youtube.com/shorts/Os1BZ1X1qRw?feature=share
//
// 12volt windturbine hz rpm LCD test
// https://youtube.com/shorts/aOyUxGy2dfM?feature=share
//
// the 12volt display rpm is better as 24volt windturbine
// maybe a diode on D2
// do not have the tools to check if rpm is correct
//
// code dlloyd 
// https://forum.arduino.cc/t/about-measuring-ac-frequency-with-digital-input/400271
// https://github.com/Dlloydev

// only pin D2 and D3 are interrupt inputs on arduino nano / uno

const byte acInputPin = 2;
const byte acCycles = 10;
volatile unsigned long startTime, stopTime;
volatile byte acCount, testState;
unsigned long acPeriod;
float acFrequency;

#include <LiquidCrystal_I2C.h>       // Include LiquidCrystal_I2C library
// https://github.com/marcoschwartz/LiquidCrystal_I2C/archive/master.zip

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Configure LiquidCrystal_I2C library with 0x27 address, 20 columns and 4 rows

void setup()
{
  Serial.begin(115200);
  pinMode(acInputPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(acInputPin), acMeasure, RISING);


  lcd.begin();                        // A4 and A5 is i2c for lcd i2c backpack pcf8574

  lcd.backlight();                    // Turn backlight ON
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("WindTurbine Hz & RPM");
}

void loop()
{
  if (testState == 2) {    // testing completed, results are ready
    EIMSK &= ~bit(INT0);   // disable INT0 interrupt
    // calculate and print
    noInterrupts();
    acPeriod = (stopTime - startTime) / acCycles;
    interrupts();
    acFrequency = 1000000.0 / acPeriod;
    //Serial.print(acPeriod);
    //Serial.print(" us  ");
    //Serial.print(acFrequency, 3);
    //Serial.print(" Hz ");
    //Serial.print((acFrequency/6)*60, 0);
    //Serial.println(" RPM ");
    lcd.setCursor(6, 2);
    lcd.print(acFrequency, 2);
    lcd.print(" Hz");
    lcd.setCursor(6, 3);
    lcd.print((acFrequency / 6) * 60, 0);
    lcd.print(" RPM");
    noInterrupts();       // <--- added
    acCount = 0;
    testState = 0;        // clear testState
    EIFR |= bit(INTF0);   // clear INT0 interrupt flag
    EIMSK |= bit(INT0);   // enable INT0 interrupt
    interrupts();         // <--- added
  }
}

void acMeasure() {
  switch (testState) {
    case 0:
      startTime = micros();
      testState = 1;
      break;
    case 1:
      acCount++;
      if (acCount == acCycles) {
        stopTime = micros();
        testState = 2;
      }
      break;
  }
}
