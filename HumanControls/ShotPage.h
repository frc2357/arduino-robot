#ifndef Shot_PAGE_H
#define Shot_pAGE_h

#include "Page.h"

class ShotPage : public Page
{
public:
    ShotPage(int increment, int min, int max);
    void paint(DisplayController &display, bool isActivated);
};

#endif