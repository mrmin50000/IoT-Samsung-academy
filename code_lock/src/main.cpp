#include <Arduino.h>
#define BUTTON_1 19
#define BUTTON_2 18
#define BUTTON_3 21
#define BUTTON_4 5
#include <string>

bool buttonState_1 = false;
bool buttonState_2 = false;
bool buttonState_3 = false;
bool buttonState_4 = false;

unsigned long stateStartTime = 0;

#define LED_PIN 2
#define RELAY_PIN 27
#define LDR_PIN 35

int pass[4] = {1, 2, 3, 4};
int code[4];
int i = 0;

enum State {
  OPEN,
  CLOSE,
  DAY,
};

State stateDoor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}
std::string a = "Time out";

void loop() {  
  unsigned long currentTime = millis();
  unsigned long stateDuration = currentTime - stateStartTime;
  int ldrValue = analogRead(LDR_PIN);
  if (ldrValue <= 1000) {
    stateDoor = DAY;
  }
  Serial.println("");
  switch (stateDoor) {
    
    case CLOSE:
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(RELAY_PIN, HIGH);

      buttonState_1 = digitalRead(BUTTON_1);
      buttonState_2 = digitalRead(BUTTON_2);
      buttonState_3 = digitalRead(BUTTON_3);
      buttonState_4 = digitalRead(BUTTON_4);

      if (i > 0 && stateDuration > 3000) {
        a = "Time out";
        i = 0;
      }

      if (!buttonState_1) {
        code[i] = 1;
        ++i;
        delay(300);
        stateStartTime = currentTime;
      }      
      else if (!buttonState_2) {
        code[i] = 2;
        ++i;
        delay(300);
        stateStartTime = currentTime;
      }
      else if (!buttonState_3) {
        code[i] = 3;
        ++i;
        delay(300);
        stateStartTime = currentTime;
      }
      else if (!buttonState_4) {
        code[i] = 4;
        ++i;
        delay(300);
        stateStartTime = currentTime;
      }

      if (i == 4) {
        stateDoor = OPEN;  
        stateStartTime = currentTime;
      }

      break;

      case OPEN:
        
        if (code[0] == pass[0] && code[1] == pass[1] && code[2] == pass[2] && code[3] == pass[3] && stateDuration <= 10000) {
          digitalWrite(RELAY_PIN, LOW);
          digitalWrite(LED_PIN, (currentTime / 500) % 2);
        }
        else {
          i = 0;
          stateDoor = CLOSE;
          Serial.println("Incorrect code");
        }

        break;  

      case DAY:

        digitalWrite(RELAY_PIN, LOW);
        digitalWrite(LED_PIN, LOW);
        stateDoor = CLOSE;

        break;
  }
}
