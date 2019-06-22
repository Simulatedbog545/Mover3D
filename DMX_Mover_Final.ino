// Copyright (c) 2011-2015 by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD style license. See http://www.mathertel.de/License.aspx
// Documentation and samples are available at http://www.mathertel.de/Arduino

// Modified by Thomas Picccione for Mover3D Project 2019

#include <DMXSerial.h>
#include <Servo.h>

Servo PanServo;  // create servo object to control the pan axis servo
Servo TiltServo;  // create servo object to control the tilt axis servo

int PanPos =      90; // variable to store the converted pan servo position
int TiltPos =     90; // variable to store the converted tilt servo position
#define RedPin    11 // PWM output pin for Red LED
#define GreenPin  5  // PWM output pin for Green LED
#define BluePin   3  // PWM output pin for Blue LED
#define WhitePin  6  // PWM output pin for White LED
#define PanPin    8  // servo output pin for Pan axis
#define TiltPin   7  // servo output pin for Tilt axis
#define RXENPIN   2 // output pin for RS485 enable line

#define RedAddress    1 // DMX Address for Red LED intensity
#define GreenAddress  2 // DMX Address for Green LED intensity
#define BlueAddress   3 // DMX Address for Blue LED intensity
#define WhiteAddress  4 // DMX Address for White LED intensity
#define PanAddress    5 // DMX Address for Pan axis location
#define TiltAddress   6 // DMX Address for Tilt axis location

void setup () {
  DMXSerial.init(DMXReceiver);
  PanServo.attach(PanPin);  // attach pan servo
  TiltServo.attach(TiltPin);  // attach tilt servo
  pinMode(RedPin,   OUTPUT); // sets the red LED pin as output
  pinMode(GreenPin, OUTPUT); // sets the green LED pin as output
  pinMode(BluePin,  OUTPUT); // sets the blue LED pin as output
  pinMode(RXENPIN, OUTPUT); // sets the RS485 enable pin as output
  pinMode(PanPin, OUTPUT); // sets the pan servo pin as output
  pinMode(TiltPin, OUTPUT); // sets the tilt servo pin as output
  digitalWrite(RXENPIN, LOW); // sets RS485 enable line LOW to enable DMX reception
  PanServo.write(90); // sets pan servo to center
  TiltServo.write(90); // sets tilt servo to center
}

void loop() {
  // Calculate time since last data packet
  unsigned long lastPacket = DMXSerial.noDataSince();
  if (lastPacket < 10000) {
    // read recent DMX packets and write to LED and servos
    analogWrite(RedPin,   DMXSerial.read(RedAddress)); //write red DMX value to LED
    analogWrite(GreenPin, DMXSerial.read(GreenAddress)); //write green DMX value to LED
    analogWrite(BluePin,  DMXSerial.read(BlueAddress)); //write blue DMX value to LED
    analogWrite(WhitePin,  DMXSerial.read(WhiteAddress)); //write white DMX value to LED
    PanPos = map(DMXSerial.read(PanAddress), 0, 255, 0, 180); //convert pan DMX value to 0-180 range
    TiltPos = map(DMXSerial.read(TiltAddress), 0, 255, 0, 180); //convert tilt DMX value to 0-180 range
    PanServo.write(PanPos); //write converted pan DMX value to pan servo
    TiltServo.write(TiltPos); //write converted tilt DMX value to tilt servo
  }
  else {
    // Set LED to red and set servos to center when no data was received for 10 seconds or more.
    analogWrite(RedPin, 255); //set red LED to full
    analogWrite(GreenPin, 0); //set green LED to 0
    analogWrite(BluePin, 0); // set blue LED to 0
    analogWrite(WhitePin, 0); // set white LED to 0
    PanServo.write(90); // center pan servo
    TiltServo.write(90); // center tilt servo
  }
}
