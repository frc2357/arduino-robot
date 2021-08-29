#ifndef PAGE_H
#define PAGE_H
#include "Arduino.h"
#include "DisplayInterface.h"
#include "RotaryKnobController.h"
#include "DisplayController.h"

class Page : DisplayInterface
{
public:
    Page();
    Page(int increment, int min, int max, bool isActive);
    void cleanUp(DisplayController &display);
    virtual void paint(DisplayController &display, bool isActivated, String Status) = 0;
    void clockwise();
    void counterClockwise();
    bool canActivate();
    int getModifyValue();
    void setNextPage(Page &page);
    void setPreviousPage(Page &page);
    Page *getNextPage();
    Page *getPreviousPage();

private:
    bool isActive;
    int modifyValue, min, max, increment;
    Page *previousPage;
    Page *nextPage;
};
#endif