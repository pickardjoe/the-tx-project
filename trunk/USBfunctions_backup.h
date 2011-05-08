/*#ifndef USBFunctions_h
#define USBFunctions_h

// MAX3421E USB Host controller keyboard communication //
#include <Usb.h>
#include <types.h>
#include <HIDTypes.h>

extern unsigned int idVendor;      // Vendor ID (assigned by the USB-IF).
extern unsigned int idProduct;     // Product ID (assigned by the manufacturer).


void startUSB();

// Poll device and print result //
void DEVICE_poll( uint16_t RawValues[], const Model_info& a, USB_device& b );


//raw usage, ReportData = buffer, ReportSize = size of buffer, and HID_ReportInfo_t is output
//void USB_ProcessHIDReport(uint8_t* ReportData, uint16_t ReportSize, device* a)
void USB_ProcessHIDReport(uint8_t* ReportData, uint16_t ReportSize);
void setUSBAxis(Model_info& a,uint8_t channel,uint8_t id,uint8_t axis);

//function to print unknown descriptor //
uint8_t getReportSize( char* descr_ptr);

// Initialize Device 
void DEVICE_init( void );

byte getDevDescr(byte addr );


void USBloop(uint16_t RawValues[], const Model_info& a, USB_device& b);

unsigned int getIdVendor();

unsigned int getIdProduct();

void processUSBinfo(uint16_t RawValues[], const Model_info& a, USB_device& b);
#endif
*/
