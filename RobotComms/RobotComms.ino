#include <Wire.h>
#include <RH_RF95.h>
#include <SPI.h>

#define SOFTWARE_VERSION   1

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
RH_RF95 rf95(RFM95_CS, RFM95_INT);

char message[] = "Test Message";
char radioBuffer[RH_RF95_MAX_MESSAGE_LEN];
uint8_t radioBufferIndex = -1;

bool isControllerConnected = false;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(SERIAL_BAUD_RATE);
  if (Serial) {
    Serial.print("--- Robot Comms v");
    Serial.print(SOFTWARE_VERSION);
    Serial.println(" ---");
  }

  if (!rf95.init()) {
    Serial.println("init failed");
    while (1) {}
  }

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1) {}
  }

  rf95.setTxPower(RF95_POWER, false);

  Wire.begin(I2C_DEVICE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Serial.println("--- Init Complete ---");
}

void loop() {}

bool isPreamble(const char *data) {
  return strncmp(data, JSON_PREAMBLE, strlen(JSON_PREAMBLE)) == 0;
}

void sendRadioMessage(uint8_t* message, uint8_t length, uint8_t address) {
  digitalWrite(LED_BUILTIN, HIGH);
  rf95.send(message, length);
  rf95.waitPacketSent();
  digitalWrite(LED_BUILTIN, LOW);
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
