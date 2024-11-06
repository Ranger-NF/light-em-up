#include <Arduino.h>
// Pin definitions for NodeMCU ESP8266 board
// Input pins use internal pullup resistors, so buttons should connect to ground
const int redIn = 16;      // GPIO16/D0 - Input pin for Red button  
const int redOut = 5;      // GPIO5/D1 - Output pin for Red LED

const int yellowIn = 4;    // GPIO4/D2 - Input pin for Yellow button
const int yellowOut = 1;   // GPIO1/TX - Output pin for Yellow LED (Note: TX pin may affect serial comms)

const int greenIn = 0;     // GPIO0/D3 - Input pin for Green button  
const int greenOut = 2;    // GPIO2/D4 - Output pin for Green LED

const int blueIn = 14;     // GPIO14/D5 - Input pin for Blue button
const int blueOut = 12;    // GPIO12/D6 - Output pin for Blue LED

const int pinkIn = 13;     // GPIO13/D7 - Input pin for Pink button
const int pinkOut = 15;    // GPIO15/D8 - Output pin for Pink LED

// Game state variables
int score = 0;            // Player's current score
int currentColor = -1;    // Currently active LED color (-1 means no color active)

const int startButton = pinkOut; // Pink button acts as start button

// Multiplayer game state flags
bool isGameRunning = false;      // True when game is in progress
bool isPlayerOneReady = true;    // Player 1 ready state (default ready)
bool isPlayerTwoReady = false;   // Player 2 ready state (must press start)
int currentPlayerNum = 2;        // Current player (1 or 2)

void setup() {
  // Configure LED pins as outputs
  pinMode(redOut, OUTPUT);
  pinMode(blueOut, OUTPUT);
  pinMode(yellowOut, OUTPUT);
  pinMode(greenOut, OUTPUT);
  pinMode(pinkOut, OUTPUT);

  // Configure button pins as inputs with internal pull-up resistors enabled
  // When button pressed, pin reads LOW
  pinMode(redIn, INPUT_PULLUP);
  pinMode(blueIn, INPUT_PULLUP);
  pinMode(yellowIn, INPUT_PULLUP);
  pinMode(greenIn, INPUT_PULLUP);
  pinMode(pinkIn, INPUT_PULLUP);

  // Initialize random number generator with analog noise from unconnected pin
  randomSeed(analogRead(0));

  // Future: Add buzzer support for audio feedback
  // pinMode(buzzer, OUTPUT);
  
  ledAllOn(); // Visual power-on test
}

void loop() {
  if (isGameRunning) {
    // Main game loop
    if (isWhacked(currentColor)) { // Check if player hit the correct button
      ledOff(currentColor);        // Turn off the current target LED
      delay(100);                  // Brief pause for visual feedback

      int newColor = pickNewColor(); // Select next random target
      setupCurrentColor(newColor);   // Activate the new target LED
    }
  } else if (isPlayerOneReady && isPlayerTwoReady) {
    // Both players ready - start new game
    isPlayerOneReady = true;      // Reset player states
    isPlayerTwoReady = false;

    isGameRunning = true;
    score = 0;

    ledAllOff();                  // Clear all LEDs before starting
    delay(500);                   // Brief pause before first target

    int newColor = pickNewColor();   // Select first random target
    setupCurrentColor(newColor);     // Show first target LED
  } else {
    // Wait for players to indicate ready
    int pushedButton = isButtonPressed();

    if (pushedButton == startButton) {
      if (currentPlayerNum == 1) {
        isPlayerOneReady = true;
      } else {
        isPlayerTwoReady = true; 
      }
    }
  }
}

void setupCurrentColor(int newColor) {
  ledOn(newColor);           // Activate the target LED
  currentColor = newColor;   // Store current target color
}

bool isWhacked(int newColor) {
  int chkButton;
  bool buttonPressed = false;

  while (!buttonPressed) {   // Loop until any button is pressed
    chkButton = isButtonPressed();

    if (newColor == chkButton) {     // Player hit correct target
      buttonPressed = true;
      score++;                       // Award point
      if (score >= 10) {
        playerWon();                 // Check for win condition
      }

    } else if (chkButton != 0) {     // Player hit wrong target
      if (score > 0) {
        score--;                     // Penalty: lose a point
      }
    }
    delay(100);                      // Debounce delay
  }

  return buttonPressed;
}

void playerWon() {
  isGameRunning = false;             // End the game
  ledAllOn();                        // Victory light show

  // Reset for next game
  isPlayerOneReady = true;
  isPlayerTwoReady = false;
  delay(2000);                       // Display victory state briefly
}

int isButtonPressed() {
  int pressedButton = 0;

  // Check each button's state and return corresponding LED pin if pressed
  // Buttons are active LOW due to pullup resistors
  if (digitalRead(redIn) == LOW) {
    ledOff(redOut);
    pressedButton = redOut;
  }
  if (digitalRead(blueIn) == LOW) {
    ledOff(blueOut);
    pressedButton = blueOut;
  }
  if (digitalRead(yellowIn) == LOW) {
    ledOff(yellowOut);
    pressedButton = yellowOut;
  }
  if (digitalRead(greenIn) == LOW) {
    ledOff(greenOut);
    pressedButton = greenOut;
  }
  if (digitalRead(pinkIn) == LOW) {
    ledOff(pinkOut);
    pressedButton = pinkOut;
  }

  return pressedButton;
}

// Randomly select a new target LED, avoiding repeats
int pickNewColor() {
  int usableLeds[5] = {greenOut, redOut, blueOut, pinkOut, yellowOut};
  int randIndex = random(0, 5);      // Generate random index 0-4
  int randomColor = usableLeds[randIndex];

  return randomColor;
}

// Activate specified LED by setting it HIGH
void ledOn(int colorON) {
  if (colorON == redOut) {
    digitalWrite(redOut, HIGH);
  } else if (colorON == blueOut) {
    digitalWrite(blueOut, HIGH);
  } else if (colorON == yellowOut) {
    digitalWrite(yellowOut, HIGH);
  } else if (colorON == greenOut) {
    digitalWrite(greenOut, HIGH);
  } else if (colorON == pinkOut) {
    digitalWrite(pinkOut, HIGH);
  }
}

// Deactivate specified LED by setting it LOW
void ledOff(int colorOFF) {
  if (colorOFF == redOut) {
    digitalWrite(redOut, LOW);
  } else if (colorOFF == blueOut) {
    digitalWrite(blueOut, LOW);
  } else if (colorOFF == yellowOut) {
    digitalWrite(yellowOut, LOW);
  } else if (colorOFF == greenOut) {
    digitalWrite(greenOut, LOW);
  } else if (colorOFF == pinkOut) {
    digitalWrite(pinkOut, LOW);
  }
}

// Turn on all LEDs for testing or victory display
void ledAllOn() {
  digitalWrite(redOut, HIGH);
  digitalWrite(blueOut, HIGH);
  digitalWrite(yellowOut, HIGH);
  digitalWrite(greenOut, HIGH);
  digitalWrite(pinkOut, HIGH);
}

// Turn off all LEDs to clear display
void ledAllOff() {
  digitalWrite(redOut, LOW);
  digitalWrite(blueOut, LOW);
  digitalWrite(yellowOut, LOW);
  digitalWrite(greenOut, LOW);
  digitalWrite(pinkOut, LOW);
}
