#include <Adafruit_BMP085_U.h>

#include <WiFi.h>

#include <dummy.h>

#include <Arduino.h>

// Includes para o código funcionar, dependencias necessárias
#include <Adafruit_Sensor.h>
#include <SPI.h>

// Dependencias para rede e envio de requisições
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

// Umidade - DHT11
#include "DHT.h"

// Temperatura DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>

// Pressão Atmosférica - BMP180
#include <Adafruit_BMP085.h> //Biblioteca para sensor BMP180
#include <Wire.h> //Biblioteca para comunicação I2C

// Temperatura - DS18B20
const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// Pressão Atmosférica
Adafruit_BMP085 bmp; //Define objeto sensor na classe SFE_BMP180 da biblioteca
int ALTITUDE = 4; // Altitude Joinville em metros
char status; // Variável auxiliar para verificação do resultado
double temperatura; //variável para armazenar o valor da temperatura
double pressao; //variável para armazenar o valor da pressão absoluta
double pressao_relativa; //variável para armazenar a pressão relativa

// Umidade
#define DHTPIN 5 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11

// Delay de coleta de dados
const int delay_min = 20;
const int delay_mili = delay_min * 60 * 1000;

DHT dht(DHTPIN, DHTTYPE);

// Server para envio de requisições
// const char* serverURL = "http://192.168.15.10:3000/data";
const char* serverURL = "https://server-mongodb-theta.vercel.app/api/data";

// Rede Wifi
const char* ssid = "CLARO_583223";
const char* password = "4R29jp471L";

// Variáveis de rede
// IPAddress Server_ip(192, 168, 1, 236);  // IP address of this box
// IPAddress gateway(192, 168, 1, 1);    // gateway of your network
// IPAddress subnet(255, 255, 255, 0);     // subnet mask of your network
// IPAddress dns(181, 213, 132, 2);        //dns address needed to help get to internet AND to ntp site below

//message buffering
String postData;  //String to post status data

//Client and Server starts
WiFiClientSecure client;
HTTPClient http;

void SendSensorData(String sensor_message);

void SendSensorData(String sensor_message) {
  postData = sensor_message;

  client.setInsecure();
  http.begin(serverURL);

  http.addHeader("Content-Type", "application/json");      // SPECIFY JSON
  http.addHeader("api-key", "nbkzbvei");  //specify API key

  int httpCode = http.POST(postData);  //Send the request

  delay(10000);
  if (httpCode > 0) {
    // HTTP header has been sent and server response header has been handled
    Serial.printf("HTTP POST code: %d\n", httpCode);
    String payload = http.getString();
    Serial.println("Response payload: " + payload);
  } else {
    // HTTP POST failed
    Serial.printf("HTTP POST failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();  //Close connection
}

void setup() {
  Serial.begin(74880);

  // DS18B20 - Temperatura
  sensors.begin();

  // BMP180 - Pressão Atmosférica
  bmp.begin();

  // DHT11 - Umidade
  dht.begin();

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  delay(1000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi network.");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi network.");
  }
}

void loop() {
  // Busca data e hora BRT
  configTime(-10800, 0, "pool.ntp.org", "time.nist.gov"); // Configure NTP
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  time(&now);
  char timeString[20];
  strftime(timeString, sizeof(timeString), "%d/%m/%Y %H:%M:%S", &timeinfo);

  // Dados do sensor BMP180
  float temperaturaBMP = bmp.readTemperature();
  float pressaoBMP = bmp.readPressure();
  float pressaoNivelMarBMP = bmp.readSealevelPressure();

  // Dados do sensor DS18B20
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);

  // Dados do sensor DHT11
  float umidadeDHT = dht.readHumidity();
  float temperaturaDHT = dht.readTemperature();
  if (isnan(temperaturaDHT) || isnan(umidadeDHT))
  {
    Serial.println("Falha ao ler sensor DHT11\n\n");
  }

  // Envia dados para o MongoDB - iotChuvaDB
  String Bstr;  //string received from sensor

  Bstr = "{\"DS18B20TempCelsius\":\"" + String(temperatureC) +
  "\",\"BMPTempCelsius\":\"" + String(temperaturaBMP) +
  "\",\"BMPPressaoPascal\":\"" + String(pressaoBMP) +
  "\",\"BMPPressaoNivelMarPascal\":\"" + String(pressaoNivelMarBMP) +
  "\",\"DHTTempCelsius\":\"" + String(temperaturaDHT) +
  "\",\"DHTUmidadePct\":\"" + String(umidadeDHT) +
  "\",\"Date\":\"" + String(timeString) + "\"}\r\n\r\n";

  SendSensorData(Bstr);

  delay(delay_mili);
}