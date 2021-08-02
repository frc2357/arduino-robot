#include "Printer.h"

Printer::Printer(int x, int y, String text, bool isActive)
{
    this->x = x;
    this->y = y;
    this->text = text;
    this->isActive = isActive;
}
int Printer::getX()
{
    return this->x;
}
int Printer::getY()
{
    return this->y;
}
String Printer::getText()
{
    return this->text;
}
bool Printer::isActiveText()
{
    return this->isActive;
}
void Printer::setX(int x)
{
    this->x = x;
}
void Printer::setY(int y)
{
    this->y = y;
}
void Printer::setText(String text)
{
    this->text = text;
}
