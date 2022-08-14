#include "RobotComms.h"

#define RFM95_SS 8
#define RFM95_INT 3

#define RF95_FREQ 915.0

#define SERIAL_BAUD_RATE   115200
#define I2C_HOST_ADDRESS   0x08
#define I2C_DEVICE_ADDRESS 0x04

RobotComms comms(RFM95_SS, RFM95_INT, I2C_HOST_ADDRESS, I2C_DEVICE_ADDRESS);

void setup() {
    Serial.begin(115200);
    comms.init(RF95_FREQ);
}

void loop() {
    comms.update();
}