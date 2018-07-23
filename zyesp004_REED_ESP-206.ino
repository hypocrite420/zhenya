#include <ESP8266WiFi.h>
#include <SPI.h>

//Sensor Number-4
String apiKey = "xxx";  //zyesp004


const char* ssid     = "ChinaNet-U47A";
const char* password = "YF7G4aYU";

const char* server = "api.thingspeak.com"; 

const int REED_PIN = 5; // GPIO05

WiFiClient client;
signed int strength = WiFi.RSSI();

void setup() 
{
  Serial.begin(115200);
  delay(10);
  pinMode(REED_PIN, INPUT_PULLUP);

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
        int door = digitalRead(REED_PIN);
        if(door == HIGH)
        {
           // "184.106.153.149" or api.thingspeak.com
          String postStr = apiKey;
          postStr +="&field1=";
          postStr += String(door);
          postStr +="&field2=";
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
          Serial.print("Door Status：OPEN "); 
          Serial.print(" value = "); 
          Serial.print(door);
          Serial.print(" Signal Strength = "); 
          Serial.print(strength);
          Serial.println(" sent to Thingspeak");
          //delay(30000);
        } 
         else
        {
           // "184.106.153.149" or api.thingspeak.com
          String postStr = apiKey;
          postStr +="&field1=";
          postStr += String(door);
          postStr +="&field2=";
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
          Serial.print("Door Status：CLOSED "); 
          Serial.print(" value = "); 
          Serial.print(door);
          Serial.print(" Signal Strength = "); 
          Serial.print(strength);
          Serial.println(" sent to Thingspeak");
        } 
     client.stop();
     Serial.println("Waiting…");
     // thingspeak needs minimum 15 sec delay between updates
     delay(3000);
     }
}

