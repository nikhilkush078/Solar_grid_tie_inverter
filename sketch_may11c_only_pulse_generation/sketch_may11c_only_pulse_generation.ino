const int sepicPin = 9;   // SEPIC (Timer 1)
const int invPinA = 11;   // Inverter Leg A (Timer 2)
const int invPinB = 3;    // Inverter Leg B (Timer 2)
const int analogPin = A0; // SEPIC Voltage Feedback
const int potPin = A1;    // Voltage Target Pot

const uint16_t timerTop = 100; // 20kHz for SEPIC
bool currentHalfCycle = false; 
int tableIndex = 0;

// Inverted Sine Table (255 = OFF, 50 = Near Full ON)
// Includes "Dead Time" zeros (255s) to prevent transformer short circuits
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

void setup() {
  // Set pins HIGH (OFF) immediately
  pinMode(sepicPin, OUTPUT);
  pinMode(invPinA, OUTPUT);
  pinMode(invPinB, OUTPUT);
  digitalWrite(sepicPin, HIGH);
  digitalWrite(invPinA, HIGH);
  digitalWrite(invPinB, HIGH);

  // Timer 1: SEPIC @ 20kHz
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11); 
  ICR1 = timerTop;  

  // Timer 2: Inverter @ 31.25kHz
  TCCR2A = _BV(WGM21) | _BV(WGM20); 
  TCCR2B = _BV(CS20); 

  Serial.begin(9600);
  setSepicDuty(0);
}

void loop() {
  // 1. SEPIC Feedback Loop (DC-DC Section)
  int targetADC = map(analogRead(potPin), 0, 1023, 0, 550);
  int actualADC = analogRead(analogPin);
  currentDuty += ((targetADC - actualADC) * kp);
  setSepicDuty(currentDuty);

  // 2. SPWM Logic (DC-AC Section)
  byte pwmVal = sineTable[tableIndex];

  if (!currentHalfCycle) {
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

  // 3. Auto-cycling the Sine Wave
  tableIndex++;
  
  if (tableIndex >= tableSize) {
    tableIndex = 0;           // Reset to start of sine curve
    currentHalfCycle = !currentHalfCycle; // Toggle to the other half of the transformer
  }

  // 4. Timing for 50Hz
  // (10ms per half cycle / 30 steps) = 333us per step
  delayMicroseconds(325); 
}