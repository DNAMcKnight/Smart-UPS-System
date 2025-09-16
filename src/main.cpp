#include <Servo.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

#define LED_PIN 2

ESP8266WebServer server(80);
Servo myservo;
int servoPin = D4; // connect servo signal wire here

void toggle_switch(int angle1, int angle2)
{
  // Move to angle1
  myservo.write(angle1);
  delay(500);

  myservo.write(angle2);
  delay(1000);

  myservo.write(angle1);

  delay(500);
}

void webserver()
{
  LittleFS.begin();
  if (!LittleFS.begin())
  {
    Serial.println("Failed to mount filesystem");
    return;
  }
  server.on("/", []()
            {
    File f = LittleFS.open("/index.html", "r");
    if (!f) {
      server.send(404, "text/plain", "File not found");
      return;
    }
    server.streamFile(f, "text/html");
    f.close(); });

  server.on("/styles.css", []()
            {
    File f = LittleFS.open("/styles.css", "r");
    if (!f) {
        server.send(404, "text/plain", "File not found");
        return;
    }
    server.streamFile(f, "text/css");
    f.close(); });

  server.on("/scripts.js", []()
            {
    File f = LittleFS.open("/scripts.js", "r");
    if (!f) {
        server.send(404, "text/plain", "File not found");
        return;
    }
    server.streamFile(f, "application/javascript");
    f.close(); });

  server.on("/toggle", []()
            {
              digitalWrite(LED_PIN, LOW);
              toggle_switch(0, 165);
              digitalWrite(LED_PIN, HIGH);
              server.send(200, "text/plain", "OK"); // respond to AJAX
            });

  server.begin();
}
void setup()
{
  // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.

  // put your setup code here, to run once:
  Serial.begin(115200);
  myservo.attach(servoPin);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  // WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  // wm.resetSettings();

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("AutoConnectAP", "password"); // password protected ap

  if (!res)
  {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else
  {
    // if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }
  webserver();
  digitalWrite(LED_PIN, HIGH);
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
}

// #include <ESP8266Ping.h>
// // 👉 If you specifically want ICMP ping (like the ping command), ESP8266 has a Ping library: ESP8266Ping
// // . Example:

// void loop()
// {
//   static unsigned long lastCheck = 0;
//   if (millis() - lastCheck > 5000)
//   {
//     lastCheck = millis();
//     if (Ping.ping(IPAddress(192, 168, 0, 100)))
//     {
//       Serial.println("Ping success!");
//     }
//     else
//     {
//       Serial.println("Ping failed!");
//     }
//   }
// }