#define maxItems 25
#define LOBYTE(x) ((char*)(&(x)))[0]
#define HIBYTE(x) ((char*)(&(x)))[1]
#define BUFSIZE 256 //buffer size

#define DEVICE_ADDR  1
#define DEVICE_EP    1
#define EP_MAXPKTSIZE  40
#define EP_POLL        0x0a


#define DATA_SIZE_MASK           0x03
#define TYPE_MASK                0x0C
#define TAG_MASK                 0xF0
#define DATA_SIZE_0              0x00
#define DATA_SIZE_1              0x01
#define DATA_SIZE_2              0x02
#define DATA_SIZE_4              0x03
#define TAG_GLOBAL_REPORTSIZE    0x70
#define TAG_GLOBAL_REPORTCOUNT   0x90
#define TYPE_MAIN                0x00
#define TAG_MAIN_INPUT           0x80
#define TYPE_GLOBAL              0x04
#define IOF_CONSTANT             (1 << 0)
#define TAG_GLOBAL_LOGICALMAX    0x20



//typedef struct{ 
uint8_t offsets[maxItems];
uint8_t bitSizes[maxItems];
uint16_t maxes[maxItems];
uint8_t reportLength;
uint8_t totalItems;
//} device; // this will be defined as a class when I break this file down


///STUFF THAT NEEDS TO BE ADDED TO USB.cpp


uint8_t numberOfItems;

uint8_t i = 0; 

EP_RECORD ep_record[ 2 ];  //endpoint record structure for the keyboard
 
MAX3421E Max;
USB Usb;
bool started = false;
bool initialized = false;
uint8_t address = 0;
uint8_t addDelay = 0;
  char buf[ 50 ] = { 0 };      //keyboard buffer //using 50 just to make sure we have room
// LiquidCrystal display with:
// rs on pin 12
// rw on pin 11
// enable on pin 10
// d4, d5, d6, d7 on pins 5, 4, 3, 2
//LiquidCrystal lcd(8 ,1, 7, 6, 5, 4, 3);//we're not using pin 1 in reality and only feeding that value to the library
