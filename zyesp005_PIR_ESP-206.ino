#include <ESP8266WiFi.h>
#include <SPI.h>

//Sensor Number-5
String apiKey = "xxx";  //zyesp005

//const char* ssid     = "HFUTGERONGM-6";
//const char* password = "gerongm1";

const char* ssid     = "ChinaNet-U47A";
const char* password = "YF7G4aYU";
const char* server = "api.thingspeak.com"; 

int inputPin = 5; //GPIO5 choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

WiFiClient client;
signed int strength = WiFi.RSSI();

void setup() 
{
  Serial.begin(115200);
  delay(10);
  pinMode(inputPin, INPUT); // declare sensor as input

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
    val = digitalRead(inputPin);  // read input value
        if (val == HIGH)          // check if the input is HIGH
        {            
          delay(150);
          
          if (pirState == LOW) 
          {
          // "184.106.153.149" or api.thingspeak.com
          String postStr = apiKey;
          postStr +="&field1=";
          postStr += String(val);
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
          // we have just turned on
          Serial.println("Motion detected!");
          Serial.print(" value = "); 
          Serial.println(val);
          Serial.print(" Signal Strength = "); 
          Serial.print(strength);
          Serial.println(" sent to Thingspeak");
          // We only want to print on the output change, not state
          pirState = HIGH;
          }
        } 
        else 
        {
           if (pirState == HIGH)
           {
            // "184.106.153.149" or api.thingspeak.com
          String postStr = apiKey;
          postStr +="&field1=";
          postStr += String(val);
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
          // we have just turned on
          Serial.println("Motion ended!");
          Serial.print(" value = "); 
          Serial.println(val);
          Serial.print(" Signal Strength = "); 
          Serial.print(strength);
          Serial.println(" sent to Thingspeak");
          // We only want to print on the output change, not state
          pirState = LOW;
          }
        }
     client.stop();
     Serial.println("Waiting…");
  }
}
