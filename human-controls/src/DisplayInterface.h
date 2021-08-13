#ifndef DISPLAYINTERFACE_H
#define DISPLAYINTERFACE_H
#include "DisplayController.h"
#include "Printer.h"

class DisplayInterface
{
public:
    virtual void init(int increment, int min, int max, bool isActivate, int printersLength, Printer printers[]) = 0;
    virtual void cleanUp(DisplayController &display) = 0;
    virtual void paint(DisplayController &display, bool isActivated) = 0;
    virtual void clockwise() = 0;
    virtual void counterClockwise() = 0;
    virtual bool canActivate() = 0;
};

#endif
