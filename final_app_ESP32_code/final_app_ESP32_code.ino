#define BLYNK_TEMPLATE_ID "TMPL3TcenC7UX"
#define BLYNK_TEMPLATE_NAME "food spoilage detection"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "7EVX6MLdiH3BbsCg8swIH4MebfiYLoWp";
char ssid[] = "DINESH";
char pass[] = "dinesht2x";

#define GAS_SENSOR 34
#define RED_LED 2
#define GREEN_LED 15
#define BUZZER 4

int THRESHOLD = 2000;

BlynkTimer timer;

void sendSensorData() {
  int value = analogRead(GAS_SENSOR);

  Serial.println(value);

  if (value > THRESHOLD) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER, HIGH);
    Blynk.logEvent("food_spoiled", "Warning: Food Spoilage Detected!");
    Blynk.virtualWrite(V12, "Food Spoiled 🚨");
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BUZZER, LOW);
    Blynk.virtualWrite(V12, value);
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\n--- Food Spoilage System Booting ---");

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // FORCE STATION MODE
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, pass);

  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  // We only wait 10 seconds here so the sensor can start working quickly
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 10) {
    delay(1000);
    Serial.print(".");
    attempt++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected! IP: " + WiFi.localIP().toString());
    Blynk.config(auth);
    Blynk.connect();
  } else {
    Serial.println("\nWiFi not ready yet. System starting in Offline Mode...");
    // It will keep trying to connect in the background
  }

  timer.setInterval(1000L, sendSensorData);
}

void loop() {
  // Only run Blynk if we actually have a connection
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.run();
  }
  timer.run();
}