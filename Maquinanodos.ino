//defines e includes
#include <SPI.h>
#include <RH_RF69.h>
#include "LowPower.h"
#define RF69_FREQ 915.0


#if defined (__AVR_ATmega32U4__) // Feather 32u4 w/Radio
#define RFM69_CS      8 //configuracion de los puertos
#define RFM69_INT     7
#define RFM69_RST     4
#define LED           13
#endif


uint8_t Alarma1=HIGH;
uint8_t Alarma2=HIGH;
uint8_t Alarma3=HIGH;
uint8_t Alarma4=HIGH;
char Pila_alarmas[2];

int btn_1=0;
int btn_2=1;
int btn_3=2;
int btn_4=3;
int borrado;


/*variables para estados*/
int alarma = 0;
int mio = 0;
int alarma_pila = 0;
int tramas_pila = 0;
int T = 20;
int NumT = 0;
int RTS_reci = 0;
int CTS_env = 0;
int rec_alarma = 0;
int CHECK = 0;
int Correcion = 0;
int Res_peticion_trama = 0;
int PT = 0;
int RTS_env = 0;
int CTS_reci = 0;
int Trama_ack = 0;
int cont = 0;
int Estado = 0;
int level_ADM = 255;
int start;  ////CAMBIADO PORQUE NO SE PUEDE IGUALAR A MILLIS TODAVIA, YA QUE TOMA EN CUENTA EL TIEMPO QUE CORRE EL ARDUINO.
int thread_level;
int x, y;
int MCL = 3;
int level_AMD;
bool cluster = 0;
int conth = 0;
int alarm = 0;
int conta = 0;
int start1 = millis();
int NIVEL_ADM=0;
int NIVEL_AMD;
//escucha
int tiempoenmilis;
int MAC_destinatario;
int MAC_emisor;
int NIVEL_ADM_emisor;
int CHECK_SUM_recibido;
int CHECK_SUM_enviado;
int mace;
int pte;
int ctse;
int rtse;
int acke;
int alarmae;



RH_RF69 rf69(RFM69_CS, RFM69_INT);
int16_t packetnum = 0;  //numero de paquetes que se envian
int MAC_destinario;
/*COMPRESION*/
int XC = 0;
int LC = 0;
int TC = 0;
int CC = 0;
char stringc[999999];
int cambioc1 = 0;
int cambioc2 = 0;
int Alarmac;
int NODOC;
int TIPOC;
int MAC_local;
/*PETICION TRAMA*/

int B = 5;
uint8_t PT1[4];
char CTS[5];
char RTS[5] ;
char ACK_IN[3] ;
char ACK_OUT[3] ;
char Malarma[1];
char TRAMA[1]; // trama de alarma
int contador_p = {0};
int mac=24;
int CHECK_SUM;

int checksum_trama;
int len_trama;
int pos_trama;


enum state { primera_fase, segunda_fase, escuchar, verificacion };
enum state current_state = primera_fase;


/* Transferencia */
char data[5];
int ID_NODE = {10}; // ID del nodo actual
int ID_NODE_IN = {20}; // ID del nodo escuchado
int contador_pr = {0}; // contador del primer estado de espera
int contador_s = {0}; // contador del segundo estado de espera
int contador_t = {0}; // contador del tercer estado de espera
int contador_c = {0}; // contador del cuarto estado de espera

enum state_st {primer_estado, segundo_estado, tercer_estado, cuarto_estado};
enum state_st current_state_st = primer_estado;


uint8_t buf[RH_RF69_MAX_MESSAGE_LEN]; // asi capta el mensaje
uint8_t len = sizeof(buf); // len es el tamaÃ±o del mensaje


int BASE=0;



/*COMPRESION*/
void setup() {
  Serial.begin(115200);
  
  pinMode(btn_1,INPUT_PULLUP);
  pinMode(btn_2,INPUT_PULLUP);
  pinMode(btn_3,INPUT_PULLUP);
  pinMode(btn_4,INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt((btn_1)),Fuego,FALLING);
  attachInterrupt(digitalPinToInterrupt((btn_2)),Moto_sierra,FALLING);
  attachInterrupt(digitalPinToInterrupt((btn_3)),Disparo,FALLING);
  attachInterrupt(digitalPinToInterrupt((btn_4)),AD,FALLING);


  
  while (!Serial) ; // wait for Arduino Serial Monitor (native USB boards)
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
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
                  };
  rf69.setEncryptionKey(key);
  pinMode(LED, OUTPUT);//vuelven a definir el led
  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz"); // para el serie*/
}

