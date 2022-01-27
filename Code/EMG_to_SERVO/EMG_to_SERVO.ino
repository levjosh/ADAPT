#include <Servo.h>
#include <movingAvg.h>



int SENSOR_PIN = A5;
int SERVO_PIN = 9;

int movingAvg_interval = 240;
int emgBotVal = 15;
int emgTopVal = 50;

unsigned long startMillis;
unsigned long currentMillis;
int servoFunctionPeriod = 25;

int encodingStep = 0;

// Moving Average Setup
movingAvg rawMAV(movingAvg_interval);
movingAvg rectMAV(movingAvg_interval);

// Servo Setup
Servo myServo;
int servoBotLimit = 00;
int servoTopLimit = 30;
int servoPosition = servoBotLimit;


// -------------------------------------- SETUP -------------------------------------
void setup() {
  Serial.begin(2000000);
  rawMAV.begin();
  rectMAV.begin();
  myServo.attach(SERVO_PIN);
  myServo.write(servoTopLimit);          // Set initial servo position to 0
  
  startMillis = millis();
}




// --------------------------------------- LOOP -------------------------------------
void loop() {
  currentMillis = millis();

  int emgData = analogRead(SENSOR_PIN);
  int emgMAV = rawMAV.reading(emgData);
  int emgRectified = abs(emgData - emgMAV);
  int emgRectifiedMAV = rectMAV.reading(emgRectified);


  Serial.print(millis()/1000.0);
  Serial.print(",");
  Serial.println(emgData);
//  Serial.print(",");
//  Serial.println(emgRectifiedMAV);



  // Adjust servo every 'servoFunctionPeriod'
  if (currentMillis - startMillis >= servoFunctionPeriod){

    // Determine step size
    if (emgRectifiedMAV < emgBotVal){
      encodingStep = -1;
    }
    else if (emgRectifiedMAV >= emgBotVal && emgRectifiedMAV <= emgTopVal){
      encodingStep = map(emgRectifiedMAV, emgBotVal, emgTopVal, 0, 1);
    }
    else{
      encodingStep = 1;
    }


//    Serial.print(emgRectifiedMAV);
//    Serial.print(", ");    
//    Serial.print(servoPosition);
//    Serial.print(", ");
//    Serial.println(encodingStep);
    
    // Incorporate step moving into servo movement
    if (servoPosition >= servoBotLimit && servoPosition <= servoTopLimit){
      Serial.println("Stage 1");
      servoPosition = servoPosition - encodingStep;
      myServo.write(servoPosition);
    }
    else if (servoPosition < servoBotLimit){
      Serial.println("Stage 2");
      servoPosition = servoPosition + 1;
      myServo.write(servoPosition);
    }
    else if (servoPosition > servoTopLimit){
      Serial.println("Stage 3");
      servoPosition = servoPosition - 1;
      myServo.write(servoPosition);
    }
    else{
//      Serial.println("what?");
    }

    
    startMillis = millis();
  }


  
}
