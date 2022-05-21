// UserInterface.h

#ifndef _USERINTERFACE_h
#define _USERINTERFACE_h



#include "arduino.h"
#include <Keypad.h>
#include <LiquidCrystal.h>

#define LCD_ROWS (2)
#define LCD_COLS (16)
#define LCD_4BITS (0U)
#define LCD_PIN_RS (2)
#define LCD_PIN_RW (3)
#define LCD_PIN_EN (4)

#define LCD_PIN_D0 (5)
#define LCD_PIN_D1 (6)
#define LCD_PIN_D2 (7)
#define LCD_PIN_D3 (8)
#define LCD_PIN_D4 (9)
#define LCD_PIN_D5 (10)
#define LCD_PIN_D6 (11)
#define LCD_PIN_D7 (12)

//Columns
#define cols (4u)
#define keyPadpinC1 (7u)  // C1 stands for column 1
#define keyPadpinC2 (6u)
#define keyPadpinC3 (5u)
#define keyPadpinC4 (4u)
//Rows
#define rows (4u)
#define keyPadpinR1 (11u)
#define keyPadpinR2 (10u)
#define keyPadpinR3 (9u)
#define keyPadpinR4 (8u)
#define sizeofInput (8u)

#define HOLD_RECIPE_NAME_TIME_DURATION (2000U)
#define recipes (5u)
#define FIRST_RECIPE_NO ('1')
#define LAST_RECIPE_NO ('5')
#define ballila (1u)
#define salad (2u)
#define fries (3u)
#define pasta (4u)
#define juice (5u)
#define recipe_1_name ("Ballila")
#define recipe_2_name ("Salad")
#define recipe_3_name ("Fries")
#define recipe_4_name ("Pasta")
#define recipe_5_name ("Juice")
#define RECIPE_1_ADDITIVES (1u)
#define RECIPE_2_ADDITIVES (4u)
#define RECIPE_3_ADDITIVES (2u)
#define RECIPE_4_ADDITIVES (2u)
#define RECIPE_5_ADDITIVES (3u)


class UserInterface {

private:
	char indexKeypad = 0;
	char keypadArray[sizeofInput];
	char keymaps[rows][cols] = { {'1', '2', '3', 'A'},
								 {'4', '5', '6', 'B'},
								 {'7', '8', '9', 'C',},
								 {'*', '0', '#', 'D'} };
	byte pin_rows[rows] = { keyPadpinR1, keyPadpinR2,keyPadpinR3,keyPadpinR4 };
	byte pin_columns[cols] = { keyPadpinC1,keyPadpinC2,keyPadpinC3,keyPadpinC4 };

	Keypad keypad = Keypad(makeKeymap(keymaps), pin_rows, pin_columns, rows, cols);
	LiquidCrystal* setupLCD();
	void (UserInterface::* recipesList[recipes + 1])() = {
		&ErrorFun,
		&BallilaFun,
		&SaladFun,
		&JuiceFun,
		&FriesFun,
		&PastaFun
	};

	//void getAdditives(char& a_key);
	int getAdditives(char& a_recipeID);

	void emptyKeypadArray();
	void printRecipeName(char& a_recipeID);
	void ErrorFun();
	void BallilaFun();
	void SaladFun();
	void JuiceFun();
	void FriesFun();
	void PastaFun();

public:

	bool inputFinished = false;
	LiquidCrystal* lcd;
	UserInterface();
	void getUserInput();

};
#endif

