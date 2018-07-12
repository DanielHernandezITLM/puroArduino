#include <Process.h>
#include <Bridge.h>
#include <Console.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>


#define DHTPIN            A0         // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT11     // DHT 11 

// initialize DHT11
DHT dht(DHTPIN, DHTTYPE);

int puente = 2; // BlueLed
int wifiConectionStatus = 3; //RedLed
int dataSended = 4; //GreenLed
int dhtStatus = 5; //YellowLed
float h; // Humidity variable
float t; // Centigrade Temperature Variable
float f; // Fahrenheit Temperature Variable
float hif; // Fahrenheit heat index Variable
float hic; // Centigrade heat index Variable

String cadenaWifiCheck; //Conection Status String

void setup()
{
  Serial.begin(9600); 
  dht.begin();
  while(!Serial);      

  Serial.println("Starting bridge...\n");
  pinMode(puente,OUTPUT);   
  pinMode(wifiConectionStatus,OUTPUT);  
  pinMode(dataSended,OUTPUT);
  pinMode(dhtStatus,OUTPUT); 

  digitalWrite(puente, LOW);  
  Bridge.begin();  
  digitalWrite(puente, HIGH);  // Bridge Incializado

  delay(2000);  
}

void loop()
{
    Process wifiCheck;
    Process p;
    Process s;

    // Reading relative humidity
    h = dht.readHumidity();
    // Reading centigrade temperature
    t = dht.readTemperature();
    // Reading Fahrenheit temperature
    f = dht.readTemperature(true);

      // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error reading DHT11 data");
    digitalWrite(dhtStatus,HIGH);
    return;
  }

    // Calculate Fahrenheit heat index
  hif= dht.computeHeatIndex(f, h);
  // Calculate centigrade heat index
  float hic = dht.computeHeatIndex(t, h, false);
     
    wifiCheck.runShellCommand("/usr/bin/pretty-wifi-info.lua");

  while (wifiCheck.available() > 0) {
        digitalWrite(wifiConectionStatus, HIGH);  // Bridge Incializado
        char c = wifiCheck.read();

        cadenaWifiCheck = c;

    
      
        Serial.print(cadenaWifiCheck);   
     }  

     
      digitalWrite(dataSended, HIGH);
      Serial.println("Enviando dato a Firebase");
      s.runShellCommand("curl -k -X PUSH https://puroapp-c85d9.firebaseio.com/iHumid.json -d '{\"tempC\" : " + String(t) + ", \"tempF\" : " + String(f) + ", \"humid\" : " + String(h) +"}'");
      Serial.println("\nDato Temperatura: " + String(t) + "\nEnviado a Firebase iHumid");
      p.runShellCommand("curl -k -X PUT https://puroapp-c85d9.firebaseio.com/iHumidStore.json -d '{\"tempC\" : " + String(t) + ", \"tempF\" : " + String(f) + ", \"humid\" : " + String(h) +"}'");  
      Serial.println("\nDato Temperatura: " + String(t) + "\nDato Farenheit: " + String(f) + "\nDato Humedad: " + String(h) + "\nEnviado a Firebase iHumidStore");
      digitalWrite(dataSended,LOW);
      Console.println(t);
      while(p.running() && s.running());
    
      Serial.println();
      delay(2000);   

  }
