uint8_t Alarma1=HIGH;
uint8_t Alarma2=HIGH;
uint8_t Alarma3=HIGH;
uint8_t Alarma4=HIGH;


void setup() {
  pinMode(btn_1,INPUT_PULLUP);
  pinMode(btn_2,INPUT_PULLUP);
  pinMode(btn_3,INPUT_PULLUP);
  pinMode(btn_4,INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt((btn_1),Fuego,FALLING);
  attachInterrupt(digitalPinToInterrupt((btn_2),Moto_sierra,FALLING);
  attachInterrupt(digitalPinToInterrupt((btn_3),Disparo,FALLING);
  attachInterrupt(digitalPinToInterrupt((btn_4),ADC,FALLING);
  
}

void loop() {
  
}
void Fuego() {
  
}
void Moto_sierra() {
  
}
void Disparo() {
  
}
void ADC() {
  
}
