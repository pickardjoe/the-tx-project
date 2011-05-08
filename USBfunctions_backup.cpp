/*// MAX3421E USB Host controller keyboard communication //
#include "USBfunctions.h"
unsigned int idVendor;      // Vendor ID (assigned by the USB-IF).
unsigned int idProduct;     // Product ID (assigned by the manufacturer).

#include <HIDTypes.h>


void startUSB()
{
if(!started)
{
  Max.powerOn();
  started = true;
}
};

// Poll device and print result //
void DEVICE_poll( uint16_t RawValues[], const Model_info& a, USB_device& b )
{
  uint16_t* tempItemPtr;
  char bufferSize = reportLength/8;
  uint8_t i;
  uint8_t j;
  uint8_t z;
  uint8_t byteMask;
  uint16_t byteMask2;
  uint8_t axisNumber = 0;
  uint8_t buttonNumber = 0;
  uint16_t tempAxis;
  byte rcode = 0;     //return code
    // poll keyboard //

    rcode = Usb.inTransfer( DEVICE_ADDR, DEVICE_EP, bufferSize, buf,1);//less than 1 ms for mouse ~ 6 ms for joystick//alternate version of this includes parameter for wait
if(rcode == 0)
{
    for(i=0;i<totalItems;i++)
    {
        if(bitSizes[i] <= 8)
        {
          j = buf[offsets[i]/8];
          byteMask = (0xFF >> (8 - bitSizes[i])) << (offsets[i]%8);
          j = j & byteMask;
          j = j >> (offsets[i]%8);
          Serial.print(j,DEC);
          Serial.print(' ');
	  if(maxes[i] > 1)
	    tempAxis = j;
        }
        else if(bitSizes[i] > 8 && bitSizes[i] <= 16 )
        {
          tempItemPtr = (uint16_t*)(buf + (offsets[i]/8));
          byteMask2 = (0xFFFF >> (16 - bitSizes[i])) << (offsets[i]%8);  //not entirely sure on a.offsets << don't have to take that into account with this joystick
          *tempItemPtr = *tempItemPtr & byteMask2;
          *tempItemPtr = *tempItemPtr >> (offsets[i]%8);
          Serial.print((uint16_t)*tempItemPtr,DEC);
          Serial.print(' ');
	    tempAxis = (uint16_t)*tempItemPtr;
        }


	if(maxes[i] > 1)
	{
	  for(z=0;z<MAX_CHANNELS;z++)
	  {
	    //If we have an axis, check through usb mapping to see if any channel needs input from this axis
	    if(((a.usb_mapping[z]>>4) &0xF) == b.id)//high nibble
	      if((a.usb_mapping[z] &0xF) == axisNumber)
	      {
		RawValues[z] = (uint16_t)((float)(tempAxis/(float)maxes[i])*MAX_VALUE);
	      }
	  }
	  axisNumber++;
	}

    }
	Serial.println();
	//Serial.println(axisNumber,DEC);
	//Serial.println(buttonNumber,DEC);
}
else
{
  Serial.println(rcode,HEX);
}
};


//raw usage, ReportData = buffer, ReportSize = size of buffer, and HID_ReportInfo_t is output
//void USB_ProcessHIDReport(uint8_t* ReportData, uint16_t ReportSize, device* a)
void USB_ProcessHIDReport(uint8_t* ReportData, uint16_t ReportSize)
{
//  Serial.println( freeMemory() );
        uint8_t currentBitSize = 0;
        uint8_t currentReportCount = 0;
        uint32_t totalBits = 0;
        uint8_t i;
        uint8_t temp;
        uint16_t lastMax;

        i = 0;//index for items
	while (ReportSize)
	{
		uint8_t  HIDReportItem  = *ReportData;
		uint32_t ReportItemData = 0;
		
		ReportData++;
		ReportSize--;

		switch (HIDReportItem & DATA_SIZE_MASK)
		{
			case DATA_SIZE_4:
//                                printProgStr(DATA_SIZE_4_str);
				ReportItemData  = *((uint32_t*)ReportData);
				ReportSize     -= 4;
				ReportData     += 4;
				break;
			case DATA_SIZE_2:
//                              printProgStr(DATA_SIZE_2_str);
				ReportItemData  = *((uint16_t*)ReportData);
				ReportSize     -= 2;
				ReportData     += 2;
				break;
			case DATA_SIZE_1:
//                              printProgStr(DATA_SIZE_1_str);
				ReportItemData  = *((uint8_t*)ReportData);
				ReportSize     -= 1;
				ReportData     += 1;
				break;
		}

		switch (HIDReportItem & (TYPE_MASK | TAG_MASK))
		{
			case (TYPE_GLOBAL | TAG_GLOBAL_REPORTSIZE):
                                currentBitSize = ReportItemData;
				break;
			case (TYPE_GLOBAL | TAG_GLOBAL_REPORTCOUNT):
				currentReportCount = ReportItemData;
				break;
			case (TYPE_GLOBAL | TAG_GLOBAL_LOGICALMAX):                                
				maxes[i] = ReportItemData;
				break;
			case (TYPE_MAIN | TAG_MAIN_INPUT):
                                if(currentBitSize != 0 && currentReportCount == 0)
                                  currentReportCount++;//in case reportcount not reported, default is 1 report
                                
                                temp = (ReportItemData & IOF_CONSTANT);
                                 
                                if(temp != IOF_CONSTANT)//only interested in variable items
                                {
                                  for(temp =0;temp<currentReportCount;temp++)
                                  {
                                    offsets[i] = totalBits;
                                    bitSizes[i] = currentBitSize;
                                    totalBits += currentBitSize;
                                    i++;
                                  }
                                  currentBitSize = currentReportCount = 0;//reset both to 0 for next item
                                }
                                else//we are padding if we encounter a constant item, this is an assumption, but makes things easier
                                  totalBits = ((totalBits / 8) * 8) + 8;//add bits to round up to next byte;
                  		break;
		}
	}
        reportLength = totalBits;
        totalItems = i;//-1 because we start at 0
        lastMax = maxes[0];//assume we have at least one item
        for(i=1;i<totalItems;i++)//go through and fill in spots for maxes
        {
          if(maxes[i] == 0)
            maxes[i] = lastMax;
          else
            lastMax = maxes[i];
        }            
};

void setUSBAxis(Model_info& a,uint8_t channel,uint8_t id,uint8_t axis)
{
	a.usb_mapping[channel] = axis &0xF;
	a.usb_mapping[channel] |= (id << 4);
};

//function to print unknown descriptor //
uint8_t getReportSize( char* descr_ptr)
{
  char buf[ BUFSIZE ];
  unsigned short length;
  byte i;
  byte rcode;
  uint8_t report_length;
  descr_ptr+= 7;
  length = (unsigned short)*descr_ptr;

  rcode = Usb.getReportDescr( 1, 0, length, buf); //get the whole descriptor
 //ADDED THIS PART AND IT STARTED TO WORK
  if( rcode ) {
        Serial.print("Error communicating with device. Return code :");
        Serial.println( rcode, HEX );
    }
  USB_ProcessHIDReport((uint8_t*) buf,length);
//  Serial.print(reportLength,DEC);//in bits
//  Serial.print(' ');
//  Serial.println(totalItems,DEC);
  return reportLength;
};

// Initialize Device 
void DEVICE_init( void )
{
  byte rcode = 0;  //return code
  byte conf =  0;
  char buf[ BUFSIZE ];
  char* buf_ptr = buf;
  unsigned int total_length;
  byte report_size;
  
  byte descr_length;
  byte descr_type;
    // Initialize data structures 
    ep_record[ 0 ] = *( Usb.getDevTableEntry( 0,0 ));  //copy endpoint 0 parameters
    ep_record[ 1 ].MaxPktSize = EP_MAXPKTSIZE;
    ep_record[ 1 ].Interval  = EP_POLL;
    ep_record[ 1 ].sndToggle = bmSNDTOG0;
    ep_record[ 1 ].rcvToggle = bmRCVTOG0;
    
    Usb.setDevTableEntry( 1, ep_record );              //plug DEVICE.endpoint parameters to devtable
    // Configure device //
    rcode = Usb.setConf( DEVICE_ADDR, 0, 1 );                    
    if( rcode ) {
        Serial.print("Error attempting to configure keyboard. Return code :");
        Serial.println( rcode, HEX );
        while(1);  //stop
    }
      rcode = Usb.getConfDescr( DEVICE_ADDR, 0, 4, conf, buf ); //get total length
      LOBYTE( total_length ) = buf[ 2 ];
      HIBYTE( total_length ) = buf[ 3 ];
      rcode = Usb.getConfDescr( DEVICE_ADDR, 0, total_length, conf, buf ); //get the whole descriptor 


  while( buf_ptr < buf + total_length )
  { //parsing descriptors
    descr_length = *( buf_ptr );
    descr_type = *( buf_ptr + 1 );
    switch( descr_type ) {
      case( USB_DESCRIPTOR_CONFIGURATION ):
        break;
      case( USB_DESCRIPTOR_INTERFACE ):
        break;
      case( USB_DESCRIPTOR_ENDPOINT ):
        break;
      default:
        report_size = getReportSize(buf_ptr);//for report DESCRIPTOR
        break;
        }//switch( descr_type
    
    buf_ptr = ( buf_ptr + descr_length ); //advance buffer pointer
  }      
};

byte getDevDescr(byte addr )
{
  USB_DEVICE_DESCRIPTOR buf;
  byte rcode;
  rcode = Usb.getDevDescr( addr, 0, 0x12, ( char *)&buf );
  if( rcode ) {
    return( rcode );
  }
  idVendor = buf.idVendor;
  idProduct = buf.idProduct;
    return( 0 );
};


void USBloop(uint16_t RawValues[], const Model_info& a, USB_device& b)
{
    Max.Task();
    Usb.Task();
byte temp = Usb.getUsbTaskState();
if(temp == USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE)
return;//need to decide what to do if device is not connected.
while(temp != USB_STATE_CONFIGURING && temp != USB_STATE_RUNNING)
{
delayMicroseconds(20000);//give device time to settle
Max.Task();
Usb.Task();
temp = Usb.getUsbTaskState();
}
    if( Usb.getUsbTaskState() == USB_STATE_CONFIGURING ) {  //wait for addressing state
        DEVICE_init();
	getDevDescr(1);//using address 1 for the moment
        Usb.setUsbTaskState( USB_STATE_RUNNING );
    }
    if( Usb.getUsbTaskState() == USB_STATE_RUNNING ) {  //poll the device
    //    lcd.print("Main menu");
        DEVICE_poll(RawValues,a,b);
    }
};




unsigned int getIdVendor()
{
  if(Usb.getUsbTaskState() == USB_STATE_RUNNING)
    return idVendor;
  else 
    return 0;
};


unsigned int getIdProduct()
{
  if(Usb.getUsbTaskState() == USB_STATE_RUNNING)
    return idProduct;
  else 
    return 0;
};

void processUSBinfo(uint16_t RawValues[], const Model_info& a, USB_device& b)
{
uint8_t i;
if(!started)
  startUSB();
USBloop(RawValues,a,b);
};*/