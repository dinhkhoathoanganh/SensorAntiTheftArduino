/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "msXf3Fyke9jy6FYQ9TWW_FcH7SMCSVtP";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "dlink-D0E0";
char pass[] = "qgiec91181";

/* Vibration Sensor (Shake Switch) */
   
int sensor;          
int previous;
int moveCounter = 0;
int VIBRATION_Pin = A0;

BlynkTimer timer;

long delayTime = 150L;
int moveThreshold;

BLYNK_WRITE(V0)
{
  delayTime = long(param.asInt());
}

BLYNK_WRITE(V1)
{
  moveThreshold = param.asInt();
}

void notifyUptime()
{
  if (delayTime < 50L){ delayTime = 150L; }
  if (moveThreshold < 10){ moveThreshold = 20; }
  
  sensor = analogRead(VIBRATION_Pin);
  
  //While sensor is moving
  if (abs(sensor - previous) > 1){
    moveCounter = moveCounter + 10;
    Serial.println(sensor);
  }
  else{ 
    if (moveCounter > 0) { moveCounter = moveCounter - 1; }
  }
  
  if (moveCounter >= moveThreshold) {
    Blynk.notify("DRAWER OPEN!!!");
    moveCounter = 0;
  }

  previous = sensor;
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Notify immediately on startup
  Blynk.notify("Device started");

  // Setup a function to be called every minute
  timer.setInterval(delayTime, notifyUptime);
}

void loop()
{
  Blynk.run();
  timer.run();
}
