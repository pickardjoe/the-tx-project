// MAX3421E USB Host controller keyboard communication //
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

/*void printValue(unsigned int value)
{
    for (unsigned int mask = 0x80; mask; mask >>= 1) {
        Serial.print(mask&value?'1':'0');
    }
}
void printValueL(unsigned int value)
{
    for (unsigned int mask = 0x8000; mask; mask >>= 1) {
        Serial.print(mask&value?'1':'0');
    }
}*/

float getAxis(uint8_t axisNumber)
{
  float temp;
  if(isSignedAxis(axisNumber))
    temp = (int16_t) axis[axisNumber] - (int16_t) mins[axisNumber];
  else
    temp = axis[axisNumber] - mins[axisNumber];
  
  temp /= (float)(maxes[axisNumber] - mins[axisNumber]);
  
  return temp;
}

uint8_t getButton(uint8_t buttonNumber)
{
  return (totalButtons & (1 << buttonNumber));
}

unsigned long getAllButtons()
{
  return allButtons;
}

bool isSignedAxis(uint8_t axisNumber)
{
 return (axisSign & (1 << axisNumber));
}
    



// Poll device and print result //
void DEVICE_poll()
{
  uint16_t* tempItemPtr;
  uint16_t tempItem;
  uint16_t tempItem2;
  char bufferSize = reportLength/8;
  uint8_t i;
  uint8_t j;
  uint8_t z;
  uint8_t byteMask;
  uint16_t byteMask2;
  uint8_t axisNumber = 0;
  uint8_t buttonNumber = 0;
  byte rcode = 0;     //return code
    // poll keyboard //
    


    rcode = Usb.inTransfer( DEVICE_ADDR, DEVICE_EP, bufferSize, buf,1);//less than 1 ms for mouse ~ 6 ms for joystick//alternate version of this includes parameter for wait
if(rcode == 0)
{
  
   /* for(i=0;i<6;i++)
  {
    	      for (unsigned int mask = 0x80; mask; mask >>= 1) {
        Serial.print(mask&(buf[i])?'1':'0');}
       Serial.print(" ");
  }
  Serial.println();*/
  
allButtons = 0;
    for(i=0;i<totalItems;i++)
    {
        if(bitSizes[i] <= 8)
        {
          j = buf[offsets[i]/8];
          byteMask = (0xFF >> (8 - bitSizes[i])) << (offsets[i]%8);
          j = j & byteMask;
          j = j >> (offsets[i]%8);
	  if(bitSizes[i] == 1)
	  {
	    allButtons |= (j << buttonNumber);

	    buttonNumber++;
	  }
	  else
	  {
	    axis[axisNumber] = j;
	    axisNumber++;
	  }
	  
          //Serial.print(j,DEC);
          //Serial.print(" ");
        }
        else if(bitSizes[i] > 8 && bitSizes[i] <= 16 )
        {
          //tempItemPtr = (uint16_t*)(buf + (offsets[i]/8));
	  tempItem = (uint8_t)(buf[offsets[i]/8 + 1]);
	  tempItem <<= 8;
	  tempItem |= (uint8_t)(buf[offsets[i]/8]);

	      /*for (unsigned int mask = 0x80; mask; mask >>= 1) {
        Serial.print(mask&(buf[offsets[i]/8 + 1])?'1':'0');}*/

	  
	      /*for (unsigned int mask = 0x80; mask; mask >>= 1) {
        Serial.print(mask&(buf[offsets[i]/8])?'1':'0');}*/

          byteMask2 = (0xFFFF >> (16 - bitSizes[i])) << (offsets[i]%8);  //not entirely sure on a.offsets << don't have to take that into account with this joystick
	  
          tempItem = tempItem & byteMask2;
          tempItem = tempItem >> (offsets[i]%8);	  
	  
	  if(mins[axisNumber] > maxes[axisNumber])
	  {
	    if(tempItem & (1 << (bitSizes[i]-1)))//if 2's complement then set bit 16 = to MSB
	    {
	     tempItem |= (0xFFFF<<bitSizes[i]);//set 1's for 2's complement
	    } 
	    //Serial.print((int16_t)tempItem,DEC);

	  }
	  else
	  {
	    //Serial.print(tempItem,DEC);
	  }
	  axis[axisNumber] = tempItem;
	  axisNumber++;
          //Serial.print(" ");
        }

    }
	//Serial.println();
}
else
{
  //Serial.println(rcode,HEX);
}
};

uint8_t getTotalAxis()
{
return totalAxis;
}

uint8_t getTotalButtons()
{
return totalButtons;
}



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
	uint16_t tempMin;
	uint16_t tempMax;
	
	bool axis = false;
	bool button = false;
	axisSign = 0;

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
			case (TYPE_GLOBAL | TAG_GLOBAL_LOGICALMIN):
				tempMin = ReportItemData;
				break;				
			case (TYPE_GLOBAL | TAG_GLOBAL_LOGICALMAX):                                
				tempMax = ReportItemData;
				break;
			case (TYPE_GLOBAL | TAG_GLOBAL_USAGEPAGE):
				if(ReportItemData == 0x09)
				  button = true;				
				break;
			case (TYPE_LOCAL | TAG_LOCAL_USAGE):
				if (ReportItemData >= 0x30 && ReportItemData <= 0x39)
				  axis = true;
				break;
			case (TYPE_MAIN | TAG_MAIN_INPUT):
                                //if(currentBitSize != 0 && currentReportCount == 0)
                                  //currentReportCount++;//in case reportcount not reported, default is 1 report

				temp = (ReportItemData & IOF_CONSTANT);


                                if(axis || button)//only interested in variable items
                                {
				  //Serial.println(currentReportCount,DEC);
                                  for(temp =0;temp<currentReportCount;temp++)
                                  {   
                                    offsets[i] = totalBits;
                                    bitSizes[i] = currentBitSize;
                                    totalBits += currentBitSize;
				    
				    if(axis)
				    {
				      
				      if(tempMin > tempMax)
				      {
					axisSign |= (1 << totalAxis);
				      }
				      maxes[totalAxis] = tempMax;
				      mins[totalAxis] = tempMin;
				      totalAxis++;
				    }
				    if(button)
				      totalButtons++;
                                    i++;
                                  }
                                  axis = false;
				  button = false;
                                  //currentBitSize = currentReportCount = 0;//reset both to 0 for next item
                                }
                                else//we are padding if we encounter a constant item, this is an assumption, but makes things easier
				{
				  totalBits += currentBitSize*currentReportCount;
				}
                                 // totalBits = ((totalBits / 8) * 8) + 8;//add bits to round up to next byte;

                  		break;
		}
	}
        reportLength = totalBits;
        totalItems = i;//-1 because we start at 0
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


void USBloop()
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
        DEVICE_poll();
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

void processUSBinfo()
{
uint8_t i;
if(!started)
  startUSB();
USBloop();
};