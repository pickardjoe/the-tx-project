#include "HIDTypes.h"

//typedef struct{ 
uint8_t offsets[maxItems];
uint8_t bitSizes[maxItems];
uint16_t maxes[8];
uint16_t mins[8];
uint16_t axis[8];//8 is max number of axis for HID joystick device
unsigned long allButtons;//8 is max number of axis for HID joystick device
uint8_t axisSign;

uint8_t reportLength;
uint8_t totalItems;
uint8_t totalAxis = 0;
uint8_t totalButtons = 0;
//} device; // this will be defined as a class when I break this file down


///STUFF THAT NEEDS TO BE ADDED TO USB.cpp


uint8_t numberOfItems;

uint8_t i = 0; 

EP_RECORD ep_record[ 2 ];  //endpoint record structure for the keyboard
 

MAX3421E Max;
USB Usb;
bool started = false;
bool initialized = false;
uint8_t address = 0;
uint8_t addDelay = 0;
  char buf[ 50 ] = { 0 };      //keyboard buffer //using 50 just to make sure we have room
// LiquidCrystal display with:
// rs on pin 12
// rw on pin 11
// enable on pin 10
// d4, d5, d6, d7 on pins 5, 4, 3, 2
//LiquidCrystal lcd(8 ,1, 7, 6, 5, 4, 3);//we're not using pin 1 in reality and only feeding that value to the library