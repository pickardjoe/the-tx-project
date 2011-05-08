#ifndef types_h
#define types_h
#include <avr/pgmspace.h>
#include <WProgram.h>
#include <Spi.h>
#include <Max3421e.h>
#include <Max_LCD.h>
#define MAX_VALUE 32767
#define MAX_CHANNELS 10
#define MAX_ANALOG_PORT 5
#define MAX_USB_DEVICES 4
#define MAX_ANALOG_VALUE 1024

#define getUSBInfo 1
#define calibrateAxis 2

typedef void (*FuncPtr)(void);
typedef struct PROGMEM menuItem {
   const struct menuItem *Next;
   const struct menuItem *Previous;
   const struct menuItem *Parent;
   const struct menuItem *CHILD;
   const FuncPtr SelectFunc;
   char name[17];
} Menu_Item;

typedef struct
{
  uint16_t bottom;    
  uint16_t center;              
  uint16_t top;                
} Calibration_values;

typedef struct
{
  uint8_t id;
  uint16_t PID;
  uint16_t VID;
} USB_device;//with this, a function will return a USB_device_value struct consisting of uint16_t buttons and uint16_t axis[MAX_CHANNEL] array

typedef struct
{
  uint16_t buttons;
  uint16_t axis[MAX_CHANNELS];
} USB_device_values;

//this has been designed to be as small in memory as possible, while being as flexible as possible
//it is based on having to parse the buffer for instructions for anything where storing more than 3 channels
//would
typedef struct
{
char modelName[16];
uint8_t analog_mapping[MAX_CHANNELS/2];//use nibbles, less than 16 analog ports on any AVR MAX_CHANNELS must be even
uint8_t usb_mapping[MAX_CHANNELS];//divided using nibbles, HIGH nibble = USB device number, low nibble = USB device axis,USB=15 means not used
int8_t expo_values[MAX_CHANNELS];
uint8_t emptySpot;
uint8_t buffer[59];
} Model_info;
//Model_info contains a general purpose buffer, which contains usb devices
//calibration values, and button to function mappings


#endif
