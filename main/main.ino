// 
// TODO: Calibrate pumps duration for each liquid
// 

//Libraries
#include <Key.h>
#include <Keypad.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include <Stepper.h>
//Classes
#include "dcMotor.h"
#include "stepperMotor.h" 
#include "stove.h"
#include "UserInterface.h"
#define DEBUG_MODE (1u)
/********************************** Global variables **********************************/
//Slider stepper motor config
struct Motor_configType conf1 = { 6,9,0,0,0,10800.0f,700 };
//Rotation stepper config
struct Motor_configType conf2 = { 6,9,0,0,0,360,1000 };
//Container servo motor config
//struct Motor_configType conf3 = { 3,3,0,0,0,360,0.2f };
//Arm motor stepper config
struct Motor_configType conf4 = { 11,12,0,0,0,360,800 };
//Cover stepper motor
struct Motor_configType conf5 = { 8,7,0,0,0,360,800 };
//Stirring DC motor config
struct Motor_configType conf6 = { 31,26,0,0,0,360,120 };

/*********************************** Definitions ************************************/
#define relPin 2
#define WATER_PUMP_PIN 31
#define MILK_PUMP_PIN 13
#define OIL_PUMP_PIN 33
#define CREAM_PUMP_PIN 32

#define IN3 8
#define IN4 7
#define ENB 5
#define PUMP_DURATION_MS (3000U)
#define STIRRING_SPEED (128U)
/*********************************** Types declarations ************************************/
enum Container_ID {
	zero,one,two,three,four,five,six,seven,eight,nine
};

enum rotation_direction {
	middle, even, odd
};
enum Liquids {
	Water, Milk, Oil, Cream
};
enum Cover_State {
	OPENED, CLOSE
};

/*********************************** Object definitions ************************************/
//Motors
stepperMotor rotationMotor;
stepperMotor sliderMotor;
stepperMotor armMotor;
stepperMotor coverMotor;
Servo containerMotor;
dcMotor stirringMotor;
//Stove
stove mainStove;
//User interface
UserInterface UI;
//UI variables 
bool inputFinished = false;
Additives* additives;


class container {


public:
	enum Container_ID currentContainer;
	enum rotation_direction current_direction = middle;
	enum Cover_State coverState = Cover_State::OPENED;

	container(Container_ID initalContainer) {
		currentContainer = initalContainer;
		pinMode(IN3, OUTPUT);
		pinMode(IN4, OUTPUT);
		pinMode(ENB, OUTPUT);
		pinMode(WATER_PUMP_PIN, OUTPUT);
		pinMode(MILK_PUMP_PIN, OUTPUT);
		pinMode(OIL_PUMP_PIN, OUTPUT);
		pinMode(CREAM_PUMP_PIN, OUTPUT);

		digitalWrite(WATER_PUMP_PIN, HIGH);
		digitalWrite(MILK_PUMP_PIN, HIGH);
		digitalWrite(OIL_PUMP_PIN, HIGH);
		digitalWrite(CREAM_PUMP_PIN, HIGH);

	}

	void startStirring() {
		if (coverState == OPEN)
			return;
		stirringMotor.setSpeed(STIRRING_SPEED);
	}

	void stopStirring() {
		stirringMotor.setSpeed(0);
	}

	void openCover() {
		stopStirring();
		if (coverState == Cover_State::OPENED)
			return;

		digitalWrite(IN3, LOW);
		digitalWrite(IN4, HIGH);
		analogWrite(ENB, 100);
		delay(1000);
		digitalWrite(IN4, LOW);
		digitalWrite(IN3, LOW);
		analogWrite(ENB, 0);
		coverState = Cover_State::OPENED;
		delay(1000);
	}
	void closeCover() {
		if (coverState == Cover_State::CLOSE)
			return;
		coverMotor.changeDirection(stepperDirection::CCW);
		digitalWrite(IN3, HIGH);
		digitalWrite(IN4, LOW);
		analogWrite(ENB, 100);
		delay(1000);
		digitalWrite(IN4, LOW);
		digitalWrite(IN3, LOW);
		analogWrite(ENB, 0);
		delay(1000);

		coverState = Cover_State::CLOSE;
		delay(1000);
	}
	void rotate_to_mid() {
		if (current_direction == even) { rotate_left(); }
		else if (current_direction == odd) { rotate_right(); }
		current_direction = middle;
	}

	void rotate_left() {
		digitalWrite(relPin, HIGH);
		rotationMotor.changeDirection(stepperDirection::CW);
		rotationMotor.write(0.25f);
		delay(1000);
		digitalWrite(relPin, LOW);
		delay(500);
		current_direction = odd;
		//Assert slider motor direction
		sliderMotor.changeDirection(sliderMotor.currDir);
	}

