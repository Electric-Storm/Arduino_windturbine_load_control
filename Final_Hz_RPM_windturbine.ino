8

// Adrduino windturbine ssense AC phase for hz and rpm
//
// https://github.com/Electric-Storm/Arduino_windturbine_load_control/blob/main/Final_Hz_RPM_windturbine.ino
//
// https://raw.githubusercontent.com/Electric-Storm/Arduino_windturbine_load_control/main/20230126_181058.jpg
// 
// https://youtube.com/shorts/yP8ffLpqSI4?feature=share
//
// inside the cheap chinese windturbine 12 magnets 
// 6pulses per rotation
// https://youtube.com/shorts/Os1BZ1X1qRw?feature=share


const byte acInputPin = 2;
const byte acCycles = 10;
volatile unsigned long startTime, stopTime;
volatile byte acCount, testState;
unsigned long acPeriod;
float acFrequency;

void setup()
{
  Serial.begin(115200);
  pinMode(acInputPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(acInputPin), acMeasure, RISING);
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
    Serial.print(acPeriod);
    Serial.print(" us  ");
    Serial.print(acFrequency, 3);
    Serial.print(" Hz ");
    Serial.print((acFrequency/6)*60, 0); // 6 pulses per rotation x 60 seconds
    Serial.println(" RPM ");
    acCount = 0;
    testState = 0;        // clear testState
    EIFR |= bit(INTF0);   // clear INT0 interrupt flag
    EIMSK |= bit(INT0);   // enable INT0 interrupt
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
