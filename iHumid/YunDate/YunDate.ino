/*#include <Process.h>

Process date;                 // process used to get the date

void setup() {
  Bridge.begin();        // initialize Bridge
  Serial.begin(9600);    // initialize serial  

  while(!Serial);               // wait for Serial Monitor to open

  Serial.println("Time Check");  // Title of sketch
}

void loop() {
  date.begin("/bin/date");
  date.run();
  
  //if there's a result from the date process, get it.
  while (date.available()>0) {
    // print the results we got.
    Serial.print(date.readString());
  }
  
  delay(5000); // wait 5000ms
}*/
#include <Process.h>

// Pin
int pir_pin = 8;

void setup()
{
  Bridge.begin();   // Initialize the Bridge

  pinMode(pir_pin,INPUT);
}

void loop()
{

  String result;
  Process time;
  time.begin("date");
  time.addParameter("+%D-%T");  
  time.run(); 

  while(time.available()>0) {
    char c = time.read();
    if(c != '\n')
      result += c;
  }
  
  Serial.println(String(result));

  Process p;
  //p.runShellCommand("curl -k -X POST https://MY_ACCOUNT.firebaseio.com/Activity.json -d '{ \"Motion Detected At\" : " + String(result) + "}'");  
  while(p.running()); 
  delay(2000);
}
