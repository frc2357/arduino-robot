#ifndef PAGE_H
#define PAGE_H
#include "Arduino.h"
#include "DisplayInterface.h"
#include "RotaryKnobController.h"
#include "DisplayController.h"

//For array of printers:
//A printer with text value of "!modifyValue!" indicates where the modifiable integer will be inserted and will be updated by Json state"
//A printer with text prefixed by "!u!" indicates a field to be updated by Json state. The following text is the field name
class Page : DisplayInterface
{
public:
    Page();
    Page(int increment, int min, int max, bool isActivate, int printersLength);
    void cleanUp(DisplayController &display);
    void paint(DisplayController &display, bool isActive);
    void clockwise();
    void counterClockwise();
    bool canActivate();
    void setNextPage(Page &page);
    void setPreviousPage(Page &page);
    Page *getNextPage();
    Page *getPreviousPage();

private:
    bool isActivate;
    int modifyValue, min, max, increment, printersLength;
    Page *previousPage;
    Page *nextPage;
};
#endif