#define button 7

void setup() {
  //Serial.begin(9600);
  //pinMode(button, INPUT);
  //pinMode(13, OUTPUT);
  Serial.begin(115200);
  pingMode(D6, INPUT);
}

void loop() {
  //bool b = digitalRead(button);
  //if(b) digitalWrite(13, HIGH);
  //else digitalWrite(13, LOW);
  bool b = digitalRead(D6);
  Serial.println(b);
}
