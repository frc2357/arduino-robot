#include "RobotComms.h"

#define RFM95_SS 8
#define RFM95_INT 3

#define RF95_FREQ 915.0

#define SERIAL_BAUD_RATE   115200
#define I2C_HOST_ADDRESS   0x08
#define I2C_DEVICE_ADDRESS 0x09

RobotComms comms(RFM95_SS, RFM95_INT, I2C_HOST_ADDRESS);

void setup() {
    Serial.begin(115200);
    comms.init(RF95_FREQ, onI2CReceive, onI2CRequest);
}

void loop() {
    comms.update();
}

void onI2CReceive(int bytesRead) {
    comms.onI2CReceive(bytesRead);
}

void onI2CRequest() {
    comms.onI2CRequest();
}