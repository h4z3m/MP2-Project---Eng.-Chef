#include <DallasTemperature.h>
#include <OneWire.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#define DEBUG_MODE (1u)
#include <Stepper.h>

#include "servoMotor.h"
#include "stepperMotor.h" 
#include "stove.h"
/********************************** Global variables **********************************/
//Slider motor config
struct Motor_configType conf1 = { 5,9,0,0,0,10800.0f,700 };
//Rotation Motormotor config
struct Motor_configType conf2 = { 5,9,0,0,0,360,1000 };
//Container servo motor config
struct Motor_configType conf3 = { 3,3,0,0,0,360,0.2f };
//Arm motor config
struct Motor_configType conf4 = { 11,12,0,0,0,360,800 };

/*********************************** Definitions ************************************/
#define relPin 2
#define WATER_PUMP_PIN 6
#define MILK_PUMP_PIN 7
#define OIL_PUMP_PIN 8
#define CREAM_PUMP_PIN 9
#define PUMP_DURATION_MS (3000U)
enum Container_ID {
	zero, one, two, three, four, five, six, seven, eight, nine
};

enum rotation_direction {
	middle, even, odd
};

enum Liquids {
	Water,Milk,Oil,Cream
};

/*********************************** Types declarations ************************************/
class container {
	stepperMotor* sliderMotor;
	stepperMotor* rotationMotor;
	stepperMotor* armMotor;
	Servo* containerMotor;

public:
	enum Container_ID currentContainer;
	enum rotation_direction current_direction = middle;
	container(Container_ID initalContainer, stepperMotor* slider, stepperMotor* rotation,stepperMotor* arm, servoMotor* container) {
		currentContainer = initalContainer;
		sliderMotor = slider;
		rotationMotor = rotation;
		armMotor = arm;
		containerMotor = container;
	}
	void rotate_to_mid() {
		if (current_direction == even) { rotate_left(); }
		else if (current_direction == odd) { rotate_right(); }
		current_direction = middle;
	}

	void rotate_left() {
		digitalWrite(relPin, HIGH);
		rotationMotor->changeDirection(CW);
		rotationMotor->write(0.25f);
		delay(1000);
		digitalWrite(relPin, LOW);
		delay(500);
		current_direction = odd;
		//Assert slider motor direction
		sliderMotor->changeDirection(sliderMotor->currDir);
	}

	void rotate_right() {
		digitalWrite(relPin, HIGH);
		rotationMotor->changeDirection(CCW);
		rotationMotor->write(0.25f);
		delay(1000);
		digitalWrite(relPin, LOW);
		delay(500);
		current_direction = even;
		//Assert slider motor direction
		sliderMotor->changeDirection(sliderMotor->currDir);
	}

	void open_container(uint16 time_push_sec,uint16 time_open_sec) {
		containerMotor->write(135);
		delay(time_push_sec*1000);
		containerMotor->write(90);
		delay(time_open_sec*1000);
		close_container(time_push_sec);
	}

	void close_container(uint16 time_pull_sec) {
		containerMotor->write(45);
		delay(time_pull_sec*1000);
		containerMotor->write(90);
	}

	void get_from_container(enum Container_ID targetContainer)
	{
		rotate_to_mid();
		delay(1000);
		moveToContainer(targetContainer);
		delay(1000);
		rotateToContainer(targetContainer);
		delay(1000);
		c.open_container(1, 2);
		delay(1000);
		rotate_to_mid();
		delay(1000);
		Serial.println(sliderMotor->currDir);
	}

	void moveToContainer(enum Container_ID nextContainer) {

		if (currentContainer / 2 == nextContainer / 2) {
			currentContainer = nextContainer;
			return;
		}
		if ((nextContainer - currentContainer) > 0) {
			sliderMotor->changeDirection(CW);
		}
		else {
			sliderMotor->changeDirection(CCW);
		}
		sliderMotor->write(abs((nextContainer / 2) - (currentContainer / 2)) * 0.7f);
		currentContainer = nextContainer;
	}

	void rotateToContainer(enum Container_ID targetContainer) {
		if (targetContainer % 2 == 0) rotate_right();
		else rotate_left();
	}
	void dropFromContainer() {
		armMotor->changeDirection(CW);
		armMotor->write(0.5f);
		delay(1000);
		armMotor->invertDirection();
		armMotor->write(0.55f);
		armMotor->invertDirection();
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
};


stepperMotor rotationMotor;
stepperMotor sliderMotor;
stepperMotor armMotor;
stove mainStove;
Servo myservo;

//servoMotor containerMotor;
container c(zero, &sliderMotor, &rotationMotor, &armMotor, &myservo);

/*********************************** Global functions ************************************/

/*Ali was here*/
void botato() {
	mainStove.heat_minutes_for_normal_humans(0.5);
	c.moveToContainer(zero);
	//c.moveToPlate();
	delay(5000);
}
/********************************** Setup function **********************************/
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
	pinMode(13, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(11, OUTPUT);
	pinMode(12, OUTPUT);
	sliderMotor.init(&conf1);
	rotationMotor.init(&conf2);
	myservo.attach(3, 1000, 2000);
	armMotor.init(&conf4);

	digitalWrite(relPin, LOW);
	digitalWrite(11, LOW);
	digitalWrite(12, LOW);
	digitalWrite(13, LOW);
	digitalWrite(7, LOW);

}

/********************************** Program super loop **********************************/

void loop() {

	//Serial.println(c.currentContainer);
	while (true)
	{
		c.dropFromContainer();
		delay(5000);
		/*delay(3000);
		c.get_from_container(seven);
		c.open_container(3);
		c.close_container();
		




		c.get_from_container(zero);
		c.rotate_to_mid();
		rotationMotor.changeDirection(stepperDirection::CW);
		sliderMotor.changeDirection(stepperDirection::CW);*/
		c.get_from_container(seven);
		
		c.get_from_container(zero);
	}
}



/*
-> slider move to given container, and given current container
-> moveSlider(dest, currPos); done

-> rotate rotation motor given the container ID
-> rotationMotor.rotate(cont ID) done

-> servo motor open given open time
-> servoMotor.open(Time);

-> close container
-> servoMotor.close(); should be called after  opened
-> goToOven given current position
-> drop
-> recover
*/
