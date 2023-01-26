

// 230vac transformer 24vac 50 hz
// added a 10k to d2


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
    Serial.println(" Hz");
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
