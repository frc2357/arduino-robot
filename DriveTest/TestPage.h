#ifndef TEST_PAGE_H
#define TEST_PAGE_H

#include "Page.h"
#include "Arduino.h"

class TestPage : public Page
{
public:
  TestPage();
  void paint(DisplayController &display, bool isActivated, TShirtCannonPayload &payload);
  void clockwise(TShirtCannonPayload &payload);
  void counterClockwise(TShirtCannonPayload &payload);
  int rangeFilter(int value);
};
#endif