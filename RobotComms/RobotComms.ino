#include <Wire.h>
#include <RHReliableDatagram.h>
#include <RH_RF95.h>
#include <SPI.h>
#include <Speck.h>
#include <RHEncryptedDriver.h>

#include "I2CBufferStack.h"

#define SOFTWARE_VERSION   "0.1.0"

#define SERIAL_BAUD_RATE   115200
#define I2C_DEVICE_ADDRESS 0x08

#define ROBOT_STATE_MAXLEN 300

// Defines for the 900MHz radio
#define RFM95_CS   8
#define RFM95_RST  4
#define RFM95_INT  3
#define RF95_FREQ  915.0
#define RF95_POWER 23

#define RADIO_ACK_RETRIES  1
#define RADIO_ACK_TIMEOUT  100

#define CONTROLLER_ADDRESS 1
#define ROBOT_ADDRESS 2

#define I2C_BUFFER_COUNT 12

const char *JSON_PREAMBLE = "~~~";

// Singleton instance of the radio driver
RH_RF95 raw_driver(RFM95_CS, RFM95_INT);
Speck myCipher;
unsigned char encryptkey[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
RHEncryptedDriver driver(raw_driver, myCipher);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, ROBOT_ADDRESS);

char message[] = "Test Message";
char radioBuffer[RH_RF95_MAX_MESSAGE_LEN];
uint8_t radioBufferIndex = -1;

bool isControllerConnected = false;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(SERIAL_BAUD_RATE);
  while (!Serial) {} // Wait for serial port to be available

  Serial.print("--- Robot Comms v");
  Serial.print(SOFTWARE_VERSION);
  Serial.println(" ---");

  if (!manager.init()) {
    Serial.println("init failed");
    while (1) {}
  }

  manager.setRetries(RADIO_ACK_RETRIES);
  manager.setTimeout(RADIO_ACK_TIMEOUT);

  if (!raw_driver.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1) {}
  }

  myCipher.setKey(encryptkey, sizeof(encryptkey));

  raw_driver.setTxPower(RF95_POWER, false);

  Wire.begin(I2C_DEVICE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Serial.println("--- Init Complete ---");
}

void loop()
{
  //uint8_t len = sizeof(buf);
  //uint8_t from;

  /*
  if (manager.available())
  {
    Serial.println("Robot received a message");
    // Wait for a message addressed to us from the client

    if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      Serial.println(String(from));

      // Send a reply back to the originator client
      if (!manager.sendtoWait((uint8_t*)message, strlen(message) + 1, CONTROLLER_ADDRESS))
        connect();
    }
  }
  */
}

bool isPreamble(const char *data) {
  return strncmp(data, JSON_PREAMBLE, strlen(JSON_PREAMBLE)) == 0;
}

boolean sendRadioMessage(uint8_t* message, uint8_t length, uint8_t address) {
  digitalWrite(LED_BUILTIN, HIGH);
  bool success = manager.sendtoWait(message, length, address);
  if (success) {
    isControllerConnected = true;
    digitalWrite(LED_BUILTIN, LOW);
    return true;
  } else {
    isControllerConnected = false;
    digitalWrite(LED_BUILTIN, LOW);
    return false;
  }
}

void receiveEvent(int howMany) {
  char buffer[33];
  size_t byteCount = 0;

  while (byteCount < 32 && Wire.available()) {
    buffer[byteCount] = Wire.read();
    byteCount++;
  }
  buffer[byteCount] = '\0';

  if (isPreamble(buffer)) {
    if (radioBufferIndex >= 0) {
      radioBuffer[radioBufferIndex + 1] = '\0';
      sendRadioMessage((uint8_t*)radioBuffer, strlen(radioBuffer) + 1, CONTROLLER_ADDRESS);
    }

    radioBufferIndex = 0;
    return;
  }

  if (radioBufferIndex >= 0) {
    if (byteCount > (RH_RF95_MAX_MESSAGE_LEN - radioBufferIndex)) {
      Serial.println("ERROR: Radio buffer overflow!");
      return;
    }
    memcpy(radioBuffer + radioBufferIndex, buffer, byteCount);
    radioBufferIndex += byteCount;
  }
}
