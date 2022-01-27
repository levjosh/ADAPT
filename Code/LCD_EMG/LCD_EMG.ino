#include <LiquidCrystal.h>
#include <TimerEvent.h>

LiquidCrystal lcd (12, 11, 5, 4, 3, 2);

TimerEvent lcd_thread;

int bicep_emg_pin = A0;
int tricep_emg_pin = A4;
unsigned long startTime;

void clearRowSection(int startColumn, int endColumn, int row) {
  lcd.setCursor(startColumn, row);
  String clearingSpace = "";
  for (int i = startColumn; i < endColumn; i++) {
    clearingSpace.concat(" ");
  }
  lcd.print(clearingSpace);
  lcd.setCursor(startColumn, row);
}

void setup() {
  Serial.begin(2000000);
  Serial.setTimeout(50);
  delay(200);
  
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Hello");

  lcd_thread.set(50, timer);

  startTime = millis();

}

void loop() {
  // Get EMG values
  int bicep_val = analogRead(bicep_emg_pin);
//  int tricep_val = analogRead(tricep_emg_pin);

  // Update timer on LCD screen every 1 second
  lcd_thread.update();

  // Print time and EMG values to serial monitor
  Serial.print((millis() - startTime)/1000.0);
  Serial.print(",");
  Serial.println(bicep_val);
//  Serial.print(",");
//  Serial.println(tricep_val);
}




void timer(){
  clearRowSection(0, 15, 1);
  double currentTime = millis()/1000.0;
  lcd.print(currentTime);


  lcd.setCursor(0, 0);
  if (currentTime <= 5.0){
    clearRowSection(0, 15, 0);
    lcd.print("Rest...");
  }
  else if (currentTime > 5.0 && currentTime <= 10.0){
    clearRowSection(0, 15, 0);
    lcd.print("Flex: ");
    int flexPower = (currentTime - 5) + 1;
    lcd.print(flexPower);    
  }
  else if (currentTime > 10.0 && currentTime <= 15.0){
    clearRowSection(0, 15, 0);
    lcd.print("Flex HARD");
  }
  else if (currentTime > 15.0 && currentTime <= 20.0){
    clearRowSection(0, 15, 0);
    lcd.print("Rest...");
  }
  else if (currentTime > 20.0 && currentTime <= 25.0){
    clearRowSection(0, 15, 0);
    lcd.print("Flex: ");
    int flexPower = (currentTime - 20) + 1;
    lcd.print(flexPower);    
  }
  else if (currentTime > 25.0 && currentTime <= 30.0){
    clearRowSection(0, 15, 0);
    lcd.print("Flex HARD");
  }
  else{
    clearRowSection(0, 15, 0);
    lcd.print("Relax");
    lcd_thread.disable();
  }
}
