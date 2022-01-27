#include <SPI.h>
#include <SD.h>

File myFile;
int potPin = A5;
unsigned long startTime = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("card initialized!");
  delay(3000);
  startTime = millis();
}


void loop() {
  int sensor = analogRead(potPin);
  File myFile = SD.open("datalog1.txt", FILE_WRITE);

  if (myFile){
    Serial.println(millis());
    double currentTime = (millis() - startTime)/1000.0;
    myFile.print(currentTime);
    myFile.print(", ");
    myFile.println(sensor);
    myFile.close();
    Serial.print(currentTime);
    Serial.print(", ");
    Serial.println(sensor);
  }else{
    Serial.println("ERROR OPENING DATALOG.TXT");
  }
  
}
