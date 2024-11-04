#include <Servo.h>
Servo servo_class;

int currentPlayerNum = 2;

// Initialize all LEDs and buttons with corresponding pins
// 1. Red
int redIn =13;        // Input pin for Red button
int redOut = 2;       // Output pin for Red LED
int red = 0;
// 2. Green
int green = 0;
int greenIn = 11;     // Input pin for Green button
int greenOut = 10;    // Output pin for Green LED
// 3. Blue
int blue = 0;
int blueIn = 8;       // Input pin for Blue button
int blueOut = 4;      // Output pin for Blue LED
// 4. Pink
int pink = 0; 
int pinkIn = 9;       // Input pin for Pink button
int pinkOut = 5;      // Output pin for Pink LED
// 5. Yellow 
int yellow = 0;
int yellowIn = 12;    // Input pin for Yellow button
int yellowOut = 6;    // Output pin for Yellow LED
// Buzzer
int buzzer = 3;      // Pin for Buzzer
int servo_motor = 7;  // Pin for Servo motor

int score = 0;        // Track the score
int increment = 18;   // Servo rotation increment per hit
int motorRotation = 0; // Current rotation angle for the servo

int currentColor = -1; // Holds the active color for each round

int startButton = redOut; // Change this for every board

bool isGameRunning = false;

bool isPlayerOneReady = false;
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

  pinMode(buzzer, OUTPUT); // Setup buzzer as output

  servo_class.attach(servo_motor); // Attach servo to pin
  servo_class.write(0); // Initialize servo position to 0 degrees


  randomSeed(analogRead(0)); // Seed the random generator

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


    isPlayerOneReady = false;
    isPlayerTwoReady = false;

    isGameRunning = true;

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
  boolean whacked = false;
  boolean buttonPressed = false;

  while (!buttonPressed) { // Wait until any button is pressed
    chkButton = isButtonPressed(); 

    if (newColor == chkButton) { // Correct button pressed
      whacked = true;
      buttonPressed = true;
      tone(buzzer, 2000, 500); // Play buzzer sound

      score++; // Increase score
      motorRotation = (score * increment); // Calculate servo rotation
    
      servo_class.write(motorRotation); // Move servo

      if (motorRotation >= 180) {
        playerWon();
      }

    } else if (chkButton > 0) { // Incorrect button pressed
      whacked = false;
      buttonPressed = true;

      if (score > -1) {
        score--; // Increase score
        motorRotation = (score * increment); // Calculate servo rotation
      
        servo_class.write(motorRotation); // Move servo
      }
    } 
    delay(100);
  }


  return whacked;
}

void playerWon() {
  isGameRunning = false;
  ledAllOn();

  motorRotation = 0;
  score = 0;

  int spinTime = 3000;
  int i =0;

  while (i < spinTime) {
    motorRotation = i % 180;
    servo_class.write(motorRotation);
    i++;
    delay(1);
  }

  motorRotation = 0;
  servo_class.write(motorRotation);
}

int isButtonPressed() {
  int buttonPressed = 0;

  // Check each button and return the corresponding LED output pin if pressed
  if (digitalRead(redIn) == LOW) {
    ledOff(redOut);
    buttonPressed = redOut;
  }
  
  if (digitalRead(blueIn) == LOW) {
    ledOff(blueOut);
    buttonPressed = blueOut;
  }
  
  if (digitalRead(yellowIn) == LOW) {
    ledOff(yellowOut);
    buttonPressed = yellowOut;
  }
  
  if (digitalRead(greenIn) == LOW) {
    ledOff(greenOut);
    buttonPressed = greenOut;
  }
  
  if (digitalRead(pinkIn) == LOW) {
    ledOff(pinkOut);
    buttonPressed = pinkOut;
  }

  return buttonPressed;
}

// Function to randomly pick a new color LED
int pickNewColor() {
  int usableLeds[5] = {redOut, greenOut, blueOut, pinkOut, yellowOut};
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

