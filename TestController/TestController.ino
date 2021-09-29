#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <Speck.h>
#include <RHEncryptedDriver.h>

//for feather m0
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
#define RF95_FREQ 915.0

#define CONTROLLER_ADDRESS 1
#define ROBOT_ADDRESS 2

#define RECEIVE_TIMEOUT 1000

RH_RF95 raw_driver(RFM95_CS, RFM95_INT);
Speck myCipher;
unsigned char encryptkey[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
RHEncryptedDriver driver(raw_driver, myCipher);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CONTROLLER_ADDRESS);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  while (!Serial) {} // Wait for serial port to be available

  if (!manager.init()) {
    Serial.println("init failed");
    while (1) {}
  }

  if (!raw_driver.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1) {}
  }

  myCipher.setKey(encryptkey, sizeof(encryptkey));

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  raw_driver.setTxPower(23, false);

  Serial.println("Finished init");
}

uint8_t data[] = "I am the first one";
uint8_t inputBuffer[RH_RF95_MAX_MESSAGE_LEN];

void loop()
{
  uint8_t inputLen = RH_RF95_MAX_MESSAGE_LEN;
  uint8_t from;

  digitalWrite(LED_BUILTIN, HIGH);
  if (manager.recvfromAckTimeout(inputBuffer, &inputLen, RECEIVE_TIMEOUT, &from))
  {
    if (from == ROBOT_ADDRESS) {
      digitalWrite(LED_BUILTIN, LOW);
      receiveRobotStatus((const char *)inputBuffer, inputLen);
    }
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Waiting to connect to robot...");
  }

  /*
  if (!manager.sendtoWait(data, sizeof(data), ROBOT_ADDRESS)) {
    Serial.println("sendtoWait failed");
  }
  */
}

void receiveRobotStatus(const char *buf, uint8_t len) {
  Serial.print("Robot Status: \"");
  Serial.print((char*)inputBuffer);
  Serial.println("\"");
}

bool connect()
{
  while (!manager.sendtoWait(data, sizeof(data), ROBOT_ADDRESS)) {
    Serial.println("Controller not found");
  }
  return true;
}
