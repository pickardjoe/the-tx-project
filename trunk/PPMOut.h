#ifndef PPMOUT_h
#define PPMOUT_h
#include <types.h>
#define TOGGLE_IO        2  //Arduino pin to toggle in timer ISR

#define TOTAL_PAUSES ((MAX_CHANNELS*2)+2)

class PPMOut
{
public:
void loadChannels(const uint16_t rawValues[],uint16_t maxValue);
void loadTXValues(uint16_t frameLength,uint16_t pulseStart,uint16_t pulseMin,uint16_t pulseMax);
void loadTXValues(uint8_t transmitterType);
void start();
private:
//void printDouble( double val, byte precision);
uint16_t frameLength;//JR framelength = 22000 usecs
uint16_t pulseStart;
uint16_t pulseMin;//724; // pulse minimum width minus start in microseconds
uint16_t pulseMax;//2048; // pulse maximum width in microseconds
};


#endif