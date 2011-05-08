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

extern const Menu_Item *current;
extern const Menu_Item *prev;
extern const Menu_Item *lastChild;

extern char LCDBuffer[17];
extern Max_LCD lcd;

extern byte buttonPins[totalButtons];
extern byte buttons;
extern byte oldButtons;
extern byte bitMask;

extern byte debounceCount;
extern byte menuItemIndex;
extern byte selectionIndex;




void displayItem();
void initializeMenu();
void menuChange();
void menuUp();
void menuDown();
void menuLeft();
void menuRight();//assume at the beginning of each function, prev and current point to same thing
void init(byte left, byte right, byte up, byte down, byte select);
boolean updateButtons();
#endif
