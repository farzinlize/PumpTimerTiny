#define ledTimerPin 1
#define ledActivePin 0
#define buttonTimerPin 4
#define buttonActivePin 3
#define relayPin 2
#define debounce_count 10

int relayValue = LOW;
int timerValue = LOW;
int activeValue = LOW;
int currentTimerValue = LOW;
int currentActiveValue = LOW;
int counterTimer = 0;
int counterActive = 0;
int timerButtonState = LOW;
int activeButtonState = LOW;
bool timerRun = false;
unsigned long startTime;
unsigned long lastTime;
unsigned long threshold = 1800000;  //30 minutes (30min * 60sec * 1000millsec)

void setup() {
  pinMode(ledTimerPin, OUTPUT);
  pinMode(ledActivePin, OUTPUT);
  pinMode(buttonTimerPin, INPUT_PULLUP);
  pinMode(buttonActivePin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);

  FM_MORSE(0);  //signuture

  digitalWrite(ledTimerPin, timerValue);
  digitalWrite(ledActivePin, activeValue);
  digitalWrite(relayPin, relayValue);

  //cli();
  GIMSK &= ~(1<<PCIE);

  lastTime = millis();
}

void loop() {
  if(millis() != lastTime){
    currentTimerValue = !digitalRead(buttonTimerPin);
  
    if(currentTimerValue == timerButtonState && counterTimer > 0){
      counterTimer--;
    }
    if(currentTimerValue != timerButtonState){
      counterTimer++;
    }
    if(counterTimer >= debounce_count){
      counterTimer = 0;
      timerButtonState = currentTimerValue;
      if(timerButtonState == HIGH){
        timerButton();
      }
    }
    
    currentActiveValue = !digitalRead(buttonActivePin);

    if(currentActiveValue == activeButtonState && counterActive > 0){
      counterActive--;
    }
    if(currentActiveValue != activeButtonState){
      counterActive++;
    }
    if(counterActive >= debounce_count){
      counterActive = 0;
      activeButtonState = currentActiveValue;
      if(activeButtonState == HIGH){
        activeButton();
      }
    }

    lastTime = millis();
  
    if(timerRun && ((unsigned long)(lastTime - startTime) >= threshold)){
      relayValue = LOW;
      timerValue = LOW;
      timerRun = false;
    }
  }

  digitalWrite(ledTimerPin, timerValue);
  digitalWrite(ledActivePin, activeValue);
  digitalWrite(relayPin, relayValue);
}

void timerButton(){
  if(timerRun){
    relayValue = LOW;
    timerValue = LOW;
  }else {
    if(activeValue == HIGH){
      activeValue = LOW;
    }
    startTime = millis();
    timerValue = HIGH;
    relayValue = HIGH;
    }
  timerRun = !timerRun;
}

void activeButton(){
  if(timerRun){
    relayValue = LOW;
    timerValue = LOW;
    timerRun = false;
  }
  activeValue = !activeValue;
  relayValue = !relayValue;
}

void FM_MORSE(int morsePin) {
  //Farzin Mohammadi Signuture
  digitalWrite(morsePin, HIGH);
  delay(200);           
  digitalWrite(morsePin, LOW); 
  delay(200);           //dot
  
  digitalWrite(morsePin, HIGH);
  delay(200);           
  digitalWrite(morsePin, LOW); 
  delay(200);           //dot
  
  digitalWrite(morsePin, HIGH);
  delay(600);           
  digitalWrite(morsePin, LOW); 
  delay(200);           //dash

  digitalWrite(morsePin, HIGH);
  delay(200);           
  digitalWrite(morsePin, LOW); 
  delay(1400);          //dot (F)

  digitalWrite(morsePin, HIGH);
  delay(600);
  digitalWrite(morsePin, LOW);
  delay(200);           //dash

  digitalWrite(morsePin, HIGH);
  delay(600);
  digitalWrite(morsePin, LOW);
  delay(1400);          //dash (M)
}
