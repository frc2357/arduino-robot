#ifndef PAYLOAD_INTERFACE
#define PAYLOAD_INTERFACE

class PayloadInterface
{
public:
    virtual uint8_t *buildTransmission();
    virtual bool readMessage(uint8_t *message);
    virtual void print();
};

#endif