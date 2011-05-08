#ifndef HIDTypes_h
#define HIDTypes_h
#include "WProgram.h"
#include "Usb.h"

#define maxItems 25
#define LOBYTE(x) ((char*)(&(x)))[0]
#define HIBYTE(x) ((char*)(&(x)))[1]
#define BUFSIZE 256 //buffer size

#define DEVICE_ADDR  1
#define DEVICE_EP    1
#define EP_MAXPKTSIZE  40
#define EP_POLL        0x0a


/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2010  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/
/** \ref HID_ReportItem_t.ItemFlags flag for constant data. */
#define IOF_CONSTANT             (1 << 0)
#define DATA_SIZE_MASK           0x03
#define TYPE_MASK                0x0C
#define TAG_MASK                 0xF0

#define DATA_SIZE_0              0x00
#define DATA_SIZE_1              0x01
#define DATA_SIZE_2              0x02
#define DATA_SIZE_4              0x03

#define TYPE_MAIN                0x00
#define TYPE_GLOBAL              0x04
#define TYPE_LOCAL               0x08

#define TAG_MAIN_INPUT           0x80
#define TAG_MAIN_OUTPUT          0x90
#define TAG_MAIN_COLLECTION      0xA0
#define TAG_MAIN_FEATURE         0xB0
#define TAG_MAIN_ENDCOLLECTION   0xC0

#define TAG_GLOBAL_USAGEPAGE     0x00
#define TAG_GLOBAL_LOGICALMIN    0x10
#define TAG_GLOBAL_LOGICALMAX    0x20
#define TAG_GLOBAL_PHYSMIN       0x30
#define TAG_GLOBAL_PHYSMAX       0x40
#define TAG_GLOBAL_UNITEXP       0x50
#define TAG_GLOBAL_UNIT          0x60
#define TAG_GLOBAL_REPORTSIZE    0x70
#define TAG_GLOBAL_REPORTID      0x80
#define TAG_GLOBAL_REPORTCOUNT   0x90
#define TAG_GLOBAL_PUSH          0xA0
#define TAG_GLOBAL_POP           0xB0

#define TAG_LOCAL_USAGE          0x00
#define TAG_LOCAL_USAGEMIN       0x10
#define TAG_LOCAL_USAGEMAX       0x20



//typedef struct{ 
extern uint8_t offsets[maxItems];
extern uint8_t bitSizes[maxItems];
extern uint16_t maxes[8];
extern uint16_t mins[8];
extern uint16_t axis[8];//8 is max number of axis for HID joystick device
extern unsigned long allButtons;//8 is max number of axis for HID joystick device
extern uint8_t axisSign;

extern uint8_t reportLength;
extern uint8_t totalAxis;
extern uint8_t totalButtons;
extern uint8_t totalItems;
//} device; // this will be defined as a class when I break this file down


///STUFF THAT NEEDS TO BE ADDED TO USB.cpp


extern uint8_t numberOfItems;

extern uint8_t i;

extern EP_RECORD ep_record[ 2 ];  //endpoint record structure for the keyboard
 

extern MAX3421E Max;
extern USB Usb;
extern bool started;
extern bool initialized;
extern uint8_t address;
extern uint8_t addDelay;
extern char buf[ 50 ];
// LiquidCrystal display with:
// rs on pin 12
// rw on pin 11
// enable on pin 10
// d4, d5, d6, d7 on pins 5, 4, 3, 2
//LiquidCrystal lcd(8 ,1, 7, 6, 5, 4, 3);//we're not using pin 1 in reality and only feeding that value to the library
#endif