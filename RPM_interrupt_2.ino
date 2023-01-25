
#include <LiquidCrystal_I2C.h>                // better use i2c display??? https://www.arduinolibraries.info/libraries/liquid-crystal-i2-c

LiquidCrystal_I2C lcd(0x27, 20, 4);          // Set the LCD address to 0x27 for a 20 chars and 4 line display
// connect LCD i2c BackPack sda scl
// Arduino uno A4=SDA A5=SCL
// Arduino nano A4=SDA A5=SCL


int count = 0;
unsigned long rotations = 0;
unsigned long startmillis, stopmillis, getmillis;
int rpm;

void setup() {
  Serial.begin(230400);
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), counter, RISING);
  lcd.begin();               // initialize the LCD
  lcd.backlight();           // Turn on the backlight
}



void counter() {   // called by interrupt d2 rising
  count++;
  //delay(1);
  while (digitalRead(2)) {
   digitalWrite(13, 0);
  delay(1);
  }
 
  rotation();
}

void rotation() {
  if (count >= 6) { // 6 pulses = 1 rotation
    stopmillis = millis();
    rotations++;
    digitalWrite(13, 1);
    rpm = 60000UL / (stopmillis - startmillis);
    //1 minute is 60000 milliseconds / elapsed time for 1 rotation makes rpm
    count = 0;
    startmillis = millis();
  }
}

void loop() {

    Serial.print(rpm);
    Serial.print("    ");
    Serial.println(rotations);
    //getmillis = millis();
    lcd.setCursor(0, 0); lcd.print(rpm); lcd.print("   ");
    ///Serial.println(millis() - getmillis);
delay(500);
}
