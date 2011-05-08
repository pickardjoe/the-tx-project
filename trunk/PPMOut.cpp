#include "PPMOut.h"
#include "TransmitterValues.h"
#include "Max_LCD.h"
extern Max_LCD lcd;

volatile uint8_t j_ISR,p_ISR;//i is general purpose, p is used for pause buffer counter, s used for receive buffer,j i used in interrupt
volatile uint16_t pauses[TOTAL_PAUSES];
volatile uint16_t pauses_buffer[TOTAL_PAUSES];
volatile uint16_t* pauses_p;
volatile uint16_t* buffer_p;
volatile uint16_t* temp;
volatile bool ready;
volatile uint32_t milliseconds;

//Timer1 overflow interrupt vector handler
//18.4 useconds without microsecond add
ISR(TIMER1_OVF_vect) {
  //digitalWrite(TOGGLE_IO,!digitalRead(TOGGLE_IO));
  
  PIND |= _BV(2);
    TCNT1 = pauses_p[p_ISR];
  //microseconds = microseconds % 1000;
   p_ISR= (p_ISR+1) % TOTAL_PAUSES;
/*if(p_ISR== TOTAL_PAUSES-1)//adding ready == true fixed issues with malformed packets,
{
   temp = buffer_p;
   buffer_p = pauses_p;
   pauses_p = temp;
}*/

   if(p_ISR== TOTAL_PAUSES-1 && ready)//adding ready == true fixed issues with malformed packets,
   {
//PORTD |= _BV(2);
   for(j_ISR=0;j_ISR<TOTAL_PAUSES;j_ISR++)             //ideally want way to swith over as soon as it is ready
   {
     pauses[j_ISR] = pauses_buffer[j_ISR];         //however it is working with just using this
                                           //makes it seem like it would miss a packet, but response
                                           //time seems to be instantaneous, alternative would be pointer switch
    }
//PORTD &= ~_BV(2);
    }
//   if(p_ISR== TOTAL_PAUSES-1 && !ready)//adding ready == true fixed issues with malformed packets,
//	lcd.clear();

};


 void PPMOut::loadTXValues(uint16_t frameLengthV,uint16_t pulseStartV,uint16_t pulseMinV,uint16_t pulseMaxV)
{
frameLength = frameLengthV;
pulseStart = pulseStartV;
pulseMin   = pulseMinV;
pulseMax   = pulseMaxV;
//pauses_pointer = buffer_p;
};

void PPMOut::loadTXValues(uint8_t transmitterType)
{
  switch(transmitterType)
  {
    case JR_ID:
      ASSIGN_TX_VALUES(JR);//macro in transmittervalues.h
    break;
  }
}

void PPMOut::loadChannels(const uint16_t rawValues[],uint16_t maxValue)
{
float tempFloat;
uint16_t totalPulseTime;//used in calculating sync period so that frame = frameLength
uint8_t i;
totalPulseTime = 0;
  ready = false;
  for ( i = 0; i < MAX_CHANNELS; i = i + 1 )
  {
    tempFloat = (pulseMax - pulseMin) * (rawValues[i] / (float)maxValue) + pulseMin;//range * value / maxResolution + pulsemin
    pauses_buffer[2*i+1] = (uint16_t)(tempFloat);
    pauses_buffer[2*i] = pulseStart;
  
    totalPulseTime += pauses_buffer[2*i+1] + pulseStart;
  }

  pauses_buffer[2*MAX_CHANNELS] = pulseStart;
  pauses_buffer[TOTAL_PAUSES-1] = frameLength - totalPulseTime;
 
  //NOW set values equivalent to appropriate number of 0.5 usecs before timer will go off.
 
  for(i=0;i<TOTAL_PAUSES;i++)
  {
//    Serial.print(pauses_buffer[i],DEC);
//    Serial.print(' ');
    pauses_buffer[i] = 65535-(pauses_buffer[i]*2);//TCNT1 produces overflow when it reaches 65535 and each tick is equivalent to 0.5 usecs
  }
//Serial.println();
  ready = true;
}

void PPMOut::start()
{
  pinMode(TOGGLE_IO,OUTPUT);
  digitalWrite(TOGGLE_IO,LOW);
  //Timer1 is 16bit
//TIMSK0&=!(1<<TOIE0);//disable timer0, this disables millis to make sure it doesn't interfere with timing. Serial doesn't work


  //Timer1 Settings: Timer Prescaler /8, mode 0
  //Timer clock = 16MHz/8 = 2Mhz or 0.5us
  //The /8 prescale gives us a good range to work with
  //so we just hard code this for now.
  TCCR1A = 0;
  TCCR1B = 0<<CS12 | 1<<CS11 | 0<<CS10;
  //Timer1 Overflow Interrupt Enable
  TIMSK1 = 1<<TOIE1;
  //load the timer for its first cycle
  p_ISR=0;//p will be used as index for pauses
  milliseconds = 0;
  buffer_p = pauses_buffer;
  pauses_p = pauses;
  TCNT1=65500;//have interrupt go off almost immediately*/
}






