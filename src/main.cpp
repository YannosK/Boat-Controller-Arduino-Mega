/*
  Code by George Fokianos and Yannos Koutoulas
*/

#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal.h>

// Initialize lcd
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

// Declare Servos
// Servo rightLever;
Servo leftEng;
Servo rightEng;
Servo leftShift;
Servo rightShift;

// Define Servo Pins
// #define leftLevPin
// #define rightLevPin
#define leftEngPin 12
#define rightEngPin 13
#define leftShiftPin 10
#define rightShiftPin 11

// Define Throttle Pins
#define leftThrotPin A1
#define rightThrotPin A2
// Define Throttle Limits
#define leftThrotMax 1023
#define leftThrotCent 512
#define leftThrotMin 0

// Define Button Pins
// #define dockButPin
// #define syncButPin
// #define shiftButPin

// Define LED Pins
// #define dockLEDPin
// #define syncLEDPin
// #define shiftLEDPin

// Define Relay Pins
#define ignRelPin 8
#define engRun 9
#define rightThrotMax 1023
#define rightThrotCent 512
#define rightThrotMin 0

// Define Engine Servo Limits
#define leftengMin 180
#define leftengMax 0
#define leftengDockMin 180
#define leftengDockMax 60
#define rightengMin 180
#define rightengMax 0
#define rightengDockMin 180
#define rightengDockMax 60
// Define Shifter Servo Positions
#define leftRev 45
#define leftNeut 90
#define leftAhead 135
#define rightRev 45
#define rightNeut 90
#define rightAhead 135

int leftEngNeut = 0;
int rightEngNeut = 0;

void setup()
{
  // // Define Input Pins
  // pinMode(dockButPin, INPUT_PULLUP);
  // pinMode(syncButPin, INPUT_PULLUP);
  // pinMode(shiftButPin, INPUT_PULLUP);
  // pinMode(engRun, INPUT_PULLUP);

  // // Define Output Pins
  // pinMode(dockLEDPin, OUTPUT);
  // pinMode(syncLEDPin, OUTPUT);
  // pinMode(shiftLEDPin, OUTPUT);
  // pinMode(ignRelPin, OUTPUT);
  digitalWrite(ignRelPin, HIGH);

  // Initialize Devices
  lcd.begin(20, 4);

  // leftLever.attach(leftLevPin);
  // rightLever.attach(rightLevPin);
  leftEng.attach(leftEngPin);
  rightEng.attach(rightEngPin);
  leftShift.attach(leftShiftPin);
  rightShift.attach(rightShiftPin);

  // LCD Welcome Screen
  lcd.setCursor(1, 0);
  lcd.print("GLIDE BY WIRE BETA");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Initializing Devices");

  // Set Servos at Idle positions
  leftEng.write(leftengMin);
  delay(1000);
  rightEng.write(rightengMin);
  delay(1000);
  leftShift.write(leftNeut);
  delay(1000);
  rightShift.write(rightNeut);
  delay(1000);

  lcd.setCursor(0, 2);
  lcd.print("      Complete");
  delay(2000);

  lcd.clear();
  delay(1000);

  // Check if the engine is running
  // int engineRun = digitalRead(engRun);
  while (!(digitalRead(engRun)))
  {
    lcd.setCursor(0, 0);
    lcd.print(" ENGINE IS RUNNING!");
    lcd.setCursor(0, 1);
    lcd.print("Please Shut Down The");
    lcd.setCursor(0, 2);
    lcd.print(" Engine And Restart");
    lcd.setCursor(0, 3);
    lcd.print("     The System");
  }

  lcd.clear();

  // While loop does not end when both conditions met but when only one of them
  while ((analogRead(leftThrotPin) != 1023) || (analogRead(rightThrotPin) != 1023))
  {
    lcd.setCursor(0, 0);
    lcd.print("   Please Set The");
    lcd.setCursor(0, 1);
    lcd.print(" Thrusters At Idle");
    lcd.setCursor(0, 2);
    lcd.print("    To Continue...");
  }

  delay(1000);
  lcd.clear();
  delay(1000);
  digitalWrite(ignRelPin, LOW);
  delay(1000);

  lcd.setCursor(0, 0);
  lcd.print(" All Systems Check!");
  lcd.setCursor(0, 1);
  lcd.print(" You Can Now Start");
  lcd.setCursor(0, 2);
  lcd.print("     The Engine");

  delay(1000);

  lcd.clear();
  lcd.print("ENGINE 1    ENGINE 2");
}

void loop()
{

  // Left Throttle Control

  int leftThrotPos = analogRead(leftThrotPin);
  int leftEngPosF = map(leftThrotPos, 517, 1023, 180, 0);
  int leftEngPosR = map(leftThrotPos, 506, 0, 180, 0);
  // int leftEngPosN = map(leftThrotPos, )

  if (leftEngNeut == 0 && leftThrotPos > 517)
  {
    leftEng.write(leftEngPosF);
    leftShift.write(leftAhead);
  }
  else if (leftEngNeut == 0 && leftThrotPos < 506)
  {
    leftEng.write(leftEngPosR);
    leftShift.write(leftRev);
  }
  else
  {
    if (leftEngNeut == 0)
    {
      leftEng.write(leftNeut);
      leftEngNeut = 1;
    }
    else
    {
      if (leftThrotPos < 519 && leftThrotPos > 504)
        leftEngNeut = 0;
    }
  }

  int leftEngRead = leftEng.read();
  int leftthrot = map(leftEngRead, 180, 0, 0, 100);
  lcd.setCursor(2, 1);
  lcd.print(leftThrotPos);
  lcd.print("%");
  lcd.print(" ");

  // Right Throttle Control

  int rightThrotPos = analogRead(rightThrotPin);
  int rightEngPosF = map(rightThrotPos, 512, 1023, 180, 0);
  int rightEngPosR = map(rightThrotPos, 511, 0, 180, 0);

  if (rightThrotPos > 511)
    rightEng.write(rightEngPosF);
  else
    rightEng.write(rightEngPosR);

  if (rightThrotPos > 511)
  {
    rightShift.write(rightRev);
  }
  else
    rightShift.write(rightAhead);

  int rightEngRead = rightEng.read();
  int rightthrot = map(rightEngRead, 180, 0, 0, 100);
  lcd.setCursor(14, 1);
  lcd.print(rightthrot);
  lcd.print("%");
  lcd.print(" ");
}
