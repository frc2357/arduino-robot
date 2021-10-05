#include <RH_RF95.h>

//for feather m0
#define RFM95_CS 8
#define RFM95_INT 3
#define RF95_FREQ 915.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  while (!Serial) {} // Wait for serial port to be available

  if (!rf95.init()) {
    Serial.println("init failed");
    while (1) {}
  }

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1) {}
  }

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);

  Serial.println("Finished init");
}

uint8_t data[] = "I am the first one";
uint8_t inputBuffer[RH_RF95_MAX_MESSAGE_LEN];

void loop()
{
  uint8_t inputLen = RH_RF95_MAX_MESSAGE_LEN;
  uint8_t from;

  digitalWrite(LED_BUILTIN, HIGH);

  if (rf95.waitAvailableTimeout(500)) {
    if (rf95.recv(inputBuffer, &inputLen)) {
      digitalWrite(LED_BUILTIN, LOW);
      receiveRobotStatus((const char *)inputBuffer, inputLen);
    } else {
      Serial.println("recv failed");
    }
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Waiting for robot connection");
  }
}

void receiveRobotStatus(const char *buf, uint8_t len) {
  Serial.print("Robot Status: \"");
  Serial.print((char*)inputBuffer);
  Serial.println("\"");
}