/* Estados
  hibernacion=0
  escuchapt=1
  clusterdefinicion=2
  primera fase de peticion de trama=3
  segunda fase de peticion de trama=4
  escucha=5
  verificacion=6
  guardar alama en pila=7
  alarmas en la pila=8
  hay tramas por transmitir=9
  comprimir alarmas y enviar siguiente salto de amd=10
  primer estado de espera=11
  segundo estado de esepera=12
  tercer estado de espera=13
  cuarto estado de espera =14
  se conprimen las alarmas  se enmvian hacia el siguente estado de ADM=15
*/


void loop() {

if (BASE==0){
  switch (Estado)
  {
    case 0:
      Serial.print("hola \n");
      hibernacion();
      Serial.print(Estado);
      break;
    case 1:
      nivel();
      Estado = 0;
      Serial.print(Estado);
      break;
    case 2:
      Estado = 0;
      Serial.print("aquino");
      break;
    case 3:
      peticion_trama();
      Serial.print(Estado);
      break;
    case 4:
     peticion_trama();
     Serial.print(Estado);
      break;
    case 5:
      peticion_trama();
      Serial.print(Estado);
      break;
    case 6:
      peticion_trama();
      Serial.print(Estado);
      break;
    case 7:
      Estado = 3;
      Serial.print(Estado);
      break;
    case 8:
      if (alarma_pila == 1)
      {
        Estado = 9;
        Serial.print(Estado);
      }
      if (alarma_pila == 0)
      {
        Estado = 0;
        Serial.print(Estado);
      }
      break;
    case 9:
      if (tramas_pila == 1)
      {
        Estado = 15;
        Serial.print(Estado);
      }
      if (tramas_pila == 0)
      {
        Estado = 10;
        Serial.print(Estado);
      }
      break;
    case 10:
      Estado = 11;
      Serial.print(Estado);
    case 11:
    prim_estado();
    Serial.print(Estado);
      break;
    case 12:
    seg_estado();
    Serial.print(Estado);
      break;
    case 13:
    terc_estado();
    Serial.print(Estado);
    break;
    case 14:
    cuar_estado();
    Serial.print(Estado);
      break;
    case 15:
      Estado = 11;
      Serial.print(Estado);
      break;


  }
}

else if(BASE==1){
  Estado=3;
  switch (Estado)
  {
    case 3:
      peticion_trama();
      Serial.print(Estado);
      break;
    case 4:
     peticion_trama();
     Serial.print(Estado);
      break;
    case 5:
      peticion_trama();
      Serial.print(Estado);
      break;
   

  }
  
  
  
  }
}
/*Nivel de Pertenencia ADM*/
int descubrimiendo_nivel_ADM(){
  escucha(2*B);
  int NIVEL_ESCUCHADO = TRAMA[2];
  if (pte != 0){ // si se escucha una trama PT
         // while (millis() < start + 600) {
            if (NIVEL_ESCUCHADO < NIVEL_ADM) {
              NIVEL_ADM = NIVEL_ESCUCHADO+1;
              
            }
          }

   else{
          Estado=0;//Regreso al estado de HIBERNACIÓN
          return;
   }
}

void nivel()
{
  start = millis();
  while (millis() < start + 600) {
    if (thread_level < NIVEL_ADM) {
      NIVEL_ADM = thread_level;
    }
  }
  NIVEL_ADM++;
  /*Nivel de Pertenencia AMD*/
  x = NIVEL_ADM % (MCL * 2);
  y = (MCL * 2) - x;

  if (x < y)
    NIVEL_AMD = x;
  else
    NIVEL_AMD = y;
  if (x == 0)
    cluster = 1;
}

void hibernacion() {


  // put your main code here, to run repeatedly:
  
  LowPower.idle(SLEEP_250MS, ADC_ON, TIMER4_ON, TIMER3_ON, TIMER1_ON, TIMER0_ON, SPI_OFF, USART1_ON, TWI_ON, USB_ON);
  Serial.print("a");
       

  if (alarma == 0)
  {
    if (conta >= 20)
    {
      Estado=1;
      conta=0;
      return;
    }
    if (conta <= 20)
    {
      conta = conta + 1;
      Estado=0;   ///Aqui es Estado => Primer Estado de Peticion de Trama (3)
    }
  }
  if (alarma != 0)
  {
    Estado=3;     ///Estado => Primer Estado de Espera (11)
    return;
  }

}

