#ifndef PAGE_H
#define PAGE_H
#include "Arduino.h"
#include "DisplayInterface.h"
#include "RotaryKnobController.h"
#include "DisplayController.h"

class Page : DisplayInterface
{
public:
    enum class PageName
    {
        DASH_PAGE = 0,
        ELEVATOR_PAGE = 1,
        SHOT_PAGE = 2,
        VALVE_PAGE = 3
    };

    Page(Page::PageName name);
    Page(int increment, int min, int max, bool isActive, Page::PageName name);
    void cleanUp(DisplayController &display);
    virtual void paint(DisplayController &display, bool isActivated, String Status) = 0;
    void clockwise();
    void counterClockwise();
    bool canActivate();
    int getModifyValue();
    void setNextPage(Page &page);
    void setPreviousPage(Page &page);
    Page::PageName getName();
    Page *getNextPage();
    Page *getPreviousPage();

private:
    bool isActive;
    int modifyValue, min, max, increment;
    PageName name;
    Page *previousPage;
    Page *nextPage;
};
#endif