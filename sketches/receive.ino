#define IR_PIN_PIN PIND
#define IR_PIN 2
#define MAX_PULSE 65000
#define RESOLUTION 20

uint16_t pulses[100][2];
uint8_t currentPulse = 0;
uint8_t currentChar = 0;
uint8_t mask = 128;
bool printSomething = false;

void setup (void) {
  Serial.begin(9600);
  Serial.println("All systems are go.");
}

bool approx (int target, int num) {
  return (num > target - target / 20)
    &&   (num < target + target / 20);
}

void loop (void) {
  uint16_t highPulse, lowPulse;
  highPulse = lowPulse = 0;

  while (IR_PIN_PIN & (1 << IR_PIN)) {
    highPulse ++;
    delayMicroseconds(RESOLUTION);

    if (highPulse >= MAX_PULSE) {
      if (printSomething) Serial.print("\n");
      currentPulse = 0;
      printSomething = false;
      return;
    }
  }

  while (! (IR_PIN_PIN & _BV(IR_PIN))) {
    lowPulse ++;
    delayMicroseconds(RESOLUTION);

    if (lowPulse >= MAX_PULSE) {
      if (printSomething) Serial.print("\n");
      currentPulse = 0;
      printSomething = false;
      return;
    }
  }

  int off  = highPulse * RESOLUTION;
  int on = lowPulse * RESOLUTION;

  if (on > 2300) {
    mask = 128;
    currentChar = 0;
  } else if (off < 1000 && on < 1000) {
    mask /= 2;
  } else if (off < 1000 && on > 1000) {
    currentChar += mask;
    mask /= 2;
  }

  if (!mask) {
    printSomething = true;
    Serial.print((char)currentChar);
    currentChar = 0;
    mask = 128;
  }
}
