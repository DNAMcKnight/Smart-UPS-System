#include <Servo.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <ESP8266Ping.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define LED_PIN 2
bool PC_ACTIVE = false;

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
bool pc_active()
{
  PC_ACTIVE = true;
  return true;
}

bool shutdown_callback(bool monitor)
{
  HTTPClient http;
  WiFiClient client;
  http.begin(client, "http://192.168.0.100:8000/shutdown");
  int httpResponseCode = http.GET();
  String response = http.getString();
  int timeout = response.toInt(); // in seconds
  http.end();
  if (httpResponseCode <= 0)
    return false;
  if (monitor == false)
    return true;

  Serial.println("Shutdown requested, timeout: " + String(timeout));

  unsigned long startTime = millis();
  while (millis() - startTime < timeout * 1000UL)
  {
    if (!Ping.ping("192.168.0.100", 2))
    { // PC is down
      Serial.println("PC is down!");
      return true;
    }
  }

  Serial.println("Timeout reached, PC still up");
  return false;
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
    server.send(200, "text/plain", "OK"); });

  server.on("/complete_shutdown", []()
            {
  if (!Ping.ping("192.168.0.100", 2)) {
    Serial.println("PC is off Toggling UPS switch");
    digitalWrite(LED_PIN, LOW);
    toggle_switch(0, 165);
    digitalWrite(LED_PIN, HIGH);
    server.send(200, "text/plain", "OK");
    return;
  }
  server.send(200, "text/plain", "OK");
  bool shutdown = shutdown_callback(true);
  Serial.println("Shutdown callback result: " + String(shutdown));
  if (shutdown)
  {
    Serial.println("Toggling UPS switch");
    digitalWrite(LED_PIN, LOW);
    toggle_switch(0, 165);
    digitalWrite(LED_PIN, HIGH);
    PC_ACTIVE = false;
  } });

  server.on("/pc_shutdown", []()
            {
    server.send(200, "text/plain", "OK");
    shutdown_callback(false); 
    PC_ACTIVE = false; });

  server.on("/pc_active", []()
            {
    server.send(200, "text/plain", "OK");
    pc_active(); });
  server.on("/settings", []()
            {
    JsonDocument doc;
    doc["PC_ACTIVE"] = PC_ACTIVE;

    String response;
    serializeJson(doc, response);

    server.send(200, "application/json", response); });

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
    Serial.println("connected...yaay :)");
  }
  // wifi_set_sleep_type(MODEM_SLEEP_T); // Allow modem sleep
  wifi_set_sleep_type(LIGHT_SLEEP_T); // Light sleep with DTIM listening (keeps connection alive)

  webserver();
  digitalWrite(LED_PIN, HIGH);
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
}