#include <Wire.h>
#include <SerLCD.h>
#include <RotaryEncoder.h>

//Pins
#define JOYSTICK_PIN_VRX  0 //Analog Pin for joystick x
#define JOYSTICK_PIN_VRY  1 //Analog Pin for joystick y
#define ENCODER_PIN_A     5 //Rotary knob quadrature A
#define ENCODER_PIN_B     6 //Rotary knob quadrature B
#define ENCODER_PIN_SW   13 //Gets the button for rotary knob
#define ENABLE_PIN       10 //Digital Pin for the enable button
#define PRIME_PIN        11 //Digital Pin for prime button
#define FIRE_PIN         12 //Digital Pin for the fire button
#define POWER_DOWN_PIN   19 //Hold high to keep powered up, drop to low to power down
#define I2C_SDA          20 //I2C used by LCD
#define I2C_SCL          21 //I2C used by LCD

//Other constraints
#define DISPLAY_ADDRESS 0x72      //This is the default address of the OpenLCD
#define DISPLAY_LENGTH 16         //Length of the lcd display
#define DISPLAY_WIDTH 2           //width of the lcd display
#define HANG_TIMER_DURATION 10000 //Amount in milliseconds to stay on a page before going to dash
#define USB_BAUDRATE 115200
#define DOWN_ARROW 0              //Custom char code for down arrow
#define UP_ARROW 1                //Custom char code for up arrow
#define ROBOT_BATTERY_CHAR 2      //Custom char code for robot battery bar
#define CONTROLLER_BATTERY_CHAR 3 //Custom char code for controller battery bar

//Joystick deadzones and max
#define X_DEAD_ZONE_SIZE 100 //Total size of the X deadzone
#define JOYSTICK_MAX 1023    //Maximum joystick value that comes from the sensor
#define Y_DEAD_ZONE_SIZE 100 //Total size of the y deadzone

int cycles = 0;

SerLCD lcd;
char lcdText[(16 * 2) + 1];
char strValue[8];

RotaryEncoder encoder(ENCODER_PIN_A, ENCODER_PIN_B, RotaryEncoder::LatchMode::FOUR3);

void setup()
{
  Serial.begin(USB_BAUDRATE);
  Serial.println("Human Controls Test");

  Wire.begin();
  lcd.begin(Wire);
  lcd.disableSystemMessages();

  lcd.setBacklight(255, 255, 255);
  lcd.setContrast(5);

  //pinMode(ENCODER_PIN_CLK, INPUT_PULLUP);
  //pinMode(ENCODER_PIN_DT, INPUT_PULLUP);
  pinMode(ENCODER_PIN_SW, INPUT_PULLUP);
  pinMode(ENABLE_PIN, INPUT_PULLUP);
  pinMode(PRIME_PIN, INPUT_PULLUP);
  pinMode(FIRE_PIN, INPUT_PULLUP);

  pinMode(POWER_DOWN_PIN, OUTPUT);
  digitalWrite(POWER_DOWN_PIN, HIGH);
}

void loop()
{
  encoder.tick();

  if (millis() % 100 == 0) {
    printLCD();
  }
}

void printLCD() {
  // Shut down if the encoder position is 10 and the switch is pressed
  if (digitalRead(ENCODER_PIN_SW) == 0 && encoder.getPosition() == 10) {
    digitalWrite(POWER_DOWN_PIN, LOW);
  }

  memset(lcdText, ' ', 16 * 2);

  // Prime
  writeString((digitalRead(PRIME_PIN) == 0 ? "P" : " "), 0);

  // Fire
  writeString((digitalRead(FIRE_PIN) == 0 ? "F" : " "), 2);

  // Encoder
  writeString("D", 5);
  writeValue((int) encoder.getDirection(), 6);
  writeString("P", 9);
  writeValue(encoder.getPosition(), 10);
  writeString((digitalRead(ENCODER_PIN_SW) == 0 ? "K" : " "), 15);

  // Enable
  writeString((digitalRead(ENABLE_PIN) == 0 ? "E" : " "), 16);

  // Joysticks
  writeString("Y", 16 + 2);
  writeValue(analogRead(JOYSTICK_PIN_VRY), 16 + 3);
  writeString("X", 16 + 7);
  writeValue(analogRead(JOYSTICK_PIN_VRX), 16 + 8);

  lcd.print(lcdText);
}

void writeString(const char *str, int index) {
  memcpy(lcdText + index, str, strlen(str));
}

void writeValue(int value, int index) {
  itoa(value, strValue, 10);
  memcpy(lcdText + index, strValue, strlen(strValue));
}
