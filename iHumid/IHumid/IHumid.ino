#include <Process.h>
#include <Bridge.h>
#include <Console.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>



#define DHTPIN            A0         // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT11     // DHT 11 

// initialize DHT11
DHT dht(DHTPIN, DHTTYPE);

String result;
int puente = 2; // BlueLed
int wifiConectionStatus = 3; //RedLed
int dataSended = 4; //GreenLed
int dhtStatus = 5; //YellowLed
int bomb = 6; //BlueCable ElectroBomb
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

  Serial.println("Starting bridge...\n");
  pinMode(puente,OUTPUT);   
  pinMode(wifiConectionStatus,OUTPUT);  
  pinMode(dataSended,OUTPUT);
  pinMode(dhtStatus,OUTPUT); 
  pinMode(bomb, OUTPUT);

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
      Process c;
      Process time;
      time.begin("date");
      time.addParameter("+%D-%T");  
      time.run(); 

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
     hic = dht.computeHeatIndex(t, h, false);
     
     wifiCheck.runShellCommand("/usr/bin/pretty-wifi-info.lua");

     while (wifiCheck.available() > 0) {
        digitalWrite(wifiConectionStatus, HIGH);  // Bridge Incializado
        char c = wifiCheck.read();
  
        cadenaWifiCheck = c;
        
        Serial.print(cadenaWifiCheck);   
     } 

      while(time.available()>0) {
      char c = time.read();
      if(c != '\n')
        result += c;
      }
      result = '"' + result + '"';


      if(h<70){
      digitalWrite(bomb, HIGH);
      delay(500); 
      digitalWrite(bomb, LOW);
      }else if(h>=75){
      digitalWrite(bomb, LOW); 
      }
      
      digitalWrite(dataSended, HIGH);
      Serial.println("Enviando datos a Firebase");
      s.runShellCommand("curl -k -X POST https://puroapp-c85d9.firebaseio.com/Users/DS6w8GODm9MXE6Q55EqYWGEMD7A3/iHumidStore.json -d '{\"tempC\" : " + String(t) + ", \"tempF\" : " + String(f) + ", \"humid\" : " + String(h) + ", \"time\" : " + String(result) +"}'");
      Serial.println("\nDato Humedad: " + String(h) + "\nDato Temperatura: " + String(t) + "\nDato Farenheit: " + String(f) + "\nEnviado a Firebase iHumid");
      p.runShellCommand("curl -k -X PUT https://puroapp-c85d9.firebaseio.com/Users/DS6w8GODm9MXE6Q55EqYWGEMD7A3/iHumid.json -d '{\"tempC\" : " + String(t) + ", \"tempF\" : " + String(f) + ", \"humid\" : " + String(h) +"}'");  
      Serial.println("\nDato Humedad: " + String(h) + "\nDato Temperatura: " + String(t) + "\nDato Farenheit: " + String(f) + "\nEnviado a Firebase iHumidStore");
      //c.runShellCommand("curl -k -X PUT https://puroapp-c85d9.firebaseio.com/time.json -d '{\"Time\" : " + String(result) +"}'"); 
      Serial.println(String(result));
      digitalWrite(dataSended,LOW);
      Serial.println("Datos Enviados a Firebase");


      Console.println(t);
      while(p.running() && s.running());
      result = "";

      //5 minutos 300000
      Serial.println();
      delay(21000);   

  }
