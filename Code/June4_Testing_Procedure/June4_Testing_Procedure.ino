#include <SPI.h>
#include <SD.h>
#include <Pushbutton.h>

#define BUTTON_PIN 14

int analogPin = A5;
unsigned long startTime = 0;


// LED RELATED VARIABLES
Pushbutton NXT_STP(BUTTON_PIN);

bool recording = false;
int recordingCount = 1;
bool activateRise = true;
bool activateHold = false;

unsigned long timeSet = 0;
unsigned long timeBetweenTest = 5000;


// Blink when waiting for button and hold for 5 seconds prior to rise
int start_LED = 3;
bool start_LED_state = false;   

// Activate on "slowly increase muscle pressure over the next 5 seconds"
int rise_LED [] = {4,5,6,7,8};

// Activate on "hold muscle
int hold_LED = 9;


// SIMPLE FUNCTION
void writePinsLow(){
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
}

// --------------------------------------- SETUP PHASE -------------------------------------------

void setup() {
  Serial.begin(2000000);
  // ----------------------------- WAIT FOR BUTTON PRESS TO START --------------------------------
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  // Rest arm  before testing. Press button to begin testing.
  // Blinking LED means we are waiting for button to be pressed
  timeSet = millis();
  while (!NXT_STP.getSingleDebouncedRelease()){
    if(millis() - timeSet >= 1000){
      start_LED_state = !start_LED_state;
      digitalWrite(start_LED, start_LED_state);
      timeSet = millis();
    }
  }
  digitalWrite(start_LED, HIGH);
  recording = true;
  startTime = millis();
  
}

void loop() { 
  // ------------------------------- START RECORDING DATA HERE -----------------------------------
  timeSet = millis();
  
  while (recording == true){
    int sensorVal = analogRead(analogPin);
    if (sensorVal >= 0){
      double currentTime = (millis() - startTime)/1000.0;
      Serial.print(currentTime);
      Serial.print(", ");
      Serial.println(sensorVal);
    }else{
      Serial.println("ERROR WITH SENSOR VAL");
    }

    // Sequentially activate LEDs over 5 seconds  
    if (activateRise == true){
      if(millis() - timeSet <= 5000){
         digitalWrite(start_LED, HIGH);
      }else if(millis() - timeSet > 5000 && millis() - timeSet <= 10050){
        if (millis() - timeSet - 5000 > 1000){
          int pin = (millis() - timeSet - 5000)/1000;
          digitalWrite(rise_LED[pin - 1], HIGH);
        } 
      }else if (millis() - timeSet > 10050){
        activateRise = false;
        activateHold = true;
        timeSet = millis();
      }
    }
    
    // Activate hold LED for the next 5 seconds
    if (activateHold == true){
      if (millis() - timeSet <= 5000){
        if (millis() - timeSet > 1000){
          digitalWrite(hold_LED, HIGH);  
        }
      }else if (millis() - timeSet > 5000){
        writePinsLow();
        activateHold = false;
        timeSet = millis();
      }
    }

    // Turn off all lights and wait to re-test
    if (activateRise == false && activateHold == false){
      if (recordingCount == 2){ 
        recording = false;     
      }else{
        if (millis() - timeSet <= timeBetweenTest){
          writePinsLow();
        }else{
          recordingCount++;
          activateRise = true;
          timeSet = millis();
        }
      }
    }
  }
  
  // Just keep delaying while waiting
  delay(1000);    
    
  
  

}
