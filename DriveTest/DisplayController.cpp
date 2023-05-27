#include "DisplayController.h"

// DisplayController::DisplayController(unsigned int displayAddress, unsigned int displayLen, unsigned int displayWidth)
//     : m_lcd(displayAddress, displayLen, displayWidth)
// {
// }

void DisplayController::init(/*unsigned int downArrow, unsigned int upArrow*/)
{
    // uint8_t down[8] = {0x4, 0x4, 0x4, 0x4, 0x4, 0x1f, 0xe, 0x4};
    // uint8_t up[8] = {0x4, 0xe, 0x1f, 0x4, 0x4, 0x4, 0x4};
    Wire.begin();
    this->m_lcd.begin(Wire);
    this->m_lcd.disableSystemMessages();
    this->m_lcd.setBacklight(255, 255, 255);
    this->m_lcd.setContrast(5);
    // this->m_lcd.createChar(downArrow, down);
    // this->m_lcd.createChar(upArrow, up);

    this->clear();
}

void DisplayController::clear()
{
    memset(m_lcdText, ' ', 32);
    this->m_lcd.clear();
    m_hasChanged = true;
}

void DisplayController::stringSetRegion(int x, int y, const char *text)
{
    memcpy(m_lcdText + ((y * 16) + x), text, strlen(text));
    m_hasChanged = true;
}

void DisplayController::intSetRegion(int x, int y, int num)
{
    itoa(num, m_strInt, 10);
    stringSetRegion(x, y, m_strInt);
    m_hasChanged = true;
}

void DisplayController::print()
{
  Serial.println(m_hasChanged);
  if (m_hasChanged) {
    m_lcd.print(m_lcdText);
  }
  m_hasChanged = false;
}

// void DisplayController::printRegion(int x, int y, String text)
// {
//     this->m_lcd.setCursor(x, y);
//     this->m_lcd.print(text);
// }

// void DisplayController::printRegion(int x, int y, int code)
// {
//     this->m_lcd.setCursor(x, y);
//     this->m_lcd.write(code);
// }

// void DisplayController::printBattery(int x, int y, int code, double level)
// {
//     uint8_t bat[8] = {isFilled(level, 85.7142), isFilled(level, 71.4285), isFilled(level, 57.1428),
//                       isFilled(level, 42.8751), isFilled(level, 28.5714), isFilled(level, 14.2857), isFilled(level, 0.0)};

//     this->m_lcd.createChar(code, bat);

//     this->printRegion(x, y, code);
// }

// byte DisplayController::isFilled(double level, double threshold)
// {
//     return level > threshold ? 0x1f : 0x0;
// }