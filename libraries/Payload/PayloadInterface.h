#ifndef PAYLOAD_INTERFACE
#define PAYLOAD_INTERFACE

class PayloadInterface
{
public:
    virtual bool buildTransmission(uint8_t *transmission, const uint8_t len);
    virtual bool readMessage(const uint8_t *message, const uint8_t len);
    virtual void print();
};

#endif