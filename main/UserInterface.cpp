#include "UserInterface.h"


LiquidCrystal* UserInterface::setupLCD() {
	LiquidCrystal* lcd = new LiquidCrystal(LCD_PIN_RS, LCD_PIN_RW, LCD_PIN_EN,
#if (LCD_4BITS != (1U))
		LCD_PIN_D0,
		LCD_PIN_D1,
		LCD_PIN_D2,
		LCD_PIN_D3,
#endif
		LCD_PIN_D4,
		LCD_PIN_D5,
		LCD_PIN_D6,
		LCD_PIN_D7
	);

	lcd->begin(LCD_COLS, LCD_ROWS);
	return lcd;
}

UserInterface::UserInterface()
{
	lcd = setupLCD();
}

void UserInterface::ErrorFun() {    //short for Error Function
#if DEBUG_MODE==(1U)
	Serial.println("Error in the inputs");
#endif
	indexKeypad = 0;
	emptyKeypadArray();
}

int UserInterface::getAdditives(char& a_recipeID)
{
	switch (a_recipeID) {
	case '1':
		return RECIPE_1_ADDITIVES;
		break;
	case '2':
		return RECIPE_2_ADDITIVES;
		break;

	case '3':
		return RECIPE_3_ADDITIVES;
		break;

	case '4':
		return RECIPE_4_ADDITIVES;
		break;

	case '5':
		return RECIPE_5_ADDITIVES;
		break;

	}
}

void UserInterface::printRecipeName(char& a_recipeID)
{
	switch (a_recipeID) {
	case '1':
		lcd->print(recipe_1_name);
		break;
	case '2':
		lcd->print(recipe_2_name);
		break;

	case '3':
		lcd->print(recipe_3_name);
		break;

	case '4':
		lcd->print(recipe_4_name);
		break;

	case '5':
		lcd->print(recipe_5_name);
		break;

	}
}

void UserInterface::SaladFun() {
	return;
}

void UserInterface::PastaFun() {
	return;

}

void UserInterface::JuiceFun() {
	return;

}

void UserInterface::FriesFun() {
	return;

}

void UserInterface::BallilaFun() {

	indexKeypad = 0;
	char key;

	//Sugar additive
	lcd->print("Sugar (0-3):");
	while (indexKeypad < 1) {
		char key = keypad.getKey();
		if (key || key < '4' || key >= '0') { // if there is a key pressed not empty charachter
#if DEBUG_MODE==(1u)
			Serial.print("Key pressed: ");
			Serial.println(key);
#endif
			lcd->print("Please choose a recipe.");
			keypadArray[indexKeypad++] = key;
		}
	}
}

void UserInterface::emptyKeypadArray() { //empty the array
	for (int i = 0; i < sizeofInput; i++) {
		keypadArray[i] = 0;
	}
}

void UserInterface::getUserInput() {
	bool flag = false;
	char key;
	while (!flag) {
		char key = keypad.getKey();
		if (key || key<FIRST_RECIPE_NO && key>LAST_RECIPE_NO) { // if there is a key pressed not empty charachter
#if DEBUG_MODE==(1u)
			Serial.print("Key pressed: ");
			Serial.println(key);
#endif
			lcd->print("Please choose a recipe.");
			flag = true;
		}
	}
	lcd->clear();
	lcd->print("Chosen recipe is ");
	printRecipeName(key);
	delay(HOLD_RECIPE_NAME_TIME_DURATION);

	//Call function responsible for handling the additive inputs 
	(*this.*recipesList[key])();
	//If all additives were not inputted by the user, call error function and redo input
	if (indexKeypad != getAdditives(key)) {
		ErrorFun();
	}
	else {
		inputFinished = true;
	}
}