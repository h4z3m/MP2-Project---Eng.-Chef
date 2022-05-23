//
// 
// TODO: Calibrate pumps duration for each liquid
// 
// 
// 
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
struct Motor_configType conf1 = { 22,23,0,0,0,10800.0f,700 };
//Rotation stepper config
struct Motor_configType conf2 = { 22,23,0,0,0,360,1000 };
//Container servo motor config
struct Motor_configType conf3 = { 3,3,0,0,0,360,0.2f };
//Arm motor stepper config
struct Motor_configType conf4 = { 28,27,0,0,0,360,800 };
//Cover stepper motor
struct Motor_configType conf5 = { 30,29,0,0,0,360,800 };
//Stirring DC motor config
struct Motor_configType conf6 = { 31,26,0,0,0,360,120 };

/*********************************** Definitions ************************************/
#define relPin 24
#define WATER_PUMP_PIN 31
#define MILK_PUMP_PIN 34
#define OIL_PUMP_PIN 33
#define CREAM_PUMP_PIN 32

#define PUMP_DURATION_MS (3000U)
#define STIRRING_SPEED (128U)
/*********************************** Types declarations ************************************/
enum Container_ID {
	DEFAULT_POSITION = 0,
	Chicken = 0,
	Botato = 1,
	Pasta = 1,
	Rice = 3,
	Onions = 3,
	Kale = 4,
	Mozzarella = 3,
	Sugar = 6,
	SaltPepper = 5,
	Spices = 8,
	MasalaSpices = 9,
	//1,3,5
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
//UserInterface UI;
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
		coverMotor.changeDirection(stepperDirection::CCW);
		coverMotor.write(0.9f);
		coverState = Cover_State::OPENED;
		delay(1000);
	}
	void closeCover() {
		if (coverState == Cover_State::CLOSE)
			return;
		coverMotor.changeDirection(stepperDirection::CW);
		coverMotor.write(0.9f);
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
		moveToContainer(MasalaSpices);
		delay(2000);
		rotate_then_drop_in_7ala();
		delay(2000);
		moveToContainer(DEFAULT_POSITION);
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
		rotationMotor.write(0.1f);
		delay(1000);
		dropFromContainer();
		rotationMotor.changeDirection(stepperDirection::CW);
		rotationMotor.write(0.1f);
		delay(1000);
		digitalWrite(relPin, LOW);
		delay(500);
	}
	void getReadyToCook() {
		stopStirring();
		openCover();
		moveToContainer(Chicken);
	}
};

container c(Chicken);

/*********************************** Global recipe functions ************************************/

/*Ali was here*/
void botato() {
	//mainStove.heat_minutes_for_normal_humans(0.5);
	c.moveToContainer(Botato);
	//c.moveToPlate();
	delay(5000);
	c.moveToContainer(Chicken);
}


void roz_blebn() {
	//c.pumpLiquid(Milk);
	//c.pumpLiquid(Cream);
	c.get_from_container(Rice, 2 ,2);
	//c.pumpLiquid(Water);
	//stearing code
	c.get_from_container(Sugar, 1, 1);
	c.get_from_container(Chicken, 1, 1);
	delay(25 * 60 * 1000);
}
void chickenPasta() {
	//Pump oil
	c.pumpLiquid(Oil, 1);
	//Get chicken
	c.get_from_container(Chicken, 2, 1);
	//Get salt & pepper
	c.get_from_container(SaltPepper, 1, 0.1);
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
	c.get_from_container(Onions, 2, 2);
	//Add water
	c.pumpLiquid(Water, 1);
	//Add cream
	c.pumpLiquid(Cream, 1);
	//Add pasta
	c.get_from_container(Pasta, 2, 2);
	//Add cheddar
	c.get_from_container(Mozzarella, 2, 2);
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
	//Add milk
	c.pumpLiquid(Milk, 6);
	//Wait 1 min
	//delay(60 * 1000);
	delay(1000);
	//Add pasta
	c.get_from_container(Pasta,1,4);
	//Close container then stir
	c.closeCover();
	c.startStirring();
	//Wait 10mins
	//delay(10 * 60 * 1000);
	delay(2000);
	
	//Open container
	c.openCover();
	//Add mozzarella & salt
	c.get_from_container(SaltPepper,1,0.4);
	c.get_from_container(Mozzarella,3,3);
	//Close cover and keep stirring
	c.closeCover();
	c.startStirring();
	//Wait 2mins
	delay(2 * 60 * 1000);
	delay(100000);
}

void crispyPotato() {
	c.getReadyToCook();

	//Pump oil
	c.pumpLiquid(Oil, 2);
	//Get potatoes
	c.get_from_container(Botato,1,1);
	//Close cover & start stirring
	c.closeCover();
	c.startStirring();
	//Keep stirring for 3 mins
	delay(3*60*1000);
	//**Get spices
	c.get_from_container(Spices, 1, 1);

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
	containerMotor.attach(3, 1000, 2000);
	armMotor.init(&conf4);
	coverMotor.init(&conf5);
	stirringMotor.init(&conf6);

	digitalWrite(relPin, LOW);
	/*digitalWrite(22, LOW);
	digitalWrite(23, LOW);
	digitalWrite(28, LOW);
	digitalWrite(27, LOW);
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
	delay(5000);
	//c.dropFromContainer();
	//c.rotate_then_drop_in_7ala();
	//recipe_MacNCheese();
	roz_blebn();
	c.moveToContainer(DEFAULT_POSITION);
	delay(50000);
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
	//	inputFinished = true;
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





