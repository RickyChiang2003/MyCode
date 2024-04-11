#define button D6

bool prev_b = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(button, INPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool b = digitalRead(button);
  if(prev_b != b && b){
    Serial.print("b\n");
  }
  prev_b = b;
  
  delay(10);
}