	void rotate_right() {
		digitalWrite(relPin, HIGH);
		rotationMotor.changeDirection(stepperDirection::CCW);
		rotationMotor.write(0.25f);
		delay(1000);
		digitalWrite(relPin, LOW);
		delay(500);
		current_direction = even;
		//Assert slider motor direction
		sliderMotor.changeDirection(sliderMotor.currDir);
	}

	void open_container(uint16 time_push_sec, uint16 time_open_sec) {
		containerMotor.write(135);
		delay(time_push_sec * 1000);
		containerMotor.write(90);
		delay(time_open_sec * 1000);
		close_container(time_push_sec);
	}


	void close_container(uint16 time_pull_sec) {
		containerMotor.write(45);
		delay(time_pull_sec * 1000);
		containerMotor.write(90);
	}

	void get_from_container(enum Container_ID targetContainer, uint16 time_push_sec, uint16 time_open_sec)
	{
		rotate_to_mid();
		delay(1000);
		moveToContainer(targetContainer);
		delay(1000);
		rotateToContainer(targetContainer);
		delay(1000);
		open_container(time_push_sec, time_open_sec);
		delay(1000);
		rotate_to_mid();
		delay(1000);
		//openCover();
		delay(1000);
		moveToContainer(nine);
		delay(2000);
		rotate_then_drop_in_7ala();
		delay(2000);
		moveToContainer(zero);
		//Serial.println(sliderMotor->currDir);
	}

	void moveToContainer(enum Container_ID nextContainer) {

		if (currentContainer / 2 == nextContainer / 2) {
			currentContainer = nextContainer;
			return;
		}
		if ((nextContainer - currentContainer) > 0) {
			sliderMotor.changeDirection(stepperDirection::CW);
		}
		else {
			sliderMotor.changeDirection(stepperDirection::CCW);
		}
		sliderMotor.write(abs((nextContainer / 2) - (currentContainer / 2)) * 0.75f);
		currentContainer = nextContainer;
	}

	void rotateToContainer(enum Container_ID targetContainer) {
		if (targetContainer % 2 == 0) rotate_right();
		else rotate_left();
	}
	void dropFromContainer() {
		armMotor.changeDirection(stepperDirection::CCW);
		armMotor.write(0.36f);
		delay(1000);
		armMotor.invertDirection();
		delay(100);
		armMotor.write(0.36f);
		delay(100);
		armMotor.invertDirection();
		delay(250);

	}

	void pumpLiquid(Liquids liq, uint16 pump_duration_sec) {
		switch (liq) {
		case Water:
			digitalWrite(WATER_PUMP_PIN, LOW);
			delay(pump_duration_sec * 1000);
			digitalWrite(WATER_PUMP_PIN, HIGH);
			break;
		case Milk:
			digitalWrite(MILK_PUMP_PIN, LOW);
			delay(pump_duration_sec * 1000);
			digitalWrite(MILK_PUMP_PIN, HIGH);
			break;
		case Oil:
			digitalWrite(OIL_PUMP_PIN, LOW);
			delay(pump_duration_sec * 1000);
			digitalWrite(OIL_PUMP_PIN, HIGH);
			break;
		case Cream:
			digitalWrite(CREAM_PUMP_PIN, LOW);
			delay(pump_duration_sec * 1000);
			digitalWrite(CREAM_PUMP_PIN, HIGH);
			break;

		}
	}

	void rotate_then_drop_in_7ala() {
		digitalWrite(relPin, HIGH);
		rotationMotor.changeDirection(stepperDirection::CCW);
		rotationMotor.write(0.05f);
		delay(1000);
		dropFromContainer();
		rotationMotor.changeDirection(stepperDirection::CW);
		rotationMotor.write(0.05f);
		delay(1000);
		digitalWrite(relPin, LOW);
		delay(500);
	}
	void getReadyToCook() {
		stopStirring();
		openCover();
		moveToContainer(zero);
	}
};

container c(zero);

/*********************************** Global recipe functions ************************************/

/*Ali was here*/
void botato() {
	//mainStove.heat_minutes_for_normal_humans(0.5);
	c.moveToContainer(one);
	//c.moveToPlate();
	delay(5000);
	c.moveToContainer(one);
}

