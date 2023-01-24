

https://github.com/Electric-Storm/Arduino_windturbine_load_control

RPM sense from AC phase windturbine
Treid it with a cheap chinese 12v and 24v windturbine

Zenerdiode 5.1v d2 to ground
Resistor 47k d2 to ground
Maybe add capacitor 100 a 200 nanofarad d2 to ground

1 ac winding to 22k redistor to d2
Ground connected to -minvdc after 3 phase rectifier

https://raw.githubusercontent.com/Electric-Storm/Arduino_windturbine_load_control/main/20230122_225828.jpg
Add Capacitor?
https://raw.githubusercontent.com/Electric-Storm/Arduino_windturbine_load_control/main/20230124_173444.jpg


unsigned long count = 0;
unsigned long rotations = 0;
unsigned long startmillis, stopmillis;

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
}

void loop() {

  if (digitalRead(2)) {
    count++;
    delay(1);
    while (digitalRead(2)) {
      digitalWrite(13, 1);
      delay(1)
    }
    delay(1)
  }
  
  if (count == 6) { // 6 pulses = 1 rotation
    stopmillis = millis(); 
    rotations++;
    digitalWrite(13, 0);  //led blink every rotation
    Serial.print(60000UL/(stopmillis - startmillis));  Serial.print("    "); Serial.println(rotations);
    //1 minute is 60000 milliseconds / elapsed time for 1 rotation makes rpm
    count = 0; 
    startmillis = millis();
  }
  
}
