/*
  Code by George Fokianos and Yannos Koutoulas
*/

#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal.h>

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

// // Define Button Pins
// #define dockButPin
// #define syncButPin
// #define shiftButPin

// // Define LED Pins
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

// Initialize lcd
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

// Declare Servos
// Servo rightLever;
Servo leftEng;
Servo rightEng;
Servo leftShift;
Servo rightShift;

int leftEngNeut = 0;
int rightEngNeut = 0;

void setup()
{
  // // Define Input Pins
  // pinMode(dockButPin, INPUT_PULLUP);
  // pinMode(syncButPin, INPUT_PULLUP);
  // pinMode(shiftButPin, INPUT_PULLUP);
  pinMode(engRun, INPUT_PULLUP);

  // // Define Output Pins
  // pinMode(dockLEDPin, OUTPUT);
  // pinMode(syncLEDPin, OUTPUT);
  // pinMode(shiftLEDPin, OUTPUT);
  pinMode(ignRelPin, OUTPUT);
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
  int engineRun = digitalRead(engRun);
  while (engineRun == LOW)
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
  // leftEngNeut = 1;
  // rightEngNeut = 1;

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
  int leftEngPosF = map(leftThrotPos, 512, 1023, 180, 0);
  int leftEngPosR = map(leftThrotPos, 511, 0, 180, 0);

  if (leftEngNeut == 0)
  {
    if (leftThrotPos > 560)
      leftShift.write(leftRev);
    else if (leftThrotPos < 470)
      leftShift.write(leftAhead);
    else
    {
      leftShift.write(leftNeut);
      leftEngNeut = 1;
    }
  }
  else if (leftEngNeut == 1)
  {
    if (leftThrotPos > 620 || leftThrotPos < 410)
      leftEngNeut = 0;
  }

  if (leftThrotPos > 512)
    leftEng.write(leftEngPosF);
  else
    leftEng.write(leftEngPosR);

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

  if (rightEngNeut == 0)
  {
    if (rightThrotPos > 560)
      rightShift.write(rightRev);
    else if (rightThrotPos < 470)
      rightShift.write(rightAhead);
    else
    {
      rightShift.write(rightNeut);
      rightEngNeut = 1;
    }
  }
  else if (rightEngNeut == 1)
  {
    if (rightThrotPos > 620 || rightThrotPos < 410)
      rightEngNeut = 0;
  }

  if (rightThrotPos > 512)
    rightEng.write(rightEngPosF);
  else
    rightEng.write(rightEngPosR);

  int rightEngRead = rightEng.read();
  int rightthrot = map(rightEngRead, 180, 0, 0, 100);
  lcd.setCursor(14, 1);
  lcd.print(rightThrotPos);
  lcd.print("%");
  lcd.print(" ");
}