#ifndef PAGE_H
#define PAGE_H
#include "Arduino.h"
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
    Page(int increment, int min, int max, bool isActive, Page::PageType name);
    void cleanUp(DisplayController &display);
    virtual void paint(DisplayController &display, bool isActivated, const char *status) = 0;
    void clockwise();
    void counterClockwise();
    bool canActivate();
    int getModifyValue();
    void setNextPage(Page &page);
    void setPreviousPage(Page &page);
    Page::PageType getName();
    Page *getNextPage();
    Page *getPreviousPage();

private:
    bool m_isActive;
    int m_modifyValue, m_min, m_max, m_increment;
    PageType m_name;
    Page *m_previousPage;
    Page *m_nextPage;
};
#endif