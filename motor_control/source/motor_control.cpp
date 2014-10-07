#include "motor_control_main.h"

#include <MakeItRobotics.h>//include library
#include <TextFinder.h>
#include <SPI.h>
#include <SoftwareSerial.h>

MakeItRobotics line_following;//declare object

boolean scanning = false;

const int sendInterval = 1000;
long lastTimeSent = 0;

static int XSIZE = 10;
int xpos = 0;

SoftwareSerial mySerial(2, 3); // RX, TX
TextFinder finder(mySerial, 1);

// **************************************************************************
// *                            Power Up Initial
// **************************************************************************
void setup() 
{ 
  mySerial.begin(4800);
  Serial.begin(10420);
  delay(500);
  line_following.line_following_setup();   //initialize the status of line following robot
  line_following.all_stop();               //all motors stop
}
// **************************************************************************
// *                            Main Loop 
// **************************************************************************
void loop() 
{
  long currentTime = millis();
  if (finder.find("SCANEND")){
    scanning = false;
  }
  if(scanning){
    mySerial.print("SCAN");
  }
  if (currentTime - lastTimeSent > sendInterval && xpos < XSIZE){
    if (scanning==false){
      line_following.go_forward(50);
      delay(2000);
      line_following.all_stop();
      xpos++;
      lastTimeSent = currentTime;
      scanning = true;
      mySerial.print("SCAN");
    } 
  }
}

