int currentPlayerNum = 2;

int redIn = D0;        // Input pin for Red button
int redOut = D1;

int greenIn = D3;     // Input pin for Green button
int greenOut = D4;    // Output pin for Green LED

int blueIn = D5;       // Input pin for Blue button
int blueOut = D6;      // Output pin for Blue LED

int pinkIn = D7;       // Input pin for Pink button
int pinkOut = D8;      // Output pin for Pink LED

int yellowIn = D2;    // Input pin for Yellow button
int yellowOut = 1;    // Output pin for Yellow LED

int score = 0;        // Track the score
int currentColor = -1;

int startButton = pinkOut; // Change this for every board

bool isGameRunning = false;

bool isPlayerOneReady = true;
bool isPlayerTwoReady = false;

void setup() {
  // Setup all LED output pins
  pinMode(redOut, OUTPUT);
  pinMode(blueOut, OUTPUT);
  pinMode(yellowOut, OUTPUT);
  pinMode(greenOut, OUTPUT);
  pinMode(pinkOut, OUTPUT);

  // Setup all button input pins with pull-up resistors
  pinMode(redIn, INPUT_PULLUP);
  pinMode(blueIn, INPUT_PULLUP);
  pinMode(yellowIn, INPUT_PULLUP);
  pinMode(greenIn, INPUT_PULLUP);
  pinMode(pinkIn, INPUT_PULLUP);

  // pinMode(buzzer, OUTPUT); // Setup buzzer as output
  ledAllOn();
}

void loop() {
  if (isGameRunning) {
    if (isWhacked(currentColor)) { // Check if the correct button was pressed
      ledOff(currentColor); // Turn off current LED
      delay(100); // Pause briefly before showing a new mole

      int newColor = pickNewColor(); // Pick a new color

      setupCurrentColor(newColor); // Set up the new color
    }
  } else if (isPlayerOneReady && isPlayerTwoReady) {
    isPlayerOneReady = true; // Change
    isPlayerTwoReady = false;

    isGameRunning = true;
    score = 0;

    ledAllOff();

    delay(500);

    int newColor = pickNewColor(); // Pick a random color to start

    setupCurrentColor(newColor); // Initialize the first "mole"
  } else {
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
  ledOn(newColor);  // Turn on the LED of the new color
  currentColor = newColor; // Update the active color
}

boolean isWhacked(int newColor) {
  int chkButton;
  boolean buttonPressed = false;

  while (!buttonPressed) { // Wait until any button is pressed
    chkButton = isButtonPressed();

    if (newColor == chkButton) { // Correct button pressed
      buttonPressed = true;
      score++; // Increase score
      if (score >= 10) {
        playerWon();
      }

    } else if (chkButton != 0) { // Incorrect button pressed
      if (score > 0) {
        score--; // Increase score
      }
    }
    delay(100);
  }

  return buttonPressed;
}

void playerWon() {
  isGameRunning = false;
  ledAllOn();

  isPlayerOneReady = true; // Change
  isPlayerTwoReady = false;
  delay(2000);
}

int isButtonPressed() {
  int pressedButton = 0;

  // Check each button and return the corresponding LED output pin if pressed
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

// Function to randomly pick a new color LED
int pickNewColor() {
  int usableLeds[5] = {greenOut, redOut, blueOut, pinkOut, yellowOut};
  int randIndex = random(0, 5); // Random index between 0 and 4
  int randomColor = usableLeds[randIndex];

  return randomColor;
}

// Function to turn on LED based on the given color pin
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

// Function to turn off LED based on the given color pin
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

void ledAllOn() {
  digitalWrite(redOut, HIGH);
  digitalWrite(blueOut, HIGH);
  digitalWrite(yellowOut, HIGH);
  digitalWrite(greenOut, HIGH);
  digitalWrite(pinkOut, HIGH);
}

void ledAllOff() {
  digitalWrite(redOut, LOW);
  digitalWrite(blueOut, LOW);
  digitalWrite(yellowOut, LOW);
  digitalWrite(greenOut, LOW);
  digitalWrite(pinkOut, LOW);
}
