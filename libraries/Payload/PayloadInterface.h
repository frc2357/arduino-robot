#ifndef PAYLOAD_INTERFACE
#define PAYLOAD_INTERFACE

class PayloadInterface
{
public:
    virtual bool buildTransmission(uint8_t *transmission, uint8_t len);
    virtual bool readMessage(uint8_t *message, uint8_t len);
    virtual void print();
};

#endif