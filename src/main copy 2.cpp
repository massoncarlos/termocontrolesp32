/*

#include <Arduino.h>
#include <WiFi.h>
#include <OneWire.h> //INCLUSÃO DE BIBLIOTECA
#include <DallasTemperature.h> //INCLUSÃO DE BIBLIOTECA

#include <WebServer.h>

#include "index.h"  //Web page header file

#define LED 2
#define DS18B20 15

// Set web server port number to 80
WebServer server(80);


OneWire ourWire(DS18B20); //CONFIGURA UMA INSTÂNCIA ONEWIRE PARA SE COMUNICAR COM O SENSOR
DallasTemperature sensors(&ourWire); //BIBLIOTECA DallasTemperature UTILIZA A OneWire

const char* ssid = "Masson";              	// ssid do seu roteador
const char* password = "16a16235c";		// password do seu roteador
 
// Global temperature reading
float temp;


// Numero de amostras na leitura
const int nAmostras = 100;

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleADC() {

	sensors.requestTemperatures();
	temp = sensors.getTempCByIndex(0);

 	String adcValue = String(temp);
 
 	server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 


void setup() {
	Serial.begin (115200);
	pinMode(LED, OUTPUT);

	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	
	// Print local IP address and start web server
	Serial.println("");
	Serial.println("WiFi conectada.");
	Serial.println("Endereço de IP: ");
	Serial.println(WiFi.localIP());

	server.on("/", handleRoot);      //This is display page
  	server.on("/readADC", handleADC);//To get update of ADC Value only

	server.begin();
	
	sensors.begin(); //INICIA O SENSOR
	
	delay(1000); //INTERVALO DE 1 SEGUNDO
}

void loop(){

	/*sensors.requestTemperatures();//SOLICITA QUE A FUNÇÃO INFORME A TEMPERATURA DO SENSOR
	Serial.print("Temperatura: "); //IMPRIME O TEXTO NA SERIAL
	Serial.print(sensors.getTempCByIndex(0)); //IMPRIME NA SERIAL O VALOR DE TEMPERATURA MEDIDO
	Serial.println("*C"); //IMPRIME O TEXTO NA SERIAL
	server.handleClient();
	delay(1);
	handleRoot();

}


*/