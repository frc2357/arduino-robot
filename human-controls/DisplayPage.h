#ifndef DISPLAYPAGE_H
#define DISPLAYPAGE_H

class DisplayPage
{
public:
    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual void clockwise() = 0;
    virtual void counterclockwise() = 0;
    virtual void menuSelect() = 0;
};

#endif