void escucha(int tiempoenmilis)  {
  if (rf69.available()) {

    while (millis() <= (start1 + tiempoenmilis)) {
      uint8_t buf[RH_RF69_MAX_MESSAGE_LEN]; // asi capta el mensaje
      uint8_t len = sizeof(buf); // len es el tamaÃ±o del mensaje
      if (rf69.recv(buf, &len)) {
        if (!len) return;
        buf[len] = 0;
        Serial.print("Received [");// para el serial
        Serial.print(len);// tamaÃ±o del mensaje
        Serial.print("]: ");// para el serial
        Serial.println((char*)buf);// asi imprimen el mensaje al serial
        Serial.print("RSSI: ");// para el serial
        Serial.println(rf69.lastRssi(), DEC);// esta es ka seÃ±al que capta el rx la intensidad
        if (buf[0] == 243 && buf[4] == mace) {
          pte = 0;
          ctse = 1;
          rtse = 0;
          alarmae = 0;
          acke = 0;
          mio = 1;
        }
        if (buf[0] == 243 && buf[4] != mace) {
          pte = 0;
          ctse = 1;
          rtse = 0;
          alarmae = 0;
          acke = 0;
          mio = 0;
        }

        if (buf[0] == 241) {
          pte = 1;
          ctse = 0;
          rtse = 0;
          alarmae = 0;
          acke = 0;
        }
        if (buf[0] == 242) {
          pte = 0;
          ctse = 0;
          rtse = 1;
          acke = 0;
          alarmae = 0;
        }
        if (buf[0] == 244) {
          pte = 0;
          ctse = 0;
          rtse = 0;
          acke = 0;
          alarmae = 1;
        }
        if (buf[0] == 245) {
          pte = 0;
          ctse = 0;
          rtse = 0;
          acke = 1;
          alarmae = 0;
        }
      }
    }
  }
  else {
    Serial.println("Receive failed");
    Estado = 0;
    return;
  }
}


/*compresion de datos*/

void compresion() {
  int XC = 0;
  int LC = 0;
  int TC = 0;
  int CC = 0;
  char stringc[999999];
  int cambioc1 = 0;
  int cambioc2 = 0;
  int Alarmac;
  int NODOC;
  int TIPOC;
  while (XC != LC) {
    if (stringc[XC] == Alarmac) {
      stringc[XC + 1] = stringc[XC + 1] + 1;
      cambioc1 = stringc[XC + 2];
      for (int i = XC + 2; i <= LC + 1; i++) {
        cambioc2 = stringc[i + 1];
        stringc[i + 1] = cambioc1;
        cambioc1 = cambioc2;
      }
      stringc[XC + 2] = NODOC;
    }
    else {
      stringc[0] = cambioc1;
      for (int i = 0; i <= 2; i++) {
        cambioc2 = stringc[i + 1];
        stringc[i + 1] = cambioc1;
        cambioc1 = cambioc2;
      }
      stringc[0] = TIPOC;
      stringc[1] = 1;
      stringc[2] = NODOC;

    }
  }
}


void gen_ACK(char* TRAMA){
  if (TRAMA != 0){
    pos_trama = len_trama - 1;
    checksum_trama = 0;    
    while (pos_trama >= 0){
      checksum_trama= TRAMA[pos_trama] + checksum_trama;
      pos_trama = pos_trama - 1;
      }
  } 
  return;
}

int peticion_trama()
{
  
    switch (current_state)
    {
      case primera_fase:
        {
          escucha(2 * B);
          if (ctse == 0 or rtse == 0) // si CTS o RTS estan activos ¿HAY QUE AGREGAR PT?
          {
            PT1[0] = 0xF0;
            PT1[1] = 0x30;
            PT1[2] = NIVEL_ADM;
            PT1[3] = MAC_local;
            rf69.send(PT1, sizeof(PT1)); // asi se envia un dato
            //oPT = 1; ///enviar PT, send rx.
            escucha(2*B);
            if (rtse != 0){
              current_state = segunda_fase;
              Estado=4;
              return;
            }
            else if(BASE==0) {
            current_state = primera_fase;
            Estado=0;
            return;   
            }
               
        }
        else{
          if (alarma==0 && BASE==0){
            current_state = primera_fase;
            Estado=0;
            return;
          }
          else{
            if (BASE==0){
            current_state = primera_fase; // va a primera fase de peticion de trama, no de espera
            Estado=3;
            }
            else{
              current_state = primera_fase;
              Estado=0;}
            return;
            }
        }
        }

          
      case segunda_fase:
        {
            CTS[0] = 243;
            CTS[1] = NIVEL_AMD;
            CTS[2] = NIVEL_ADM;
            CTS[3] = TRAMA[3]; //Reenvia el tamaño de trama que recibió en el RTS
            CTS[4] = TRAMA[4];
            MAC_destinatario = TRAMA[4];
            rf69.send(CTS, sizeof(CTS));
            current_state = escuchar;
            Estado=5;
            return;
        }
      case escuchar:
        {
          escucha(2*B); //tiempo de espera 2B
          if (alarmae!= 0 && BASE==0) {
            current_state = verificacion;
            Estado=6;
            return;
          }
          else  {
            current_state = primera_fase;
            Estado=3;
            if(BASE==0){
            current_state = primera_fase;
            Estado=3;
            }
            return;
          }
        }
      case verificacion:
        {
          gen_ACK(TRAMA);
          ACK_OUT[0] = 245;
          ACK_OUT[1] = checksum_trama;
          ACK_OUT[2] = MAC_destinario; //REVISAR si es la MAC local o la del destinatario??
          escucha(2*B); //tiempo de espera 2B
          if (alarmae == 0){
            borrado=0;
            pila_alarmas();
            current_state = primera_fase;
            Estado=3;
            return;
          }
          else { 
            current_state = verificacion;
            Estado=6;
            return;
          }
        }
    }
  }



