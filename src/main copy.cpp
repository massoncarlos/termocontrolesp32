/*
#include <Arduino.h>
#include <WiFi.h>
#include <OneWire.h> //INCLUSÃO DE BIBLIOTECA
#include <DallasTemperature.h> //INCLUSÃO DE BIBLIOTECA

#define LED 2
#define DS18B20 15

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

String html;//String que armazena o corpo do site.

// Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
const int output27 = 27;

OneWire ourWire(DS18B20); //CONFIGURA UMA INSTÂNCIA ONEWIRE PARA SE COMUNICAR COM O SENSOR
DallasTemperature sensors(&ourWire); //BIBLIOTECA DallasTemperature UTILIZA A OneWire

const char* ssid = "Masson";              	// ssid do seu roteador
const char* password = "16a16235c";		// password do seu roteador
 
// Global temperature reading
float temp;


// Numero de amostras na leitura
const int nAmostras = 100;

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
	
	server.begin();
	
	sensors.begin(); //INICIA O SENSOR
	
	delay(1000); //INTERVALO DE 1 SEGUNDO
}
void http()//Sub rotina que verifica novos clientes e se sim, envia o HTML.
{
   WiFiClient client = server.available();//Diz ao cliente que há um servidor disponivel.

	if (client) {
    	Serial.println("New Client.");
		String currentLine = "";
		while (client.connected()) {
		if (client.available()) {
			char c = client.read();
			Serial.write(c);
			if (c == '\n') {
			if (currentLine.length() == 0) {
				client.println("HTTP/1.1 200 OK");
				client.println("Content-type:text/html");
				client.println("<head><meta http-equiv='refresh' content='1'></head>");
				sensors.requestTemperatures();
				temp = sensors.getTempCByIndex(0);
				client.println();
				client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 2 on.<br>");
				client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 2 off.<br>");
				client.print("Temperaratura");
				client.print((float)temp);
				client.print(".<br>");
				client.println();
				break;
			} else {
				currentLine = "";
			}
			} else if (c != '\r') {
			currentLine += c;
			}
			if (currentLine.endsWith("GET /H")) {
			digitalWrite(LED, HIGH);
			}
			if (currentLine.endsWith("GET /L")) {
			digitalWrite(LED, LOW);
			}
		}
		}
		client.stop();
		Serial.println("Client Disconnected.");
	}
	
}

void loop(){
	
	/*sensors.requestTemperatures();//SOLICITA QUE A FUNÇÃO INFORME A TEMPERATURA DO SENSOR
	Serial.print("Temperatura: "); //IMPRIME O TEXTO NA SERIAL
	Serial.print(sensors.getTempCByIndex(0)); //IMPRIME NA SERIAL O VALOR DE TEMPERATURA MEDIDO
	Serial.println("*C"); //IMPRIME O TEXTO NA SERIAL
	http();

	

}
*/

