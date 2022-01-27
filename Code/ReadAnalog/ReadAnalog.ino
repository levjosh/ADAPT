int emgPin = A5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(2000000);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorVal = analogRead(emgPin);
  Serial.print(float(millis()/1000.0));
  Serial.print(",");
  Serial.println(sensorVal);
}
