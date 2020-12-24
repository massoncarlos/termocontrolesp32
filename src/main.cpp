#include <Arduino.h>
#include <WiFi.h>
#include <OneWire.h> //INCLUSÃO DE BIBLIOTECA
#include <DallasTemperature.h> //INCLUSÃO DE BIBLIOTECA.
#include <WiFiServer.h>

#define LED 2
#define DS18B20 15

// Set web server port number to 80
WiFiServer sv(80);//Cria o objeto servidor na porta 555
WiFiClient cl;//Cria o objeto cliente.

OneWire ourWire(DS18B20); //CONFIGURA UMA INSTÂNCIA ONEWIRE PARA SE COMUNICAR COM O SENSOR
DallasTemperature sensors(&ourWire); //BIBLIOTECA DallasTemperature UTILIZA A OneWire

const char* ssid = "Sensor_Temp";              	// ssid do seu roteador
const char* password = "16a16235c";		// password do seu roteador
 
// Global temperature reading
float temp;
int timer100ms;
int timer1s;
int timerReadSensor;
int temp2;
String setTemp;
String msg;


// Numero de amostras na leitura
const int nAmostras = 100;

volatile int interruptCounter;
int totalInterruptCounter;
bool enviar = false;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  timer100ms--;
  timer1s--;
  if (!timer100ms){
      timer100ms = 100;
  }
  if (!timer1s){
      timer1s = 1000;
      timerReadSensor--;
  }
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {

    timer100ms = 100;
    timer1s = 1000;
    setTemp = "3000";

	Serial.begin (115200);
	pinMode(LED, OUTPUT);

	WiFi.mode(WIFI_AP); 
	WiFi.softAP(ssid, password); 
	
	sv.begin();
	
	sensors.begin(); //INICIA O SENSOR

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000, true);
    timerAlarmEnable(timer);
	
	delay(1000); //INTERVALO DE 1 SEGUNDO
}
void tcp()//Sub rotina que verifica novos clientes e se sim, envia o HTML.
{
    if (cl.connected())//Detecta se há clientes conectados no servidor.
    {	
        if (enviar == true)
        {
            msg = "S" + setTemp + "T" + temp2 + "\n";
            cl.print(msg);
            Serial.println("enviando");
            enviar = false;
        }
        if (cl.available() > 0)//Verifica se o cliente conectado tem dados para serem lidos.
        {
            String req = "";
            while (cl.available() > 0)//Armazena cada Byte (letra/char) na String para formar a mensagem recebida.
            {
                char z = cl.read();
                req += z;
            }
            //Mostra a mensagem recebida do cliente no Serial Monitor.
            Serial.print("\nUm cliente enviou uma mensagem");
            Serial.print("\n...IP do cliente: ");
            Serial.print(cl.remoteIP());
            Serial.print("\n...IP do servidor: ");
            Serial.print(WiFi.softAPIP());
            Serial.print("\n...Mensagem do cliente: " + req + "\n");

            int req1 = req.toInt();
            Serial.println(req1);
            Serial.println(req);
            
             if (req1 >= 1000) {
                setTemp = req;
                enviar = true;
            } 
			if (req == "R1\n")
			{
                enviar = true;
			}     
        }
    }
    else//Se nao houver cliente conectado,
    {
        cl = sv.available();//Disponabiliza o servidor para o cliente se conectar.
        delay(1);
    }

}

void ligaSaida(){
   int tempInt =  setTemp.toInt();
   if (temp2 >= tempInt) {
        digitalWrite(LED, LOW);
    } else
    {
        digitalWrite(LED, HIGH);
    }
}


void readSensor(){

    if (timerReadSensor <= 0){
        sensors.requestTemperatures();
        temp = sensors.getTempCByIndex(0);
        temp2 =temp *100;
        Serial.println(temp2);
        timerReadSensor = 3;
    }
}

void loop(){

	tcp();
    ligaSaida();
    readSensor();
}


