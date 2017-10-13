#include <IRremote.h>

#define SONY_HDR_MARK             1200
#define SONY_HDR_SPACE             300
#define SONY_ONE_MARK              600
#define SONY_ZERO_MARK             300
#define KHZ                          1

IRsend irsend;

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
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

void loop() {
  const char* message = "btp+wss://:verysecretsecret@red.example.com/api/rpc";

  Serial.print("sending message\n");
  transmitString(message);

  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);

  delay(5000);
}

