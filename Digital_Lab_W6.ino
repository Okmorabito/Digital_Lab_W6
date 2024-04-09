unsigned long lastStepTime = 0;
int stepLength = 1000;
long lastDelayTime = 0;
long delayTime = 50;
int ledPinArray[4] = { 7, 8, 9, 10 };
int buttonPinArray[4] = { 33, 34, 35, 37 };
int buttonOnArray[4] = { false, false, false, false };
int lastButtonStateArray[4] = { LOW, LOW, LOW, LOW };
int buttonStateArray[4] = { LOW, LOW, LOW, LOW };
int currentStateArray[4] = { 0, 0, 0, 0 };
int totalSteps = 4;
int currentStep = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(ledPinArray[i], OUTPUT);
    pinMode(buttonPinArray[i], INPUT);
  }
}

void loop() {
  checkButton();
  checkLed();
  sequence();
}

void sequence() {
  if (millis() > lastStepTime + stepLength) {
    lastStepTime = millis();
    usbMIDI.sendNoteOff(60, 127, 1);
    nextStep();
    if (buttonOnArray[currentStep] == true) {
      usbMIDI.sendNoteOn(60, 127, 1);
    }
  }
  Serial.println(currentStep);
}

void checkButton() {
  for (int i = 0; i < 4; i++) {
    currentStateArray[i] = digitalRead(buttonPinArray[i]);
    if (currentStateArray[i] != lastButtonStateArray[i]) {
      lastDelayTime = millis();
    }
    if ((millis() - lastDelayTime) > delayTime) {
      if (currentStateArray[i] != buttonStateArray[i]) {
        buttonStateArray[i] = currentStateArray[i];

        if (buttonStateArray[i] == HIGH) {
          buttonOnArray[i] = !buttonOnArray[i];
        }
      }
    }
    lastButtonStateArray[i] = currentStateArray[i];
  }
}

void checkLed() {
  for (int i = 0; i < 4; i++) {
    if (currentStep == i) {
      analogWrite(ledPinArray[i], 255);
    } else if (buttonOnArray[i] == true) {
      analogWrite(ledPinArray[i], 75);
    } else {
      analogWrite(ledPinArray[i], 5);
    }
  }
}

void nextStep() {
  currentStep = currentStep + 1;
  if (currentStep > 3) {
    currentStep = 0;
  }
}