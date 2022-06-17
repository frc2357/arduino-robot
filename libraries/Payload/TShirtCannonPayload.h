#ifndef T_SHIRT_CANNON_PAYLOAD
#define T_SHIRT_CANNON_PAYLOAD

#include <Arduino.h>

class TShirtCannonPayload
{

private:
    static const int PAYLOAD_LENGTH; // Length of payload in bytes
    static const int DATA_LENGTH;   
    uint8_t m_messageType;          // 0=From robot, 1=From controller, 2=no op from controller, 3=drive x/y from controller
    uint8_t m_messageIndex;         // Count up from 0, then roll over from 31 back to 0
    uint8_t m_status;               // 0=disabled, 1=enabled, 2=adjusting (cannot prime or fire), 3=primed, 4=firing
    uint8_t m_error;                // 0=no error, 1=e-stop, 2=no controller, 3=other (check logs)
    uint8_t m_controllerDriveLeft;  // Specific to motor controller
    uint8_t m_controllerDriveRight; // Specific to motor controller
    uint8_t m_batteryVoltage;       // 0= <20, 1= 20-22, 2= 22-24,3= 24+
    uint8_t m_angle;                // 0=down, 127=up
    uint8_t m_tankPressure;         // In PSI: 0-127
    uint8_t m_firingPressure;       // In PSI: 0-127
    uint8_t m_firingTime;           // 0=100ms, 20=300ms


public:
    TShirtCannonPayload();
    uint8_t buildTransmission();
    boolean readMessage(uint8_t *message);
    void print();

};
#endif
