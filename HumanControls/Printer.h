#ifndef PRINTER_H
#define PRINTER_H

#include "Arduino.h"

class Printer
{
public:
    Printer(int x, int y, String text, bool isActive);
    int getX();
    int getY();
    String getText();
    bool isActiveText();
    void setX(int x);
    void setY(int y);
    void setText(String text);

private:
    int x, y;
    String text;
    bool isActive;
};
#endif