
// hibernación Modulo

int cont=0;
#include <Adafruit_SleepyDog.h>
void setup() {
  Serial.begin(115200);
  while (!Serial) ; // wait for Arduino Serial Monitor (native USB boards)

}

void loop() {
 
  int hibernacion(int cont, int alarm)
    {// put your main code here, to run repeatedly:
      delay(1000);
      int sleepMS = Watchdog.sleep(200);
      
      if(alarm==0)
        {
          if(conta=>20)
            {
            // Mandar hacia descubrimiento de nivel de pertenencia
            }
          if conta(conta<20)
            {
             cont=cont +1;
             hibernacion(cont,alarm);
            }
        }
      if (alarm==1)
        {
          //Mandar a primera fase de peticion de trama
        }
     }
}     
