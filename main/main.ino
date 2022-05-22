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

struct Motor_configType conf1 = { 5,9,0,0,0,10800.0f,700 };
struct Motor_configType conf2 = { 5,9,0,0,0,360,700 };
struct Motor_configType conf3 = { 9,3,0,0,0,180,0.2f };
struct Motor_configType conf4 = { 8,2,0,0,0,180,0.2f };
//struct Motor_configType conf5 = {}
stepperMotor stepper;
Servo myservo;


/*********************************** Definitions ************************************/
#define relPin 2
enum Container_ID {
	zero, one, two, three, four, five, six, seven, eight, nine
};
enum rotation_direction {
	middle, even, odd
};
/*********************************** Types declarations ************************************/
class container {
	stepperMotor* sliderMotor;
	stepperMotor* rotationMotor;
	servoMotor* containerMotor;

public:
	enum Container_ID currentContainer;
	enum rotation_direction current_direction = middle;
	container(Container_ID initalContainer, stepperMotor* slider, stepperMotor* rotation, servoMotor* container) {
		currentContainer = initalContainer;
		sliderMotor = slider;
		rotationMotor = rotation;
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

	void open_container(uint16) {
		myservo.write(35);
		delay(1800);
		myservo.write(90);
		delay(100);
		myservo.write(135);
		delay(1500);

	}

	void get_from_container(enum Container_ID targetContainer)
	{
		rotate_to_mid();
		delay(1000);
		moveToContainer(targetContainer);
		delay(1000);
		rotateToContainer(targetContainer);
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

};
stepperMotor rotationMotor;
stepperMotor sliderMotor;
servoMotor containerMotor;
container c(zero, &sliderMotor, &rotationMotor, &containerMotor);
stove mainStove;



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

		delay(3000);
		c.get_from_container(seven);
		

		c.get_from_container(six);

		c.get_from_container(three);



		c.get_from_container(six);
		c.get_from_container(zero);
		c.rotate_to_mid();
		rotationMotor.changeDirection(stepperDirection::CW);
		sliderMotor.changeDirection(stepperDirection::CW);


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
