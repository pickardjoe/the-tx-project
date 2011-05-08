/*#include "LCDMenu.h"

void LCDMenu::start() 
{
MAKE_MENU(Root, NULL_ENTRY, NULL_ENTRY, NULL_ENTRY , NULL_ENTRY, NULL_FUNC, NULL_FUNC, "The Tx Project");
  init(5,6,3,4,7);
  //updateMenu();
Serial.print("hey htere");
SET_MENU_WRITE_FUNC(printFunc);
//SET_MENU(Root);//issue ocurring here, system restarting, trying moving more of function from menu into this function
};

void LCDMenu::init(byte left, byte right, byte up, byte down, byte select)
{

  Serial.begin(9600);

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
  lcd1.begin(16, 2);
  lcd1.setCursor(0,0);
  lcd1.clear();
  menuIndex = menuItemIndex = selectionIndex =0;
//  strcpy_P(lcdbuffer, (char*)pgm_read_word(&(MENUS[0][menuIndex]))); 
  lcd1.print("hello there");
  debounceCount = 0;
};


boolean LCDMenu::updateButtons()
{

  Serial.print("got here");
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
      	  //SET_MENU(PARENT);      
      if((buttons & 1<<buttonPins[downPin]) == 0)
      {
          //SET_MENU(CHILD);
          Serial.print("got here");
      }
      if((buttons & 1<<buttonPins[leftPin]) == 0)
          //SET_MENU(PREVIOUS);   
      if((buttons & 1<<buttonPins[rightPin]) == 0)
         // SET_MENU(NEXT);
        buttons = 0;
        return true;
    }
  return false;
  }

};

void LCDMenu::printFunc(const char str[])
{

  lcd1.clear();
  lcd1.print(str);
};
*/
