/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "1DSt6br8w40Kr-NwqB_wSQQKmWkX9IoR";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "dlink-D0E0";
char pass[] = "qgiec91181";

int LDR_Pin = A0;

BlynkTimer timer;

int thresholdVal;

BLYNK_WRITE(V1)
{
  thresholdVal = param.asInt();
}

void notifyUptime()
{
  //long uptime = millis() / 60000L;

  int lightSensor = analogRead(LDR_Pin);

  if (thresholdVal < 50) { thresholdVal = 300; }
  
  if (lightSensor > thresholdVal) {
    Blynk.notify(String("DRAWER OPEN!!! Light : ") + lightSensor + " Threshold: " + thresholdVal);
  } else if (lightSensor < 10) {
    Blynk.notify(String("PROBLEM WITH SENSOR!!! Light too low: ") + lightSensor);
  }
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
  timer.setInterval(3000L, notifyUptime);
}

void loop()
{
  Blynk.run();
  timer.run();
}
