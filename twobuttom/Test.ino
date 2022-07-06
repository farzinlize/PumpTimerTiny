#define ledTimerPin 1
#define buttonTimerPin 4
#define relayPin 2
#define debounce_count 10

int relayValue = LOW;
int timerValue = LOW;
int timerButtonRead = LOW;
int timerDebouncer = 0;
int timerButtonState = LOW;
bool timerRun = false;
unsigned long startTime;
unsigned long lastTime;
unsigned long threshold = 1800000;  //30 minutes (30min * 60sec * 1000millsec)

void setup() {
  pinMode(ledTimerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(buttonTimerPin, INPUT_PULLUP);

  FM_MORSE(0);  //signuture

  digitalWrite(ledTimerPin, timerValue);
  digitalWrite(relayPin, relayValue);

  //cli();
  GIMSK &= ~(1<<PCIE);

  lastTime = millis();
}

void loop() {

  if(millis() != lastTime){
    timerButtonRead = !digitalRead(buttonTimerPin);
  
    if(timerButtonRead == timerButtonState && timerDebouncer > 0){
      timerDebouncer--;
    }
    if(timerButtonRead != timerButtonState){
      timerDebouncer++;
    }
    if(timerDebouncer >= debounce_count){
      timerDebouncer = 0;
      timerButtonState = timerButtonRead;
      if(timerButtonState == HIGH){
        timerButton();
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
  digitalWrite(relayPin, relayValue);
}

void timerButton(){
  if(timerRun){
    relayValue = LOW;
    timerValue = LOW;
  }else {
    startTime = millis();
    timerValue = HIGH;
    relayValue = HIGH;
  }
  timerRun = !timerRun;
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