void recipe_chickenMasala() {
	//Pump oil
	c.pumpLiquid(Oil, 1);
	//Get chicken
	c.get_from_container(six, 2, 1);
	/********* Optional additive *************/
	//**Get extra chicken
	for (uint8 i = 0; i < additives->chicken; ++i)
		c.get_from_container(six, 2, 1);
	/****************************************/
	//Get salt & pepper
	c.get_from_container(five, 1, 0.1);
	//Stir
	c.closeCover();
	c.startStirring();
	//Stir for 10 mins
	delay(10 * 60);
	c.stopStirring();
	c.openCover();
	//Add oil
	c.pumpLiquid(Oil, 1);
	//Get mushroom & onions
	c.get_from_container(eight, 2, 2);
	/********* Optional additive *************/

	//**Get masala spices
	for (uint8 i = 0; i < additives->spices; ++i)
		c.get_from_container(five, 1, 1);
	//**Get onions
	for (uint8 i = 0; i < additives->onions; ++i)
		c.get_from_container(six, 1, 1);
	/****************************************/

	//Add water
	c.pumpLiquid(Water, 1);
	//Add cream
	c.pumpLiquid(Cream, 1);
	//Add pasta
	c.get_from_container(zero, 2, 2);
	//Add cheddar
	//c.get_from_container(Mozzarella, 2, 2);
	//Close cover and stir for 10 mins
	c.closeCover();
	c.startStirring();
	//Wait 10mins
	delay(10 *1000* 60);
	c.stopStirring();
	c.openCover();
}

void roz_blebn() {
	c.closeCover();
	delay(5000);
	c.openCover();
	//c.pumpLiquid(Milk,2);
	//c.pumpLiquid(Cream,2);
	c.get_from_container(zero, 2 ,2);
	//c.pumpLiquid(Water,2);
	//stearing code
	c.get_from_container(three, 1, 1);
	c.get_from_container(five, 1, 1);
	delay(25 * 60 * 1000);
}

void recipe_chickenPasta() {
	//Pump oil
	c.pumpLiquid(Oil, 1);
	//Get chicken
	c.get_from_container(six, 2, 1);
	//Get salt & pepper
	c.get_from_container(five, 1, 0.1);

	/********* Optional additive *************/

	//**Get extra chicken
	for (uint8 i = 0; i < additives->chicken; ++i)
		c.get_from_container(six, 2, 1);

	
	//**Get spices
	for (uint8 i = 0; i < additives->spices; ++i)
		c.get_from_container(six, 2, 1);
	/****************************************/
	//Stir
	c.closeCover();
	c.startStirring();
	//Stir for 10 mins
	delay(10 * 60);
	c.stopStirring();
	c.openCover();
	//Add oil
	c.pumpLiquid(Oil, 1);
	//Get mushroom & onions
	c.get_from_container(eight, 2, 2);
	//Add water
	c.pumpLiquid(Water, 1);
	//Add cream
	c.pumpLiquid(Cream, 1);
	//Add pasta
	c.get_from_container(five, 2, 2);
	//Add cheddar
	//c.get_from_container(Mozzarella, 2, 2);
	//Close cover and stir for 10 mins
	c.closeCover();
	c.startStirring();
	delay(10 * 60);
	c.stopStirring();
	c.openCover();
}

void recipe_MacNCheese() {
	//open cover, stop stirring
	c.getReadyToCook();
	delay(1000);
	c.openCover();
	//Add milk
	c.pumpLiquid(Milk, 6);
	//Wait 1 min
    delay(60 * 1000);
	//delay(1000);
	//Add pasta
	c.get_from_container(one, 2, 3);
	c.get_from_container(one, 2, 3);
	//Close container then stir
	c.closeCover();
	c.startStirring();
	//Wait 10mins
	delay(10*60*1000);
	//delay(2000);
	
	//Open container
	c.openCover();
	//Add mozzarella & salt
	for (uint8 i = 0; i < additives->salt; ++i)
		c.get_from_container(five, 3, 3);
	/********* Optional additive *************/

	for (uint8 i = 0; i < additives->cheese; ++i)
		c.get_from_container(three, 3, 3);

	for (uint8 i = 0; i < additives->spices; ++i)
		c.get_from_container(one, 3, 3);
	/****************************************/

	//Close cover and keep stirring
	c.closeCover();
	c.startStirring();
	//Wait 2mins
	delay(2 * 60 * 1000);

	//delay(100000);
}

void crispyPotato() {
	c.getReadyToCook();
	//Pump oil
	c.pumpLiquid(Oil, 2);
	//Get potatoes
	c.get_from_container(five,1,1);
	//Close cover & start stirring
	c.closeCover();
	c.startStirring();
	//Keep stirring for 3 mins
	delay(5*60*1000);
	/********* Optional additive *************/

	//**Get spices
	for(uint8 i = 0;i< additives->spices;++i)
		c.get_from_container(five, 1, 1);
	//**Get salt
	for (uint8 i = 0; i < additives->salt; ++i)
		c.get_from_container(five, 1, 1);
	/****************************************/

}
/********************************** Setup function **********************************/
char order;
void setup() {					/*To execute only once*/
	Serial.begin(9600);
	/*
		1. Calibrate slider (go to container x as function of rotations)
			-Difference between each container is 2 rotations
			-To go to a container:
				1. specify dir
				2. go back 2 revolutions*containers to go back
		2. Move arm under each container and make sure of its position
	*/
	pinMode(relPin, OUTPUT);
	
	//Motor initialization
	sliderMotor.init(&conf1);
	rotationMotor.init(&conf2);
	containerMotor.attach(3);
	armMotor.init(&conf4);
	coverMotor.init(&conf5);
	stirringMotor.init(&conf6);

	digitalWrite(relPin, LOW);
	digitalWrite(6, LOW);
	digitalWrite(9, LOW);
	digitalWrite(11, LOW);
	
	digitalWrite(12, LOW);
	digitalWrite(8, LOW);
	digitalWrite(7, LOW);

	/*
	digitalWrite(30, LOW);
	digitalWrite(29, LOW);
	analogWrite(31, 0);
	digitalWrite(26, LOW);*/
	//Pre-heat oven
	//mainStove.heat_minutes_for_normal_humans(0.5);
	//armMotor.write(0.25f);
	//delay(10000);
	//c.pumpLiquid(Water, 1);
	//delay(5000);
}

