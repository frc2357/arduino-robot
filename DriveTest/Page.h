#ifndef PAGE_H
#define PAGE_H
#include <Arduino.h>
#include <JsonElement.h>
#include "DisplayInterface.h"
#include "RotaryKnobController.h"
#include "DisplayController.h"
#include "TShirtCannonPayload.h"

class Page : DisplayInterface
{
public:
    enum class PageType
    {
        // DASH_PAGE = 0,
        // ELEVATOR_PAGE = 1,
        // SHOT_PAGE = 2,
        // VALVE_PAGE = 3,
        // DEBUG_PAGE = 4
        TEST_PAGE = 0
    };

    Page(bool isActive, bool applyHang, /*unsigned int downArrow, unsigned int upArrow,
         unsigned int robotBatChar, unsigned int controllerBatChar,*/ Page::PageType name);
    void cleanUp(DisplayController &display);
    virtual void paint(DisplayController &display, bool isActivated, TShirtCannonPayload &payload) = 0;
    virtual void clockwise(TShirtCannonPayload &payload) = 0;
    virtual void counterClockwise(TShirtCannonPayload &payload) = 0;
    virtual int rangeFilter(int value) = 0;
    bool canActivate();
    void setNextPage(Page &page);
    void setPreviousPage(Page &page);
    bool applyHang();
    Page::PageType getName();
    Page *getNextPage();
    Page *getPreviousPage();

protected:
    bool hasUpdated();
    // int m_downArrow, m_upArrow, m_robotBatChar, m_controllerBatChar;

private:
    bool m_isActive, m_applyHang;
    PageType m_name;
    Page *m_previousPage;
    Page *m_nextPage;
};
#endif