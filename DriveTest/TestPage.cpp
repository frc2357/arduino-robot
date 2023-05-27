#include "TestPage.h"

TestPage::TestPage() : Page(true, false, Page::PageType::TEST_PAGE)
{
}

void TestPage::paint(DisplayController &display, bool isActivated, TShirtCannonPayload &payload)
{
  uint8_t time = payload.getFiringTime();
  
  display.clear();

  display.stringSetRegion(0, 0, "L:");
  display.stringSetRegion(0, 1, "R:");

  display.intSetRegion(3, 0, payload.getControllerDriveLeft());

  display.intSetRegion(3, 1, payload.getControllerDriveRight());

  display.intSetRegion(8, 0, (100 + (time * 10)));
}

void clockwise(TShirtCannonPayload &payload)
{
  return;
}
void counterClockwise(TShirtCannonPayload &payload)
{
  return;
}
int rangeFilter(int value)
{
  return 0;
}