void prim_estado(){
 if (ctse != 0 or rtse != 0){ // si CTS o RTS estan activos
            delay(10); // tiempo de espera para trasmisión
            Estado = 11; //Primer estado de la maquina 
            return;
        }
       else if (pte != 0){
           //Acá hay que separar la trama PT para sacar el ID_NODE_IN para compararlo con el ID_NODE
           MAC_emisor=TRAMA[4];
           NIVEL_ADM_emisor = TRAMA[3];
         
           if (NIVEL_ADM_emisor == NIVEL_ADM-1) // si el ID del PT escuchado es menor al ID del nodo reconfiguración{
           { 
             Estado = 12;
             return;// Cambio de nodo si se escucha uno menor
           }
       }
       return; 
}

void seg_estado(){
   delay((rand() % 9 + 2)*B) ; // espera un tiempo aleatorio
      escucha(2*B);
      if (ctse !=0){

        if(MAC_local==TRAMA[5]){ // Se compara la MAC_local con la MAC_del_destinatario para saber si el CTS es para el nodo.
            if (contador_s == 3){
              Estado=0;//Regreso al estado de HIBERNACIÓN
              contador_s = 0;
              return;
            }
            else{
              contador_s = contador_s +1;
              Estado = 11;
              return;
            }
        }
      }
      else {
        //Se envia la trama RTS
        contador_s = 0;
        Estado = 13;
        return;
      }
}

void terc_estado(){
  if (ctse != 0) {
        if (TRAMA[5]==MAC_local){//Se compara la MAC local con la MAC_del_destinatario en la CTS
            Estado = 14;
            contador_t = 0;
            return;
          }
      }
      else if (ctse == 0) {
        if (contador_t == 3) {
          Estado=0;// Pasa a modo Hibernación
          contador_t = 0;
          return;
        }
        else {
          contador_t = contador_t +1;
          Estado = 11;
          return;
        }
      } 
}

void cuar_estado(){
  
 char data [3] ; // Esta es la TRAMA_enviada de alarma que se envía
      data[0]=245;
      data[1]=TRAMA[len-1];
      data[2]=MAC_emisor;
        rf69.send(data, sizeof(data)); // asi se envia un dato 
      // CHECK_SUM_enviado=TRAMA_enviada[6] //Se guarda el check_sum enviado
      delay(10);//Escucha por una trama ACK
      if (acke != 0){
         CHECK_SUM_recibido=TRAMA[2];
         if (CHECK_SUM_recibido==CHECK_SUM_enviado){
         borrado=1;
         pila_alarmas();
         borrado=0;
         Estado=0;// pasa a HIBERNACIÓN
         }
        else {
         Estado = 14;
         return;
        }
      }
      else {
        Estado=0;
        return;
        //Pasa a hibernacion
       } 
}

void Fuego() {
  Serial.print("fuego");
  Pila_alarmas[0]=4;
  Pila_alarmas[1]=NIVEL_ADM;
  alarma=1;
}
void Moto_sierra() {
  Serial.print("sierra");
  Pila_alarmas[0]=2;
  Pila_alarmas[1]=NIVEL_ADM;
  alarma=1;
}
void Disparo() {
  Serial.print("Disparo");
  Pila_alarmas[0]=1;
  Pila_alarmas[1]=NIVEL_ADM;
  alarma=1;
}
void AD() {
  Serial.print("ad");
  Pila_alarmas[0]=5;
  Pila_alarmas[1]=NIVEL_ADM;
  alarma=1;
}
void pila_alarmas() {
  if (borrado==1){
  Pila_alarmas[0]=0;
  Pila_alarmas[1]=0;
  alarma=0;
  }
  else {
    Pila_alarmas[0]=TRAMA[1];
    Pila_alarmas[1]=TRAMA[2];
    alarma=1;
    
  }
}

