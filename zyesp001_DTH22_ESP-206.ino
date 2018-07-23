#include <DHT.h>
#include <SPI.h>
#include <ESP8266WiFi.h>

#define DHTTYPE DHT22
const byte dhtPin = 5;
DHT dht(dhtPin, DHTTYPE);
float t, tf, h;

//Sensor Number-1
String RealThingSpeakAPIkey = "xxx";  //zyesp001


//const char* ssid     = "HFUTGERONGM-6";
//const char* password = "gerongm1";

const char* ssid     = "ChinaNet-U47A";
const char* password = "YF7G4aYU";

char ThingSpeakServer[] = "api.thingspeak.com"; 


WiFiClient client;

void setup() 
{
  dht.begin();
  Serial.begin(115200);
  delay(10);

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
   if(getTH() == true)
    {    
         signed int strength = WiFi.RSSI();
                //========to ThingSpeak Real server=================
                if (client.connect(ThingSpeakServer, 80)) 
                {           
                  client.print("GET /update?key=");
                  client.print(RealThingSpeakAPIkey); 
                  client.print("&field1="); 
                  client.print(t); 
                  client.print("&field2="); 
                  client.print(h); 
                  client.print("&field3=");
                  client.print(strength);
                  client.println(" HTTP/1.1"); // Part of the GET request
                  client.println("Host:api.thingspeak.com"); 
                  client.println("Connection: close");  // Part of the GET request telling the server that we are over transmitting the message
                  client.println(); // Empty line
                  client.println(); // Empty line
                  Serial.print("Writing data ");
                  Serial.print("Temperature: "); 
                  Serial.print(t);
                  Serial.print(" degrees Celcius Humidity: "); 
                  Serial.print(h);
                  Serial.print(" % and rssi: "); 
                  Serial.println(strength);
                  Serial.print("Host: api.thingspeak.com->"); 
                  Serial.print("Connection: closed and ");
                  Serial.println("data saved to Real ThingSpeak");
                  client.stop();    // Closing connection to server                
                }
                else 
                {
                  Serial.println("--> Real ThingSpeak connection failed\n");
                }
    
    
      Serial.println("Waiting…");
      // Give the server some time to recieve the data and store it. 
      delay(10000);
    }
}


boolean getTH()
{
  static unsigned long timer = 0;
  unsigned long interval = 5000;

  if (millis() - timer > interval)
  {
    timer = millis();   
    h = dht.readHumidity();
    t = dht.readTemperature();
    tf =  dht.readTemperature(true);
    if (isnan(h) || isnan(t) || isnan(tf))
    {
      Serial.println("Failed to read from DHT sensor!");      
      return false;
    }
    return true;
  }
  else
  {
    return false;
  }
}

