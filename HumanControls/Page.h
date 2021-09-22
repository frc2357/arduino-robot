#ifndef PAGE_H
#define PAGE_H
#include "Arduino.h"
#include <JsonEl.h>
#include "DisplayInterface.h"
#include "RotaryKnobController.h"
#include "DisplayController.h"

class Page : DisplayInterface
{
public:
    enum class PageType
    {
        DASH_PAGE = 0,
        ELEVATOR_PAGE = 1,
        SHOT_PAGE = 2,
        VALVE_PAGE = 3
    };

    Page(Page::PageType name);
    Page(bool isActive, Page::PageType name);
    void cleanUp(DisplayController &display);
    virtual void paint(DisplayController &display, bool isActivated, JsonState &state) = 0;
    virtual void clockwise(JsonState &state) = 0;
    virtual void counterClockwise(JsonState &state) = 0;
    bool canActivate();
    void setNextPage(Page &page);
    void setPreviousPage(Page &page);
    Page::PageType getName();
    Page *getNextPage();
    Page *getPreviousPage();

private:
    bool m_isActive;
    PageType m_name;
    Page *m_previousPage;
    Page *m_nextPage;
};
#endif