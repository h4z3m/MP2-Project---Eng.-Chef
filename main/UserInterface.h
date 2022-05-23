// UserInterface.h

#ifndef _USERINTERFACE_h
#define _USERINTERFACE_h



//#include "arduino.h"

///////// LCD /////////////////////
#include <LiquidCrystal.h>

#define LCD_ROWS (2)
#define LCD_COLS (16)
#define LCD_4BITS (0U)
#define LCD_PIN_RS (3)
#define LCD_PIN_RW (2)
#define LCD_PIN_EN (2)

#define LCD_PIN_D0 (5)
#define LCD_PIN_D1 (6)
#define LCD_PIN_D2 (7)
#define LCD_PIN_D3 (8)

#define LCD_PIN_D4 (8)
#define LCD_PIN_D5 (9)
#define LCD_PIN_D6 (10)
#define LCD_PIN_D7 (11)
//////////////////////////////////
//////////keyPad //////////////////
#include <Keypad.h>
//Columns
#define cols (4)
#define keyPadpinC1 (6)  // C1 stands for column 1
#define keyPadpinC2 (7)
#define keyPadpinC3 (12)
#define keyPadpinC4 (13)
//Rows
#define rows (2)
#define keyPadpinR1 (4)
#define keyPadpinR2 (5)

#define sizeofInput (8)
///////////////////////////////////////////////

/////////// Recipes ///////////////////
#define HOLD_RECIPE_NAME_TIME_DURATION (2000U)
#define recipes (5)
#define FIRST_RECIPE_NO (0)
#define LAST_RECIPE_NO (5)

#define recipe_1_name ("MacNCheese")
#define recipe_2_name ("ChickenPasta")
#define recipe_3_name ("RozBlabn")
#define recipe_4_name ("ChickenMasala")
#define recipe_5_name ("CrispyPotato")
#define recipe_6_name ("Potato2")

// #define RECIPE_2_ADDITIVES (4u)
// #define RECIPE_3_ADDITIVES (2u)
// #define RECIPE_4_ADDITIVES (2u)
// #define RECIPE_5_ADDITIVES (3u)

/////////////////////////////////////
enum Recipes {
	MacNCheese = 1,
	ChickenPasta = 2,
	RicePudding = 3,
	Jelly = 3,
	Masala = 4,
	CrispyPotato = 5

};
typedef struct Additives {  //additives that are variables
	/*byte salt;
	byte pepper;
	byte Cheddar;
	byte sugar;
	byte Oil;
	byte Chilli;
	byte milk;
	byte vanilla;
	byte cumin;
	byte turmeric;*/
	byte spices;
	byte salt;
	byte chicken;
	byte cheese;
	byte onions;
};
//int inputFinished = 0;


class UserInterface {

private:
	char indexKeypad = 0;
	char keypadArray[sizeofInput];
	char keymaps[rows][cols] = { {'0', '1', '2', '3'},
								 {'4', '5', '6', '7'} };
	byte pin_rows[rows] = { keyPadpinR1, keyPadpinR2 };
	byte pin_columns[cols] = { keyPadpinC1,keyPadpinC2,keyPadpinC3,keyPadpinC4 };

	Keypad keypad = Keypad(makeKeymap(keymaps), pin_rows, pin_columns, rows, cols);
	LiquidCrystal* setupLCD();

	//void getAdditives(char& a_key);
	//int getAdditives(byte& a_recipeID);

	void emptyKeypadArray();
	void printRecipeName(int a_recipeID);
	void ErrorFun();
	void MacNCheese(byte);
	void ChickenPasta(byte);
	void RozBlabn(byte);
	void ChickenMasala(byte);
	void CrispyPotato(byte);
	void Potato2(byte b);
	void (UserInterface::* recipesList[7])(byte input) = {
	&MacNCheese,
	&MacNCheese,
	&ChickenPasta,
	&RozBlabn,
	&ChickenMasala,
	&CrispyPotato,
	&Potato2,
	};
	Additives add;
	byte recipe;
	int inputMode; //input mode is 0 for keyinput and 1 for serial web
	int inputEntered;
	Recipes decodeFun(char x);
	void printLCD(char* add1Msg, char* add2Msg, char* add3Msg, byte qty1, byte qty2, byte qty3);
public:
	UserInterface();
	LiquidCrystal* lcd;
	void getUserInput();
	Additives* getAdditives();
	byte* getAdditivesArray(int numberofAdditives, char** msgs);
	byte getRecipe();
};
#endif
