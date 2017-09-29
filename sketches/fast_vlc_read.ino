#define MAX_PULSE 65000
#define THRESHOLD 10

int numSamples = 0;
volatile byte reading;
long t, t0;

void setup () {
  Serial.begin(115200);

  ADCSRA = 0;             // clear ADCSRA register
  ADCSRB = 0;             // clear ADCSRB register
  ADMUX |= (0 & 0x07);    // set A0 analog input pin
  ADMUX |= (1 << REFS0);  // set reference voltage
  ADMUX |= (1 << ADLAR);  // left align ADC value to 8 bits from ADCH register

  // sampling rate is [ADC clock] / [prescaler] / [conversion clock cycles]
  // for Arduino Uno ADC clock is 16 MHz and a conversion takes 13 clock cycles
  ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    // 32 prescaler for 38.5 KHz
  //ADCSRA |= (1 << ADPS2);                     // 16 prescaler for 76.9 KHz
  //ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz

  ADCSRA |= (1 << ADATE); // enable auto trigger
  ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN);  // enable ADC
  ADCSRA |= (1 << ADSC);  // start ADC measurements
}

volatile bool countingHigh = true;
volatile uint16_t highPulse = 0;
volatile uint16_t lowPulse = 0;
volatile byte mask = 128;
volatile byte currentChar;
volatile bool printSomething = false;

ISR (ADC_vect) {
  reading = ADCH;

  /*
  Serial.print("0 ");
  Serial.print(reading, DEC);
  Serial.print(" 100\n");*/
  
  if (countingHigh && reading > THRESHOLD) {
     highPulse ++;

     if (highPulse >= MAX_PULSE) {
       if (printSomething) Serial.print("\n");
       printSomething = false;
       return;
     }
  } else if (!countingHigh && reading < THRESHOLD) {
    lowPulse ++;

    if (lowPulse >= MAX_PULSE) {
      if (printSomething) Serial.print("\n");
      printSomething = false;
    }
  } else if (countingHigh) {
    countingHigh = false;
  } else {
    /*Serial.print(highPulse, DEC);
    Serial.print(" ");
    Serial.print(lowPulse, DEC);
    Serial.print("\n");*/
    
    countingHigh = true;
    if (highPulse > 70) {
      mask = 128;
      currentChar = 0;
    } else if (lowPulse < 30 && highPulse < 30) {
      mask /= 2;
    } else if (lowPulse < 30 && highPulse > 30) {
      currentChar += mask;
      mask /= 2;
    }

    lowPulse = 0;
    highPulse = 0;
  }

  if (!mask) {
    printSomething = true;
    Serial.print((char)currentChar);
    currentChar = 0;
    mask = 128;
  }
  
  numSamples ++;
}

void loop () {
  /*if (numSamples >= 1000) {
    t = micros() - t0;

    Serial.println(reading, DEC);
    Serial.print("Sampling frequency: ");
    Serial.print((float) 1000000 / t);
    Serial.println(" KHz");
    delay(400);

    t0 = micros();
    numSamples = 0;
  } */
  delay(100);
}
