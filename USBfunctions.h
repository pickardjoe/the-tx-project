#ifndef USBFunctions_h
#define USBFunctions_h

// MAX3421E USB Host controller keyboard communication //
#include <Usb.h>
#include <types.h>
#include <HIDTypes.h>

extern unsigned int idVendor;      // Vendor ID (assigned by the USB-IF).
extern unsigned int idProduct;     // Product ID (assigned by the manufacturer).


void startUSB();
float getAxis(uint8_t axisNumber);
uint8_t getButton(uint8_t buttonNumber);
unsigned long getAllButtons();
bool isSignedAxis(uint8_t axisNumber);
// Poll device and print result //
void DEVICE_poll();


//raw usage, ReportData = buffer, ReportSize = size of buffer, and HID_ReportInfo_t is output
//void USB_ProcessHIDReport(uint8_t* ReportData, uint16_t ReportSize, device* a)
void USB_ProcessHIDReport(uint8_t* ReportData, uint16_t ReportSize);

//function to print unknown descriptor //
uint8_t getReportSize( char* descr_ptr);

// Initialize Device 
void DEVICE_init( void );

byte getDevDescr(byte addr );
uint8_t getTotalAxis();
uint8_t getTotalButtons();


void USBloop();

unsigned int getIdVendor();

unsigned int getIdProduct();

void processUSBinfo();
#endif

