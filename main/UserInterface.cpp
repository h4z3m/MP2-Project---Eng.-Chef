#include "UserInterface.h"


LiquidCrystal* UserInterface::setupLCD() {
	LiquidCrystal* lcd = new LiquidCrystal(LCD_PIN_RS, LCD_PIN_RW, LCD_PIN_EN,
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

void UserInterface::printLCD(char* add1Msg, char* add2Msg, char* add3Msg, byte qty1, byte qty2, byte qty3) {
	if (add1Msg) {
		lcd->clear();
		lcd->setCursor(0, 0);
		lcd->print(add1Msg);
		lcd->setCursor(0, 15);
		lcd->print(qty1);
		delay(3000);
	}
	if (add2Msg) {
		lcd->clear();
		lcd->setCursor(0, 0);
		lcd->print(add2Msg);
		lcd->setCursor(0, 15);
		lcd->print(qty2);
		delay(3000);
	}
	if (add3Msg) {
		lcd->clear();
		lcd->setCursor(0, 0);
		lcd->print(add3Msg);
		lcd->setCursor(0, 15);
		lcd->print(qty3);
		delay(3000);
	}
}

void UserInterface::ErrorFun() {    //short for Error Function
	lcd->clear();
	lcd->setCursor(0, 0);
#if DEBUG_MODE==(1U)
	Serial.println("Please Enter a valid input");
#endif
	lcd->print("Invalid input");
	delay(2);
	indexKeypad = 0;
	inputEntered = false;
	//emptyKeypadArray();
}
byte* UserInterface::getAdditivesArray(int numberofAdditives, char** msgs) {
	char key;
	int keyConverted;
	//int numberofAdditives = 3; //arbitrary till deciding the number of additive variables
	byte* additivesArray = new byte[numberofAdditives];
	int iterator = 0;
	bool flag = false;
	while (iterator != numberofAdditives && inputEntered == false) {

		if (!flag) {
			lcd->clear();

			lcd->setCursor(0, 0);
			lcd->print(msgs[iterator]);
			flag = true;
		}
		key = keypad.getKey();
		keyConverted = key - '0';
		if (key) {
			if (keyConverted > 2) {
				ErrorFun();
			}
			else {

				lcd->setCursor(0, 1);
				lcd->print(key);
				flag = false;
				delay(1000);
				additivesArray[iterator++] = keyConverted;
			}
		}
	}
	return additivesArray;
}

void UserInterface::RozBlabn(byte b) {
	if (inputMode == 0) {  //Keypad mode
		
	}
	else if (inputMode == 1) {  //Web mode
	
	}
	return;
}

void UserInterface::ChickenPasta(byte b) {
	int numberofAdditives = 2;
	byte* additivesArray;
	if (inputMode == 0) {  //Keypad mode
		char* msg1 = "Spices Qty(0-2):";
		char* msg2 = "Ex Chicken(0-1):";
		char* msgs[] = { msg1,msg2 };
		additivesArray = getAdditivesArray(numberofAdditives, msgs);
		add.spices = additivesArray[0];
		add.chicken = additivesArray[1];
		delete[] additivesArray;
	}
	else if (inputMode == 1) {  //Web mode
		byte input = b;
		b = input;
		add.spices = b & (0x03);
		add.chicken = (b >> 2) & (0x01);
	}
	return;
}

void UserInterface::MacNCheese(byte b) {
	int numberofAdditives = 2;
	byte* additivesArray;
	if (inputMode == 0) {  //Keypad mode
	// first variable additive is salt
	// second variable additive is pepper
	// third variable additive is Cheddar
		char* msg1 = "Cheese Qty:";
		char* msg2 = "Spices Qty:";
		char* msgs[] = { msg1,msg2 };
		additivesArray = getAdditivesArray(numberofAdditives, msgs);
		add.cheese = additivesArray[0];
		add.spices = additivesArray[1];

		// now call function print lcd
	//	printLCD(msg1,msg2,msg3,qty1,qty2,qty3);
		delete[] additivesArray;
	}
	else if (inputMode == 1) {  //Web mode

		byte input = b;
		add.spices = b & (0x03);
		add.cheese = (b >> 2) & (0x01);

	}
	return;
}

void UserInterface::ChickenMasala(byte b) {
	int numberofAdditives = 2;
	byte* additivesArray;
	if (inputMode == 0) {  //Keypad mode
	// first variable additive is salt
	// second variable additive is pepper
	// third variable additive is Oil
		char* msg1 = "Spices Qty:";
		char* msg2 = "Chicken Qty:";
		char* msgs[] = { msg1,msg2};
		additivesArray = getAdditivesArray(numberofAdditives, msgs);
		add.spices= additivesArray[0];
		add.chicken = additivesArray[1];
		delete[] additivesArray;
	}
	else if (inputMode == 1) {  //Web mode
		byte input = b;
		b = input;
		add.spices = b & (0x03);
		add.chicken = (b >> 2) & (0x01);
	}
	return;
}
void UserInterface::CrispyPotato(byte b) {
	int numberofAdditives = 2;
	byte* additivesArray;
	if (inputMode == 0) {  //Keypad mode
	// first variable additive is salt
	// second variable additive is Chilli
	// third variable additive is Oil
		char* msg1 = "Salt Qty:";
		char* msg2 = "Spices Qty:";
		char* msgs[] = { msg1,msg2 };
		additivesArray = getAdditivesArray(numberofAdditives, msgs);
		add.salt = additivesArray[0];
		add.spices = additivesArray[1];
		delete[] additivesArray;
	}
	else if (inputMode == 1) {  //Web mode
		byte input = b;
		add.salt = (b >> 1) & (0x01);
		add.spices = (b) & (0x01);

	}
	return;
}
void UserInterface::Potato2(byte b) {
	int numberofAdditives = 1;
	byte* additivesArray;
	if (inputMode == 0) {  //Keypad mode
	// first variable additive is salt
	// second variable additive is Cumin
	// third variable additive is Turmeric
		char* msg1 = "Salt Qty:";
		char* msgs[] = { msg1 };
		additivesArray = getAdditivesArray(numberofAdditives, msgs);
		add.salt = additivesArray[0];
		delete[] additivesArray;
	}
	else if (inputMode == 1) {  //Web mode
		byte input = b;
		add.salt = (b >> 1) & 2;
	}
	return;
}

void UserInterface::emptyKeypadArray() { //empty the array
	for (int i = 0; i < sizeofInput; i++) {
		keypadArray[i] = 0;
	}
}

void UserInterface::printRecipeName(int a_recipeID)
{
	lcd->clear();
	lcd->setCursor(0, 0);
	switch (a_recipeID) {
	case 0:
		lcd->setCursor(0, 0);
		lcd->clear();
		lcd->print("Recipe is:");
		lcd->setCursor(0, 1);
		lcd->print(recipe_1_name);
		break;
	case 1:
		lcd->print("Recipe is:");
		lcd->setCursor(0, 1);
		lcd->print(recipe_2_name);
		break;

	case 2:
		lcd->print("Recipe is:");
		lcd->setCursor(0, 1);
		lcd->print(recipe_3_name);
		break;

	case 3:
		lcd->print("Recipe is:");
		lcd->setCursor(0, 1);
		lcd->print(recipe_4_name);
		break;

	case 4:
		lcd->print("Recipe is:");
		lcd->setCursor(0, 1);
		lcd->print(recipe_5_name);
		break;
	case 5:
		lcd->print("Recipe is:");
		lcd->setCursor(0, 1);
		lcd->print(recipe_6_name);
		break;
	}

}

void UserInterface::getUserInput() {
	inputEntered = false;
	int theByte;
	char key;

	while (!inputEntered) {
		key = keypad.getKey();
		int keyConverted = key - '0';
		if (key) { // if there is a key pressed not empty charachter
			if (keyConverted >= FIRST_RECIPE_NO && keyConverted <= LAST_RECIPE_NO) {
				printRecipeName(keyConverted);
				recipe = keyConverted;
				delay(3000);
				//delay(2);
				//inputEntered = true;  //input entered is true till proven otherwise
				inputMode = 0;
				(*this.*recipesList[keyConverted])(12);
				inputEntered = true;
			}
			else {
				ErrorFun();
			}

			//lcd->print("Please choose a recipe.");
		}
		else if (Serial.available()) {
			theByte = Serial.read();
			inputMode = 1;
			decodeFun(theByte);
			inputEntered = true;
		}
	}

}

Recipes UserInterface::decodeFun(char input) {
	char reservedVal = input;
	char newReserved = input;
	recipe = input >> 5;
	reservedVal = reservedVal & (0x1F);
	Serial.println(reservedVal);
	Serial.println(newReserved);
	delay(5);
	(*this.*recipesList[recipe])(reservedVal);
}

Additives* UserInterface::getAdditives() {
	return &add;
}
byte UserInterface::getRecipe() {
	return recipe;
}