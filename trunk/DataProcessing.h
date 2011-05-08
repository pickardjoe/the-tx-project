#ifndef DataProcessing_h
#define DataProcessing_h
#include <types.h>
#include <USBfunctions.h>
#include "Max_LCD.h"
extern Max_LCD lcd;

uint16_t linearizeData(uint16_t dataIn, Calibration_values v)//returns value between 0 - 32767
{///NEED TO REDO THIS FOR NEW uint8_t values
float temp;
temp = (float)dataIn;

//sanitize data
if(temp < v.bottom)
  temp = v.bottom;
if(temp > v.top)
  temp = v.top;

//linearize data
if(temp < v.center)
  temp = ((temp - v.bottom) / (float)(v.center - v.bottom))* ((float)MAX_VALUE/2.0);//can't use 65535 not enough bits, so cast then mupltiply at end
else
  temp = (1 + ((temp - v.center) / (float)(v.top - v.center)))* ((float)MAX_VALUE/2.0);//can't use 65535 not enough bits, so cast then mupltiply at end

return (uint16_t)temp;
};

void resetModelInfo(Model_info& a)
{
memset(a.modelName,0,sizeof(a.modelName));
memset(a.analog_mapping,0xFF,MAX_CHANNELS);
memset(a.usb_mapping,0xFF,MAX_CHANNELS);
memset(a.expo_values,0,MAX_CHANNELS);
memset(&a.emptySpot,0,1);
memset(a.buffer,0,sizeof(a.buffer));
};

void getAnalogInput(uint16_t RawValues[], const Model_info& a)
{
  uint8_t i;
  uint8_t temp;
  for(i=0;i<MAX_CHANNELS;i++)
    {
      if(i%2)//high nibble
	temp = (a.analog_mapping[i/2] >> 4) &0xF;
      else
	temp = a.analog_mapping[i/2] &0xF;
      if(temp <= MAX_ANALOG_PORT)
      {
        RawValues[i] = (uint16_t)((float)(analogRead(temp)/(float)MAX_ANALOG_VALUE)*MAX_VALUE);
      }
    }
};

void setAnalogMapping(uint8_t analogMapping[], Model_info& a)
{
  uint8_t i;
  uint8_t temp;
  for(i=0;i<MAX_CHANNELS;i++)
  {	
      if(i%2)//low nibble
	a.analog_mapping[i/2] |= (analogMapping[i] << 4);
      else
	a.analog_mapping[i/2] = analogMapping[i] &0xF;
  }
};




void addUSBDevice(Model_info&a,const USB_device& b)//needs to be made to work with flash
{
a.buffer[a.emptySpot] = getUSBInfo;//defined in types
a.emptySpot++;
memcpy(a.buffer + a.emptySpot,&b,sizeof(b));
a.emptySpot += sizeof(b);
};

void addCalibrationValue(Model_info&a,uint8_t axisNumber, const Calibration_values& b)//needs to be made to work with flash
{
a.buffer[a.emptySpot] = calibrateAxis;//defined in types
a.emptySpot++;
a.buffer[a.emptySpot] = axisNumber;
a.emptySpot++;
memcpy(a.buffer + a.emptySpot,&b,sizeof(b));
a.emptySpot += sizeof(b);
};


void processBuffer(uint16_t RawValues[],const Model_info& a)
{
uint8_t bI=0;//buffer index
while(a.buffer[bI] != 0)
{
switch (a.buffer[bI])
{
  
  case getUSBInfo:
    bI++;
    USB_device* b;//use pointer so we don't have to allocate space
    b = (USB_device*)(a.buffer + bI);
    bI += sizeof(USB_device);
    processUSBinfo(RawValues,a,(*b));  
    break;

  case calibrateAxis:
    bI++;
    uint8_t axisNumber;
    Calibration_values* v;
    axisNumber = *(a.buffer + bI);
    bI++;
    v = (Calibration_values*)(a.buffer + bI);
    bI += sizeof(Calibration_values);
    RawValues[axisNumber] = linearizeData(RawValues[axisNumber],*v);
    break;
}
}

};



//amount must be between -100 and 100
//dataIn should be between 0 and MAX_VALUE
uint16_t exponential(uint16_t dataIn, int8_t amount)
{
float x = dataIn;
float B = amount;
float temp;

/*if (B > 100)
  B = 100;
if (B < -100)
  B = -100;

if (dataIn > MAX_VALUE)
  x = MAX_VALUE;*/

//normalize x
x = x / MAX_VALUE;

//normalize B
if (B > 0)//want positive expo
{
  B /= 25;//equation requires number between 0 and 4
temp = 0.5 + (1-0.25*B)*(x-0.5)+B*pow((x-0.5),3);
}
if (B <= 0)//want negative expo
{
  B /= -100;//normalize to o - 1
  B *= 0.9449;//equation requires number between 0 and 0.9449 odd, but keeps function monotonic
temp = 0.5 + (1-B*pow(2.0,(float)(2.0/3.0)))*(x-0.5);
if(x-0.5 < 0.0)
temp += B*-1*pow((float)(x-0.5)*-1,(float)(1.0/3.0));//can't have negative base with pow
else
temp += B*pow((float)(x-0.5),(float)(1.0/3.0));
}

temp = temp * MAX_VALUE;

return (uint16_t) temp;
};
  

void processExpo(uint16_t RawValues[],Model_info& a)
{
uint8_t i;
for(i=0;i<MAX_CHANNELS;i++)
  RawValues[i] = exponential(RawValues[i],a.expo_values[i]);
};

void getInputs(uint16_t RawValues[],Model_info& a)
{
getAnalogInput(RawValues,a);
processBuffer(RawValues,a);
processExpo(RawValues,a);
};


#endif
