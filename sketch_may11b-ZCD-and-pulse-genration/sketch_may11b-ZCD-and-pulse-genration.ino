const int sepicPin = 9;   // SEPIC (Timer 1)
const int invPinA = 11;   // Inverter Leg A (Timer 2)
const int invPinB = 3;    // Inverter Leg B (Timer 2)
const int zcdPin = 2;     // Zero Crossing Input
const int analogPin = A0; // SEPIC Voltage Feedback
const int potPin = A1;    // Voltage Target Pot

const uint16_t timerTop = 100; // 20kHz for SEPIC
volatile bool currentHalfCycle = false; 
volatile int tableIndex = 0;

// Inverted Sine Table (255 = OFF, 50 = Near Full ON)
// Includes "Dead Time" zeros (255s) at the ends to prevent short circuits
const byte sineTable[] = {
  255, 230, 205, 180, 155, 130, 110, 95, 85, 75,
  70, 65, 62, 60, 60, 62, 65, 70, 75, 85,
  95, 110, 130, 155, 180, 205, 230, 255, 255, 255
};
const int tableSize = 30;

float currentDuty = 0;
const float kp = 0.05;

void setSepicDuty(float percent) {
  if (percent > 30.0) percent = 30.0; // Safety cap
  if (percent < 0.0) percent = 0.0;
  uint16_t ticks = (percent * timerTop) / 100;
  OCR1A = timerTop - ticks; 
}

// ZCD Interrupt: Synchronizes the Sine Wave to the Grid
void zcISR() {
  tableIndex = 0; // Reset sine wave to start
  currentHalfCycle = !currentHalfCycle; // Flip polarity
}

void setup() {
  // CRITICAL: Set pins HIGH (OFF) immediately
  pinMode(sepicPin, OUTPUT);
  pinMode(invPinA, OUTPUT);
  pinMode(invPinB, OUTPUT);
  digitalWrite(sepicPin, HIGH);
  digitalWrite(invPinA, HIGH);
  digitalWrite(invPinB, HIGH);

  pinMode(zcdPin, INPUT); 
  attachInterrupt(digitalPinToInterrupt(zcdPin), zcISR, FALLING);

  // Timer 1: SEPIC @ 20kHz (Pins 9)
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11); 
  ICR1 = timerTop;  

  // Timer 2: Inverter @ 31.25kHz (Pins 11 & 3)
  TCCR2A = _BV(WGM21) | _BV(WGM20); 
  TCCR2B = _BV(CS20); 

  Serial.begin(9600);
  setSepicDuty(0);
}

void loop() {
  // 1. SEPIC Feedback (Maintains constant DC for the inverter)
  int targetADC = map(analogRead(potPin), 0, 1023, 0, 550);
  int actualADC = analogRead(analogPin);
  currentDuty += ((targetADC - actualADC) * kp);
  setSepicDuty(currentDuty);

  // 2. SPWM Logic
  byte pwmVal = sineTable[tableIndex];

  if (currentHalfCycle) {
    TCCR2A &= ~_BV(COM2B1);  // Disconnect Pin 3
    digitalWrite(invPinB, HIGH); 
    TCCR2A |= _BV(COM2A1);   // Connect Pin 11
    OCR2A = pwmVal;
  } else {
    TCCR2A &= ~_BV(COM2A1);  // Disconnect Pin 11
    digitalWrite(invPinA, HIGH);
    TCCR2A |= _BV(COM2B1);   // Connect Pin 3
    OCR2B = pwmVal;
  }

  // 3. Timing for 50Hz (Approximate)
  // 30 steps per 10ms half-cycle = ~333us per step
  tableIndex++;
  if (tableIndex >= tableSize) tableIndex = tableSize - 1; 

  delayMicroseconds(320); 
} 