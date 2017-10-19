#include <Joystick.h>

// Requires joystick library from https://github.com/MHeironimus/ArduinoJoystickLibrary/tree/master/Joystick

Joystick_ Joystick;

void setup() {
  // Initialize Button Pins
  pinMode(1, INPUT_PULLUP); // Right button
  pinMode(3, INPUT_PULLUP); // Right rocker up
  pinMode(4, INPUT_PULLUP); // Right rocker down
  pinMode(5, INPUT_PULLUP); // Left progression button 1
  pinMode(7, INPUT_PULLUP); // Left progression button 2
  pinMode(9, INPUT_PULLUP); // Left progression button 3
  pinMode(11, INPUT_PULLUP); // Left progression button 4
  pinMode(18, INPUT_PULLUP); // A0 - Left Big Red Button (BRB)
  pinMode(20, INPUT_PULLUP); // A2 - Left switch
  pinMode(21, INPUT_PULLUP); // A3 - Left rocker up
  pinMode(22, INPUT_PULLUP); // A4 - Left rocker down

  // Initialize LED Pins
  pinMode(2, OUTPUT); // Right button LED
  pinMode(6, OUTPUT); // Left progression button 1 LED
  pinMode(8, OUTPUT); // Left progression button 2 LED
  pinMode(10, OUTPUT); // Left progression button 3 LED
  pinMode(12, OUTPUT); // Left progression button 4 LED
  pinMode(19, OUTPUT); // A1 - Left Big Red Button (BRB) LED
  
  // Initialize Joystick Library
  Joystick.begin();
}

// Constant array that lists buttons (the order of the buttons in this list represents the button number that will be presented to the drive station, e.g pinToButtonMap[0] will be button 1, pinToButtonMap[1] will be button 2, etc. Changing the order in the array will adjust the button order presented.
const int pinToButtonMap[11] = {1,3,4,5,7,9,11,18,20,21,22};

// Last state of the button
int lastButtonState[11] = {0,0,0,0,0,0,0,0,0,0,0};

// Constants for buttons in progression with LEDs and the associated LEDs
const int pLEDButtons[4] = {5,7,9,11};
const int pButtonLEDs[4] = {6,8,10,12};

// Last button selected in progression
int pLast = 0;

// Constants for individual LED buttons
const int BRB = 18; // A0
const int BRBLED = 19; // A1
const int BRBLEDTime = 5000;
const int rightButton = 1;
const int rightButtonLED = 2;
const int rightButtonLEDTime = 5000;

// Time last button press for individual LED buttons
unsigned long BRBLast = 0;
unsigned long rightButtonLast = 0;

void loop() {

  // Read pin values
  for (int index = 0; index < 11; index++) {
    int currentButtonState = !digitalRead(pinToButtonMap[index]);
    if (currentButtonState != lastButtonState[index]) {
        Joystick.setButton(index, currentButtonState);
        lastButtonState[index] = currentButtonState;
    }
    // last pressed in progression array
    for (int subindex = 0; subindex < 4; subindex++) {
      if (pinToButtonMap[index] == pLEDButtons[subindex] and currentButtonState == 1) {
        pLast = subindex;
      }
    }
    // BRB last pressed
    if (pinToButtonMap[index] == BRB and currentButtonState == 1) {
      BRBLast = millis();
    }
    // right button last pressed
    if (pinToButtonMap[index] == rightButton and currentButtonState == 1) {
      rightButtonLast = millis();
    }
  }
  // LED button progression - keeps the most recently pressed LED on until other button selected in the progression
  for (int index = 0; index < 4; index++) {
    if (index == pLast) {
      digitalWrite(pButtonLEDs[index], HIGH);
    } else {
      digitalWrite(pButtonLEDs[index], LOW);
    }
  }
  // BRB LED hold - keeps the BRB LED on for period defined by BRBLEDTime
  if (BRBLast != 0 and BRBLast + BRBLEDTime > millis()) {
    digitalWrite(BRBLED, HIGH);
  } else {
    digitalWrite(BRBLED, LOW);
  }
  // Right button hold - keeps the right button LED on for a period defined by rightButtonLEDTime
  if (rightButtonLast != 0 and rightButtonLast + rightButtonLEDTime > millis()) {
    digitalWrite(rightButtonLED, HIGH);
  } else {
    digitalWrite(rightButtonLED, LOW);
  }

  delay(10);
}

