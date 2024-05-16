void setup() {
  Serial.begin(9600);
  pinMode( 7 , OUTPUT);
}

void loop() {
   if (Serial.available() > 0) { 
    int receivedValue = Serial.parseInt(); 
    Serial.print("Received value: " );
    Serial.println(receivedValue);
    if (receivedValue == 0){
         digitalWrite(7 , HIGH);
    }
    else{
      digitalWrite(7 , LOW);
    }
  }
}


