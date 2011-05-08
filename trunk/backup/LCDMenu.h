#ifndef LCDMenu_h
#define LCDMenu_h
#include <types.h>
#include <Usb.h>
///DIRECTORY STRUCTURE///

/*				                                ROOT
							      /	    \
						    properties      main menu
							/                \
model selection, modelname, input information, input mapping		Expo,Reversing,subtrim,travel adjustment,mixers,output
*/

#define totalButtons 5
#define leftPin   0
#define rightPin  1
#define downPin   2
#define upPin     3
#define selectPin 4
#define debounceDepth 4


//level 1
extern const Menu_Item root;

//level 2
extern const Menu_Item properties;//left side of tree
extern const Menu_Item main_menu;//right side of tree

//level 3 -properties
extern const Menu_Item model_selection;
extern const Menu_Item model_name;
extern const Menu_Item input_information;
extern const Menu_Item input_mapping;

//level 3 - main_menu
extern const Menu_Item expo;
extern const Menu_Item reversing;
extern const Menu_Item subtrim;
extern const Menu_Item travel_adjustment;
extern const Menu_Item mixers;
extern const Menu_Item output;

//level 1
const Menu_Item root PROGMEM = 			{NULL,			NULL,			NULL,&properties,NULL,"The Tx Project"};

//level 2
const Menu_Item properties PROGMEM = 		{&main_menu,		&main_menu,		&root,&model_selection,NULL,"Properties"};
const Menu_Item main_menu PROGMEM = 		{&properties,		&properties,		&root,&expo,NULL,"Main Menu"};

//level 3 -properties
const Menu_Item model_selection PROGMEM = 	{&model_name,		&input_mapping,		&properties,NULL,NULL,"Model Selection"};
const Menu_Item model_name PROGMEM = 		{&input_information,	&model_selection,	&properties,NULL,NULL,"Model Name"};
const Menu_Item input_information PROGMEM = 	{&input_mapping,	&model_name,		&properties,NULL,NULL,"Input Info"};
const Menu_Item input_mapping PROGMEM = 	{&model_selection,	&input_information,	&properties,NULL,NULL,"Input Mapping"};


//level 3 -main menu
const Menu_Item expo PROGMEM =              	{&reversing,		&output,		&main_menu,NULL,NULL,"Expo"};
const Menu_Item reversing PROGMEM =         	{&subtrim,		&expo,			&main_menu,NULL,NULL,"Reversing"};
const Menu_Item subtrim PROGMEM =           	{&travel_adjustment,	&reversing,		&main_menu,NULL,NULL,"Subtrim"};
const Menu_Item travel_adjustment PROGMEM = 	{&mixers,		&subtrim,		&main_menu,NULL,NULL,"Travel Adjust"};
const Menu_Item mixers PROGMEM =            	{&output,		&travel_adjustment,	&main_menu,NULL,NULL,"Mixers"};
const Menu_Item output PROGMEM =            	{&expo,			&mixers,		&main_menu,NULL,NULL,"Output"};


const Menu_Item *current;
const Menu_Item *prev;
const Menu_Item *lastChild;

char LCDBuffer[17];
Max_LCD lcd;

byte buttonPins[totalButtons];
byte buttons;
byte oldButtons;
byte bitMask;

byte debounceCount;
byte menuItemIndex;
byte selectionIndex;




void displayItem()
{
  strcpy_P(LCDBuffer,(const prog_char*)&current->name);
  //Serial.print(LCDBuffer);
  lcd.clear();
  lcd.print(LCDBuffer);
}

void initializeMenu()
{
  current = &root;
  prev = &root;

  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.clear();

if(current)
  displayItem();
}

void menuChange()
{
if(current)//current != null
{
  prev = current;//set them equal
  displayItem();
}
else
  current = prev;//can't move anymore
}

void menuUp()
{
  lastChild = current;
  current = (Menu_Item*)pgm_read_word(&current->Parent);
  menuChange();
}

void menuDown()
{
  if(lastChild != NULL && (current == (Menu_Item*)pgm_read_word(&lastChild->Parent)))//make sure parent menu hasn't changed
    current = lastChild;
  else
    current = (Menu_Item*)pgm_read_word(&current->CHILD);
  menuChange();
}


void menuLeft()
{
  current = (Menu_Item*)pgm_read_word(&current->Previous);
  menuChange();
}

void menuRight()//assume at the beginning of each function, prev and current point to same thing
{
  current = (Menu_Item*)pgm_read_word(&current->Next);
  menuChange();
};


void init(byte left, byte right, byte up, byte down, byte select)
{
  buttons =    0x00;
  oldButtons = 0x00;
  bitMask    = 0x00;
   byte i;
   buttonPins[leftPin]   = left;
   buttonPins[rightPin]  = right;
   buttonPins[downPin]   = down;
   buttonPins[upPin]     = up;
   buttonPins[selectPin] = select;
   for(i=0;i<totalButtons;i++)
   {
     bitMask |= 1<<buttonPins[i];
     pinMode(buttonPins[i],INPUT);
     digitalWrite(buttonPins[i],HIGH);
   }

};


boolean updateButtons()
{
  if(debounceCount != debounceDepth)
  {
    buttons |= (PIND | ~bitMask);//use bitmask so we are only concerned about buttons we are checking, as pins in PIND maybe used for other things such as Serial
    debounceCount++;            //use OR and negate because we are using negative logic and a 0 in PIND is button pushed and anded with a 1 would erase it
    return false;
  }
  else
  {
    oldButtons = buttons;
    buttons = (PIND | ~bitMask);
    if(oldButtons == 0xFF && oldButtons != buttons)//means if last button check registered a button press and new button press is different, meaning button released
    {
      debounceCount = 0;
//      pgm_read_byte_near( menu_length + i);
      //seems convoluted but saves space and does all pins at once
      if((buttons & 1<<buttonPins[upPin]) == 0)//should think about negating buttons later to make this easier to work with
      	  menuUp();      
      if((buttons & 1<<buttonPins[downPin]) == 0)
          menuDown();
      if((buttons & 1<<buttonPins[leftPin]) == 0)
          menuLeft();
      if((buttons & 1<<buttonPins[rightPin]) == 0)
          menuRight();
        buttons = 0;
        return true;
    }
  return false;
  }
};





//WORKING CODE!!!!
/*  
   //function pointer
    FuncPtr FPtr;
   char buffer[17];   
   printf("ROOT");
   while(current)
   {
     prev = current;
     strcpy_P(buffer,(const prog_char*)&current->name);
     Serial.print(buffer);
      current = (Menu_Item*)pgm_read_word(&current->Next);
   }
   current = prev;
   FPtr = (FuncPtr)pgm_read_word(&current->SelectFunc);
   FPtr();
*/
#endif