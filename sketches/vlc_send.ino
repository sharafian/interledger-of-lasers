#include <IRremote.h>

#define SONY_HDR_MARK             1200
#define SONY_HDR_SPACE             300
#define SONY_ONE_MARK              600
#define SONY_ZERO_MARK             300
#define KHZ                         38

IRsend irsend;

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(8, INPUT);
}

uint8_t currentBit;
void transmitByte (uint8_t data) {
  for (currentBit = 128; currentBit; currentBit >>= 1) {
    
    if (currentBit & data) irsend.mark(SONY_ONE_MARK);
    else irsend.mark(SONY_ZERO_MARK);
    
    irsend.space(SONY_HDR_SPACE);
  }
}

void transmitString (const char* str) {
  unsigned int len = strlen(str);

  irsend.enableIROut(KHZ);
  irsend.mark(SONY_HDR_MARK);
  irsend.space(SONY_HDR_SPACE);

  for (uint16_t i = 0; i < len; ++i) {
    transmitByte(str[i]);
  }

  irsend.space(0);
}

const char* message = "btp+ws://:verysecretsecret@localhost:3000/";
//const char * message = "MDAxY2xvY2F0aW9uIGh0dHA6Ly9teWJhbmsvCjAwMjZpZGVudGlmaWVyIHdlIHVzZWQgb3VyIHNlY3JldCBrZXkKMDAxZGNpZCBhY2NvdW50ID0gMzczNTkyODU1OQowMDIwY2lkIHRpbWUgPCAyMDIwLTAxLTAxVDAwOjAwCjAwMjJjaWQgZW1haWwgPSBhbGljZUBleGFtcGxlLm9yZwowMDJmc2lnbmF0dXJlID8f19FL+bkC9p/aoMmIecC7GxdOcLVyUnrv6lJMM7NSCg==";
int state;

void loop() {
  state = digitalRead(8);
  digitalWrite(13, state);
  if (state == HIGH) {
    Serial.print("sending message");
    transmitString(message);
    delay(500);
   }
  delay(50);
}

