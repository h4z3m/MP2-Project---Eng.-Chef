#include <LiquidCrystal.h>
#define DEBUG_MODE (1u)
#include <Stepper.h>

#include "servoMotor.h"
#include "stepperMotor.h" 
#include "stove.h"
/********************************** Global variables **********************************/

struct Motor_configType conf1 = { 4,3,0,0,0,10800.0f,700 };
struct Motor_configType conf2 = { 4,3,0,0,0,360,500 };
struct Motor_configType conf3 = { 9,3,0,0,0,180,0.2f };
struct Motor_configType conf4 = { 8,2,0,0,0,180,0.2f };
stepperMotor stepper;
/*********************************** Definitions ************************************/
#define relPin 5
enum Container_ID {
	zero,one,two,three,four,five,six,seven,eight,nine
};
/*********************************** Types declarations ************************************/
class container {
	stepperMotor* sliderMotor;
	stepperMotor* rotationMotor;
	servoMotor* containerMotor;

	enum Container_ID currentContainer;

public:
	container(Container_ID initalContainer, stepperMotor* slider,stepperMotor* rotation, servoMotor* container) {
		currentContainer = initalContainer;
		sliderMotor = slider;
		rotationMotor = rotation;
		containerMotor = container;
	}
	void rotate_to_mid() {
		
			digitalWrite(relPin, HIGH);
			rotationMotor->invertDirection();
			rotationMotor->write(0.25f);
			delay(250);
			digitalWrite(relPin, LOW);
		
	}
	//void rotate_to_container(Container_ID nextContainer) {
	//	if (nextContainer%2 ==0){
	//		digitalWrite(relPin, HIGH);
	//		
	//		digitalWrite(relPin, LOW);
	//	}
	//	else {
	//		digitalWrite(relPin, HIGH);
	//		rotationMotor->changeDirection(CCW);
	//		rotationMotor->write(0.25f);
	//		rotationMotor->changeDirection(CW);
	//		digitalWrite(relPin, LOW);
	//	}
	//	
	//}
	//void moveToContainer(enum Container_ID nextContainer) {
	//	/*
	//	* 
	//	*   9    8
	//	*   7	 6
	//	*   5    4
	//	*   3    2
	//		1    0

	//	*/
	//	if (currentContainer == nextContainer) {
	//		rotate_to_container(nextContainer);
	//		return;
	//	}
	//	if (currentContainer == zero || currentContainer== one)
	//	{
	//		sliderMotor->write(6.0f);
	//		rotate_to_mid();
	//		sliderMotor->changeDirection(CW);
	//		sliderMotor->write(6.0f);
	//		sliderMotor->changeDirection(CCW);

	//	}
	//	else {
	//		rotate_to_mid();
	//	}
	//	if (abs(currentContainer - nextContainer)==1)
	//		rotate_to_container(nextContainer);
	//	else {
	//		sliderMotor->write(((float(currentContainer) - nextContainer) / 2)*6.0f);
	//		rotate_to_container(nextContainer);

	//	}

	//	
	//	
	//}
	/*
		*
		*   9    8
		*   ^7	 6
		*   5    -4
		*   3    2
			1    0

	//	*/
	void moveToContainer(Container_ID nextContainer) {
		rotate_to_mid();

		int diff = (currentContainer - nextContainer);
		int dist = abs(diff) / 2 * 1-.0f;
		

		if (diff < 0) {
			digitalWrite(relPin, LOW);
			sliderMotor->changeDirection(CW);
			sliderMotor->write(dist);
		}
		else {
			digitalWrite(relPin, LOW);
			sliderMotor->changeDirection(CCW);
			sliderMotor->write(dist);
		}
	}

	void moveToPlate() {
		rotate_to_mid();
		//hazem will write his equation here
		//rotate 2l bowl I have no idea which motor and im too tired to know
		//lazen yetlaf slowly
		delay(2000);
		//roate back 2lbowl
		//rotate left or right to reach container nine idk
		currentContainer = nine;
	}
};

stepperMotor rotationMotor;
stepperMotor sliderMotor;
servoMotor containerMotor;
container c(three,&sliderMotor,&rotationMotor,&containerMotor);
stove mainStove;
Servo myservo;
/*********************************** Global functions ************************************/

/*Ali was here*/
void botato(){
	mainStove.heat_minutes_for_normal_humans(0.5);
	c.moveToContainer(zero);
	c.moveToPlate();
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
	sliderMotor.init(&conf1);
	rotationMotor.init(&conf2);
	//containerMotor.init();
	digitalWrite(relPin, LOW);
	myservo.attach(9);
}

/********************************** Program super loop **********************************/

void loop() {
	/*stepper.write(0.0f);
	delay(1000);
	stepper.write(360.0f);
	delay(1000);
	stepper.write(720.0f);
	delay(1000);
	stepper.write(1080.0f); 
	delay(1000);
	stepper.write(1440.0f);
	delay(1000);
	stepper.write(1800.0f);
	delay(1000);
	stepper.write(2160.0f);*/
	//c.moveToContainer(zero);
	//c.rotate_to_mid();
	//delay(5000);
	/*rotationMotor.write(90);
	delay(2000);
	rotationMotor.changeDirection(CCW);
	rotationMotor.write(90);
	delay(2000);*/
	myservo.write(35);
	delay(1800);
	myservo.write(90);
	delay(100);
	myservo.write(135);
	delay(1500);
	/*rotationMotor.write(2.0f);
	delay(2000);

	digitalWrite(relPin, HIGH);
	rotationMotor.write(0.25f);
	delay(250);
	rotationMotor.invertDirection();
	rotationMotor.write(0.25f);
	delay(250);
	rotationMotor.invertDirection();

	digitalWrite(relPin, LOW);

	rotationMotor.write(2.0f);

	delay(10000);*/
	/*digitalWrite(relPin, HIGH);
	delay(2000);
	rotationMotor.write(0.5f);
	delay(2000);
	digitalWrite(relPin, LOW);
	delay(2000);*/

	/*Ali's code here*/
	//delay before start 34an law hanwasel kahrba yb2a feh forsa
	delay(3000);
	
	//wait for signal from esp or keypad
	/*
	while(true && true){

	
	}
	//if(/*gelna 7aga men esp*/
	//global char = Serial.read();

	//if(/*gelna 7aga men keypad*/){
	//code omar
	
	

	
}

