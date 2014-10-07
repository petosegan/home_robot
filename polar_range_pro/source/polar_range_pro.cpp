#include "polar_range_pro_main.h"
 
#include <NewPing.h>
#include <TextFinder.h>
#include <Stepper.h>
#include <SoftwareSerial.h>
 
#define TRIGGER_PIN  8  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     9  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define DELAY_TIME 1
#define MOTOR_PIN_1 10
#define MOTOR_PIN_2 11
#define MOTOR_PIN_3 12
#define MOTOR_PIN_4 13
 
String remoteAddress = "642737BF757E"; //mimir
 
SoftwareSerial mySerial(2, 3);
TextFinder finder(Serial);
TextFinder ardfinder(mySerial);
 
//boolean connection = false;
boolean connection = true;
boolean commandMode = false;
 
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor
const int maxSteps = 200;
int stepCount = 0;         // number of steps the motor has taken
int ranges[stepsPerRevolution];
 
Stepper myStepper(stepsPerRevolution,MOTOR_PIN_1, MOTOR_PIN_2, MOTOR_PIN_3, MOTOR_PIN_4);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
 
void setup() {
	mySerial.begin(4800);
	Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
//	Serial.print("$$$");
//	if(finder.find("CMD")){
//		commandMode = true;
//	}
	 
//	if (commandMode){
//		Serial.print("C," + remoteAddress + "\r");
//		finder.find("CONNECT");
//		if (finder.find("failed")){
//			connection = false;
//		}
//		else {
//			connection = true;
//			commandMode = false;
//		}
//	}
	if(connection){
		scan(ranges);
		report(ranges);
	}
}
 
void loop() {
	mySerial.println("SCANEND");
	Serial.println("SCANEND");
	if(ardfinder.find("SCAN")){
		Serial.println("Start Scan:");
		scan(ranges);
		report(ranges);
	}
}
 
void scan(int *ranges) {
	for(int i=0; i<maxSteps; i++){
		unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
		ranges[i] = uS / US_ROUNDTRIP_CM;
		myStepper.step(1);
	}
	for(int j=0; j<maxSteps; j++){
		myStepper.step(-1);
		delay(1);
	}
}
 
void report(int *ranges){
	for(int i = 0; i < maxSteps; i++){
		Serial.print(String(ranges[i]) + ',' + String(i) + '\n'); // Convert ping time to distance in cm and print result (0 = outside set distance range)
	}
	mySerial.print("SCANEND");
}
 
