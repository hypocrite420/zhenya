#include <ESP8266WiFi.h>
#include <SPI.h>

//Sensor Number-8
String apiKey = "xxx";  //zyesp008

//const char* ssid     = "HFUTGERONGM-6";
//const char* password = "gerongm1";

const char* ssid     = "ChinaNet-U47A";
const char* password = "YF7G4aYU";
const char* server = "api.thingspeak.com"; 

WiFiClient client;
signed int strength = WiFi.RSSI();

int measurePin = A0; //ADC
int ledPower = 5; //GPIO5

unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(ledPower,OUTPUT);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // start the WiFi connection
  WiFi.begin(ssid, password);
  
   while (WiFi.status() != WL_CONNECTED) 
     {
        delay(500);
        Serial.print("...");
        Serial.println("Can't connect to WIFI");
      }
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Signal Strength: ");
    Serial.println(WiFi.RSSI());
}

void loop()
{
   if (client.connect(server,80)) 
   {
    digitalWrite(ledPower,LOW);
    delayMicroseconds(samplingTime);
  
    voMeasured = analogRead(measurePin);
  
    delayMicroseconds(deltaTime);
    digitalWrite(ledPower,HIGH);
    delayMicroseconds(sleepTime);
  
    calcVoltage = voMeasured*(5.0/1024);
    dustDensity = 0.17*calcVoltage-0.1;
  
    if ( dustDensity < 0)
    {
      dustDensity = 0.00;
    }
  
     // "184.106.153.149" or api.thingspeak.com
          String postStr = apiKey;
          postStr +="&field1=";
          postStr += String(voMeasured);
          postStr +="&field2=";
          postStr += String(calcVoltage);
          postStr +="&field3=";
          postStr += String(dustDensity);
          postStr +="&field4=";
          postStr += String(strength);
          postStr += "\r\n\r\n";
          client.print("POST /update HTTP/1.1\n");
          client.print("Host: api.thingspeak.com\n");
          client.print("Connection: close\n");
          client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
          client.print("Content-Type: application/x-www-form-urlencoded\n");
          client.print("Content-Length: ");
          client.print(postStr.length());
          client.print("\n\n");
          client.print(postStr);
          Serial.println("Raw Signal Value (0-1023):");
          Serial.println(voMeasured);
  
          Serial.println("Voltage:");
          Serial.println(calcVoltage);
  
          Serial.println("Dust Density:");
          Serial.println(dustDensity);
          Serial.print(" Signal Strength = "); 
          Serial.print(strength);
          Serial.println(" sent to Thingspeak");
  
    delay(10000);
   }
  
}


