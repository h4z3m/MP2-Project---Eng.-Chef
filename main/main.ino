#include <DallasTemperature.h>
#include <OneWire.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#define DEBUG_MODE (1u)
#include <Stepper.h>
#include "dcMotor.h"
#include "servoMotor.h"
#include "stepperMotor.h" 
#include "stove.h"
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
#define WATER_PUMP_PIN 32
#define MILK_PUMP_PIN 33
#define OIL_PUMP_PIN 34
#define CREAM_PUMP_PIN 31
#define PUMP_DURATION_MS (3000U)
#define STIRRING_SPEED (128U)
enum Container_ID {
	zero, one, two, three, four, five, six, seven, eight, nine
};

enum rotation_direction {
	middle, even, odd
};

enum Liquids {
	Water,Milk,Oil,Cream
};
enum Cover_State {
	OPENED,CLOSED
};

/*********************************** Types declarations ************************************/


stepperMotor rotationMotor;
stepperMotor sliderMotor;
stepperMotor armMotor;
stepperMotor coverMotor;
Servo containerMotor;
dcMotor stirringMotor;

stove mainStove;


class container {
	

public:
	enum Container_ID currentContainer;
	enum rotation_direction current_direction = middle;
	enum Cover_State coverState;

	container(Container_ID initalContainer) {
		currentContainer = initalContainer;
		
	}
	void startStirring() {
		stirringMotor.setSpeed(STIRRING_SPEED);
	}
	
	void stopStirring() {
		stirringMotor.setSpeed(0);
	}

	/*void openCover() {
		stopStirring();
		if (coverState == OPENED)
			return;
		coverMotor.changeDirection(stepperDirection::CCW);
		coverMotor.write(0.9f);
		coverState = OPENED;
		delay(1000);
	}
	void closeCover() {
		if (coverState == CLOSED)
			return;
		coverMotor.changeDirection(stepperDirection::CW);
		coverMotor.write(0.9f);
		coverState = CLOSED;
		delay(1000);
	}*/
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

	void open_container(uint16 time_push_sec,uint16 time_open_sec) {
		containerMotor.write(135);
		delay(time_push_sec*1000);
		containerMotor.write(90);
		delay(time_open_sec*1000);
		close_container(time_push_sec);
	}


	void close_container(uint16 time_pull_sec) {
		containerMotor.write(45);
		delay(time_pull_sec*1000);
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
		//moveToContainer(zero);
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
		armMotor.write(0.36f);
		armMotor.invertDirection();
		delay(250);

	}

	void pumpLiquid(Liquids liq) {
		switch (liq) {
		case Water:
			digitalWrite(WATER_PUMP_PIN, HIGH);
			delay(PUMP_DURATION_MS);
			digitalWrite(WATER_PUMP_PIN, LOW);
			break;
		case Milk:
			digitalWrite(MILK_PUMP_PIN, HIGH);
			delay(PUMP_DURATION_MS);
			digitalWrite(MILK_PUMP_PIN, LOW);
			break;
		case Oil:
			digitalWrite(OIL_PUMP_PIN, HIGH);
			delay(PUMP_DURATION_MS*3);
			digitalWrite(OIL_PUMP_PIN, LOW);
			break;
		case Cream:
			digitalWrite(CREAM_PUMP_PIN, HIGH);
			delay(PUMP_DURATION_MS*3);
			digitalWrite(CREAM_PUMP_PIN, LOW);
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

};

container c(zero);

/*********************************** Global functions ************************************/

/*Ali was here*/
void botato() {
	//mainStove.heat_minutes_for_normal_humans(0.5);
	c.moveToContainer(two);
	//c.moveToPlate();
	delay(5000);
	c.moveToContainer(zero);
}

void roz_blebn() {
	//c.pumpLiquid(Milk);
	//c.pumpLiquid(Cream);
	c.get_from_container(zero,1,1);
	//c.pumpLiquid(Water);
	//stearing code
	c.get_from_container(one, 1, 1);
	c.get_from_container(two, 1, 1);
	delay(25 * 60 * 1000);
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
	pinMode(30, OUTPUT);
	pinMode(29, OUTPUT);
	//Motor initialization
	sliderMotor.init(&conf1);
	rotationMotor.init(&conf2);
	containerMotor.attach(3, 1000, 2000);
	armMotor.init(&conf4);
	coverMotor.init(&conf5);
	stirringMotor.init(&conf6);

	digitalWrite(relPin, LOW);
	digitalWrite(30, LOW);
	digitalWrite(29, LOW);

	//mainStove.heat_minutes_for_normal_humans(0.5);
}

/********************************** Program super loop **********************************/
unsigned char recipe = 0;
void loop() {
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
	

}




	