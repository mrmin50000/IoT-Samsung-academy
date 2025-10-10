#include <Arduino.h>

#define CAR_RED   23
#define CAR_YELLOW 22
#define CAR_GREEN 21
#define PED_RED   17
#define PED_GREEN 16
#define BUTTON_PIN    13

enum TrafficState {
  CARS_GO,
  CARS_STOPPING,
  PEDS_GO,
  PEDS_STOPPING
};

TrafficState currentState = CARS_GO;
bool pedestrian = false;
bool redState = false;
unsigned long stateStartTime = 0;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 100;

void setTrafficLights(bool red, bool yellow, bool green) {
  digitalWrite(CAR_RED, red);
  digitalWrite(CAR_YELLOW, yellow);
  digitalWrite(CAR_GREEN, green);
}

void setPedLights(bool red, bool green) {
  digitalWrite(PED_RED, red);
  digitalWrite(PED_GREEN, green);
}

void checkButton() {
  if (digitalRead(BUTTON_PIN) == HIGH) {
    if (millis() - lastDebounceTime > debounceDelay) {
      pedestrian = true;
    }
    lastDebounceTime = millis();
  }
  Serial.println(pedestrian);
}

void setup() {
  Serial.begin(9600);
  pinMode(CAR_RED, OUTPUT);
  pinMode(CAR_YELLOW, OUTPUT);
  pinMode(CAR_GREEN, OUTPUT);
  pinMode(PED_RED, OUTPUT);
  pinMode(PED_GREEN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
          
}

void loop() {
  checkButton();
  unsigned long currentTime = millis();
  unsigned long stateDuration = currentTime - stateStartTime;

  switch (currentState) {
    case CARS_GO:
      setPedLights(HIGH, LOW); 
      if (stateDuration < 2000)
        setTrafficLights(LOW, HIGH, LOW);
      else
        setTrafficLights(LOW, LOW, HIGH);
      
      if (stateDuration > 10000) {
        currentState = CARS_STOPPING;
        stateStartTime = currentTime;
      }
      break;

    case CARS_STOPPING:
      if (!redState)
        setTrafficLights(LOW, HIGH, LOW);  
      
      if (pedestrian && !redState && stateDuration > 2000) {
        currentState = PEDS_GO;
        stateStartTime = currentTime;
      }
      if (!pedestrian && stateDuration > 2000 && stateDuration < 12000) {
        setTrafficLights(HIGH, LOW, LOW);
        redState = true;
      }
      if (stateDuration >= 12000){
        currentState = CARS_GO;
        stateStartTime = currentTime;
        redState = false;
      }
      break;

    case PEDS_GO:
      setTrafficLights(HIGH, LOW, LOW);  
      setPedLights(LOW, HIGH);           
      
      if (stateDuration > 7000) {
        currentState = PEDS_STOPPING;
        stateStartTime = currentTime;
      }
      break;

    case PEDS_STOPPING:
      setTrafficLights(HIGH, LOW, LOW); 
      setPedLights(LOW, (currentTime / 500) % 2);
      
      if (stateDuration > 3000) {
        currentState = CARS_GO;
        stateStartTime = currentTime;
        setPedLights(HIGH, LOW);
      }
      pedestrian = false;
      break;
  }
}
