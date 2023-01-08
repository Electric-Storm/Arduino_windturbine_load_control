
// windturbine dumpload manager measure dc volt
//                         300 Kohm      15Kohm
//           +0...99vdc----\/\/\/--+---\/\/\/--------- min 0vdc ground
//                                 |
//                                 |
//                  Arduino analog A0  0-5v
// https://ohmslawcalculator.com/voltage-divider-calculator


/****************************************************/
#include <LiquidCrystal_I2C.h>                // better use i2c
display??? https://www.arduinolibraries.info/libraries/liquid-crystal-i2-c




LiquidCrystal_I2C lcd(0x27, 20, 4);          // Set the LCD address to
0x27 for a 20 chars and 4 line display
// connect LCD i2c BackPack sda scl
// Arduino uno A4=SDA A5=SCL
// Arduino nano A4=SDA A5=SCL


float vPow = 4.75;  // measured on vref to ground with multimeter
float r1 = 300000;   // 300k
float r2 = 15000;       // 15k

void setup() {
  Serial.begin(9600);

  Serial.println("--------------------");
  Serial.println("DC VOLTMETER");
  Serial.print("Maximum Voltage: ");
  Serial.print((int)(vPow / (r2 / (r1 + r2))));
  Serial.println("V");
  Serial.println("--------------------");
  Serial.println("");



     lcd.begin();               // initialize the LCD

  lcd.backlight();           // Turn on the backlight

  lcd.setCursor(0, 0); lcd.print("DC VOLTMETER");
  lcd.setCursor(0, 1); lcd.print("resistor
");lcd.print((int)(r1/1000));lcd.print("K &
");lcd.print((int)(r2/1000));lcd.print("K");
  lcd.setCursor(0, 2); lcd.print("Max. ");lcd.print((int)(vPow / (r2 /
(r1 + r2))));lcd.print(" Volt DC");
  lcd.setCursor(0, 3); lcd.print("--------------------");
  delay(2000);
}

void loop() {
 float voltread=0;
  for (int i=0; i<1000; i++){
        voltread=voltread+analogRead(1);      // do 1000 readings
  }
  voltread=voltread/1000;                    // divide by 1000 to make
it more stable

  float v = (voltread * vPow) / 1024.0;
  float v2 = v / (r2 / (r1 + r2));

  Serial.println(v2);    // print the unit as V, short for voltage on
the serial monitor

 lcd.setCursor(0, 3); lcd.print(v2,1);lcd.print(" Volt           ");




 //if(v2>=4.5){Serial.println(">= 4.5 Volt");}
// if(val>=10){Serial.println(">= 10 Volt");} // maybe switch on load relais
// if(val>=11){Serial.println(">= 11 Volt");}
// if(val>=12){Serial.println(">= 12 Volt");}
// if(val>=13){Serial.println(">= 13 Volt");}
// if(val>=14){Serial.println(">= 14 Volt");}
// if(val>=15){Serial.println(">= 15 Volt");}


  delay(1000);                 // Wait for

}
