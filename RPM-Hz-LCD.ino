
// Arduino windturbine sense AC phase for hz and rpm
//
// https://github.com/Electric-Storm/Arduino_windturbine_load_control/blob/main/Final_Hz_RPM_windturbine.ino
//
// schema circuit drawing
// https://raw.githubusercontent.com/Electric-Storm/Arduino_windturbine_load_control/main/20230126_181058.jpg
// for 24v windturbine i replaced 22k for 150k ohm
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
// if i replace the AC sense 22k for 150k ohm resistor
// the hz/rpm count looks ok for 24volt windturbine
// do not have the tools to check if rpm is correct
// did a 50hz 230 to 24vac grid check
// https://youtube.com/shorts/yP8ffLpqSI4?feature=share
//


// looks like the 12v an 24v windturbine
// give at +/- 50hz is 500rpm their rated voltage


// code dlloyd 
// https://forum.arduino.cc/t/about-measuring-ac-frequency-with-digital-input/400271
// https://github.com/Dlloydev

// only pin D2 and D3 are interrupt inputs on arduino nano / uno



// Adrduino windturbine VAC hz rpm
//
// https://raw.githubusercontent.com/Electric-Storm/Arduino_windturbine_load_control/main/20230126_181058.jpg
// replaced the windturbine for 230vac to 24vac transformer
// test to see if it displays 50hz
// https://youtube.com/shorts/yP8ffLpqSI4?feature=share



const byte acInputPin = 2;
const byte acCycles = 12;
volatile unsigned long startTime, stopTime;
volatile byte acCount, testState;
unsigned long acPeriod;
float acFrequency;

// voltage divider for analog A2 DC side
float vPow = 4.75;      // measured on vref to ground with multimeter
float r1 = 200000;      // 200k
float r2 = 10000;       // 10k


#include <LiquidCrystal_I2C.h>       // Include LiquidCrystal_I2C library
// https://github.com/marcoschwartz/LiquidCrystal_I2C/archive/master.zip

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Configure LiquidCrystal_I2C library with 0x27 address, 16 columns and 2 rows

void setup()
{
  Serial.begin(115200);
  pinMode(acInputPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(acInputPin), acMeasure, RISING);

  Serial.println("--------------------");
  Serial.println("WindTurbine Hz RPM Volt DC");
  Serial.println("DC VOLTMETER");
  Serial.print("Maximum Voltage: ");
  Serial.print((int)(vPow / (r2 / (r1 + r2))));
  Serial.println("V");
  Serial.println("--------------------");
  Serial.println("");

  lcd.begin();                        // A4 and A5 is i2c for lcd i2c backpack pcf8574

  lcd.backlight();                    // Turn backlight ON
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WindTurbine Hz & RPM");
  lcd.setCursor(6, 1);
  lcd.print("--       VDC");
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
    // if (acFrequency <10){lcd.print(" ");}
    // if (acFrequency<100){lcd.print(" ");}
    lcd.print(acFrequency, 1); lcd.print("  ");
    lcd.setCursor(15, 2);
    lcd.print("Hz");
    lcd.setCursor(6, 3);
    //float rpm=(acFrequency / 6) * 60;
    float rpm = (acFrequency) * 10;
    //if (rpm <10){lcd.print(" ");}
    //if (rpm<100){lcd.print(" ");}
    //if (rpm<1000){lcd.print(" ");}
    lcd.print(rpm, 0); lcd.print("  ");
    lcd.setCursor(15, 3);
    lcd.print("RPM");

    int sensorValue = analogRead(A2);
    Serial.println(sensorValue);
    float v = (sensorValue * vPow) / 1024.0;
    float voltage = v / (r2 / (r1 + r2));
   
    lcd.setCursor(6, 1);
    lcd.print(voltage, 1);
   
    delay(500);
   
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
