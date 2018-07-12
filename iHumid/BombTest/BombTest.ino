int bomb = 7;
int cont = 1;
void setup() {
  // put your setup code here, to run once:

  pinMode(bomb, OUTPUT);
}

void loop() {

      Serial.println("Bomba encendida");
      digitalWrite(bomb, HIGH);
      delay(1000);
      digitalWrite(bomb, LOW);
  /*if(cont == 1){
      digitalWrite(bomb, HIGH);
      Serial.println("encendida");
      delay(5000); 
      digitalWrite(bomb, LOW);
      Serial.println("apagada");
      cont++;
  }*/ 

}
