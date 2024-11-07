#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* SSID = "FTTH";
const char* password = "12345678";

int currentPlayerId = -1;

int redIn = D1;        // Input pin for Red button
int redOut = D0;

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

bool isPlayerReady = false;
bool isOpponentReady = false;

int wifiTimeout = 10;
int isWifiConnected = false;

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

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, password);

  while (WiFi.status() != WL_CONNECTED && wifiTimeout > 0) {
    wifiTimeout--;
    ledOn(redOut);
    delay(150);
    ledOn(pinkOut);
    delay(150);
    ledOn(greenOut);
    delay(150);
    ledOn(yellowOut);
    delay(150);
    ledOn(blueOut);
    delay(150);
    ledAllOff();
    delay(150);
  }

  if (WiFi.status() == WL_CONNECTED) {
      isWifiConnected = true;

      ledOn(greenOut);
      delay(200);
      ledOff(greenOut);
      delay(200);
      ledOn(greenOut);
      delay(200);
      ledOff(greenOut);
      delay(200);
      ledOn(greenOut);
      delay(200);
      ledOff(greenOut);
      delay(200);
  } else { // Connection failed
      isWifiConnected = false;

        ledOn(redOut);
        delay(200);
        ledOff(redOut);
        delay(200);
        ledOn(redOut);
        delay(200);
        ledOff(redOut);
        delay(200);
        ledOn(redOut);
        delay(200);
        ledOff(redOut);
        delay(200);

  }
    ledAllOn();

  // pinMode(buzzer, OUTPUT); // Setup buzzer as output
}

void loop() {
  if (isGameRunning) {
    if (isCorrectButtonPresed(currentColor)) { // Check if the correct button was presse

      if (isGameRunning) {
        ledOff(currentColor); // Turn off current LED
        delay(500);
        int newColor = pickNewColor(); // Pick a new color
        setupCurrentColor(newColor); // Set up the new color
      }
    }
  } else if (isPlayerReady && isOpponentReady) {
    isPlayerOneReady = true; // Change
    isPlayerTwoReady = false;

    isGameRunning = true;
    score = 0;

    ledAllOff();

    delay(100);

    int newColor = pickNewColor(); // Pick a random color to start

    setupCurrentColor(newColor); // Initialize the first "mole"
  } else {
    int pushedButton = isButtonPressed();

    if (pushedButton == startButton) {
        isPlayerReady = true
    }
  }

  delay(1);

}

void setupCurrentColor(int newColor) {
  ledOn(newColor);  // Turn on the LED of the new color
  currentColor = newColor; // Update the active color
}

boolean isCorrectButtonPresed(int currentTargetColor) {
    int pressedButton;
    boolean correctButtonPressed = false;

    pressedButton = isButtonPressed();

    if (currentTargetColor == pressedButton) { // Correct button pressed
        ledOff(pressedButton);
        correctButtonPressed = true;
        score++; // Increase score
        sendMessage("Score: " + String(score));

        if (score >= 10) {
            playerWon();
        }
    }
    return correctButtonPressed;
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
    pressedButton = redOut;
  }
  if (digitalRead(blueIn) == LOW) {
    pressedButton = blueOut;
  }
  if (digitalRead(yellowIn) == LOW) {
    pressedButton = yellowOut;
  }
  if (digitalRead(greenIn) == LOW) {
    pressedButton = greenOut;
  }
  if (digitalRead(pinkIn) == LOW) {
    pressedButton = pinkOut;
  }
  if (pressedButton != 0) {
      ledOff(pressedButton);
      // sendMessage("Pressed button: " + String(pressedButton));

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

void sendMessage(String msg) {
    return;
    WiFiClient client;

    const char* serverHost = "ntfy.sh";

    if (!client.connect(serverHost, 80)) {
      return;
    }
    String url = "/light-em-up";


    client.print("POST " + url + " HTTP/1.1\r\n" +
                 "Host: " + serverHost + "\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + msg.length() + "\r\n" +
                 "Connection: close\r\n\r\n" +
                 msg);

}