/********************************** Program super loop **********************************/
unsigned char recipe = 0;
void loop() {
	//delay(5000);
	//c.dropFromContainer();
	//c.rotate_then_drop_in_7ala();
	recipe_MacNCheese();
	//roz_blebn();
	//c.openCover();
	/*for (uint16 steps = 0; steps < 1450; ++steps) {
		digitalWrite(8, HIGH);
		delayMicroseconds(1000);
		digitalWrite(8, LOW);
		delayMicroseconds(1000);
	}*/
	//recipe_MacNCheese();
	//c.moveToContainer(zero);
	//delay(50000);
	//recipe_MacNCheese();
	//delay(1000000);
	//cover motor
	/*/c.closeCover();
	c.openCover();*/

	//arm motor
	/*c.dropFromContainer();

	//rotation and arm motors
	c.rotate_then_drop_in_7ala();
	c.closeCover();*/

	//rotationmotor
	/*c.rotate_to_mid();
	c.rotate_left();
	c.rotate_right();*/


	//containerMotor
	//c.open_container(1, 1);
	//c.close_container(1);

	/*
	//slider motor
	c.moveToContainer(zero);

	c.rotateToContainer(zero);

	c.pumpLiquid(Water);
	c.rotate_then_drop_in_7ala();

	//all of the above
	c.get_from_container(zero, 1, 1);*/
	/*
	UI.getUserInput();
	UI.lcd->clear();
	UI.lcd->setCursor(0, 0);
	UI.lcd->println("Cooking :)");
	recipe = UI.getRecipe();
	additives = UI.getAdditives();

	Serial.print("Recipe:");
	Serial.println(recipe);
	Serial.print("Salt:");
	Serial.println(additives->salt);
	Serial.print("spices:");
	Serial.println(additives->spices);
	Serial.print("Cheese:");
	Serial.println(additives->cheese);
	Serial.print("chicken:");
	Serial.println(additives->chicken);
	Serial.print("onions:");
	Serial.println(additives->onions);*/
	
	
	//recipe_MacNCheese();
	//if (!inputFinished) {
	//	UI.getUserInput();
	//	UI.lcd->clear();
	//	UI.lcd->setCursor(0, 0);
	//	UI.lcd->print("Cooking :)");
	//	recipe = UI.getRecipe();
	//	additives = UI.getAdditives();
	//	/********************************************/
	//	Serial.println("Recipe:");
	//	Serial.print(recipe);
	//	Serial.println("Salt:");
	//	Serial.print(additives->salt);
	//	Serial.println("cheese:");
	//	Serial.print(additives->cheese);
	//	Serial.println("chicken:");
	//	Serial.print(additives->chicken);
	//	Serial.println("onions:");
	//	Serial.print(additives->onions);
	//	Serial.println("spices:");
	//	Serial.print(additives->spices);
	//	delay(2000);
	//	/********************************************/

	//	inputFinished = true;
	//	////Decide which recipe to cook
	//	//switch (recipe) {
	//	//case MacNCheese:
	//	//	recipe_MacNCheese();
	//	//	break;
	//	//case ChickenPasta:
	//	//	recipe_chickenPasta();
	//	//	break;
	//	//case RicePudding:
	//	//	roz_blebn();
	//	//	break;
	//	//case Masala:
	//	//	recipe_chickenMasala();
	//	//	break;
	//	//case CrispyPotato:
	//	//	crispyPotato();
	//	//	break;
	//	//}
	//	////To be ready for next recipe
	//	//inputFinished = false;

	//}
	//else {

	//}
	/*coverMotor.write(0.906f);
	delay(1000);
	sliderMotor.write(0.8f);
	delay(1000);
	digitalWrite(relPin, HIGH);
	rotationMotor.write(0.8f);
	delay(1000);
	digitalWrite(relPin, LOW);
	*/
	//roz_blebn();
}





