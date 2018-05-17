
#include <SPI.h>
#include <RH_RF69.h>
#define RF69_FREQ 915.0
#if defined (__AVR_ATmega32U4__) // Feather 32u4 w/Radio
  #define RFM69_CS      8 //configuracion de los puertos
  #define RFM69_INT     7
  #define RFM69_RST     4
  #define LED           13
#endif

RH_RF69 rf69(RFM69_CS, RFM69_INT);
int16_t packetnum = 0;  //numero de paquetes que se envian
void setup() 
{
  Serial.begin(115200); // velocidad de baudios
  pinMode(LED, OUTPUT);     //led para mostrar comunicacion
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);
  Serial.println("Feather RFM69 RX Test!"); //para el serial ploter
  Serial.println();
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);
  if (!rf69.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }
  rf69.setTxPower(20, true);  // define el rango va de 14-20 el true debe estar siempre activo en el hwc
 // esto es como una encriptacion (no entiendo para que sirva)
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
  pinMode(LED, OUTPUT);//vuelven a definir el led
  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz"); // para el serie
}

void loop() {
 if (rf69.available()) {
    // Should be a message for us now   
    uint8_t buf[RH_RF69_MAX_MESSAGE_LEN]; // asi capta el mensaje
    uint8_t len = sizeof(buf); // len es el tamaño del mensaje
    if (rf69.recv(buf, &len)) { 
      if (!len) return;
      buf[len] = 0;
      Serial.print("Received [");// para el serial 
      Serial.print(len);// tamaño del mensaje
      Serial.print("]: ");// para el serial
      Serial.println((char*)buf);// asi imprimen el mensaje al serial
      Serial.print("RSSI: ");// para el serial
      Serial.println(rf69.lastRssi(), DEC);// esta es ka señal que capta el rx la intensidad

      if (strstr((char *)buf, "Hello World")) {
// esta parte del codigo es para trabajar las respuestas
        uint8_t data[] = "And hello back to you"; // este es un mensaje que envian
        rf69.send(data, sizeof(data)); // asi se envia un dato
        rf69.waitPacketSent();
        Serial.println("Sent a reply");
        Blink(LED, 40, 3); //blink LED 3 times, 40ms between blinks
      }
    } else {
      Serial.println("Receive failed");
    }
  }
}

// esto de aca abajo es para medir el PING de la trama pero tampoco lo entiendo muy bien
void Blink(byte PIN, byte DELAY_MS, byte loops) {
  for (byte i=0; i<loops; i++)  {
    digitalWrite(PIN,HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN,LOW);
    delay(DELAY_MS);
  }
}
