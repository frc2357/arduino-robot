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

    Page(unsigned int downArrow, unsigned int upArrow, unsigned int robotBatChar,
         unsigned int controllerBatChar, Page::PageType name);
    Page(bool isActive, unsigned int downArrow, unsigned int upArrow,
         unsigned int robotBatChar, unsigned int controllerBatChar, Page::PageType name);
    void cleanUp(DisplayController &display);
    virtual void paint(DisplayController &display, bool isActivated, JsonElement &object) = 0;
    virtual void clockwise(JsonElement &object) = 0;
    virtual void counterClockwise(JsonElement &object) = 0;
    bool canActivate();
    void setNextPage(Page &page);
    void setPreviousPage(Page &page);
    Page::PageType getName();
    Page *getNextPage();
    Page *getPreviousPage();

protected:
    int m_downArrow, m_upArrow, m_robotBatChar, m_controllerBatChar;

private:
    bool m_isActive;
    PageType m_name;
    Page *m_previousPage;
    Page *m_nextPage;
};
#endif