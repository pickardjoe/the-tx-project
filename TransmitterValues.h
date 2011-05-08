#ifndef TRANSVALUES_h
#define TRANSVALUES_h

#define ASSIGN_TX_VALUES(name)      \
  frameLength = name ## _frameLength; \
  pulseStart  = name ## _pulseStart;  \
  pulseMin    = name ## _pulseMin;    \
  pulseMax    = name ## _pulseMax;
  

//define TRANSMITTER case values
#define JR_ID 0
#define JR_frameLength 22000
#define JR_pulseStart 380
#define JR_pulseMin 588
#define JR_pulseMax 1610 

#endif