int analogPin = A5;
int pinoutArray [] = {3,4,5,6,7,8,9};
float splitValues[7];
int val = 0;

void analogToLED(int val){
  Serial.println(val);
  for (int i = 0; i < 7; i++){
    if (val > splitValues[i]){
      digitalWrite(pinoutArray[i], HIGH);
    }else{
      digitalWrite(pinoutArray[i], LOW);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  for (int i=0;i<7;i++){
    splitValues[i] = float(1024.0/7.0)*i;
  }
}

void loop() {
  val = analogRead(analogPin);
  analogToLED(val);
}
