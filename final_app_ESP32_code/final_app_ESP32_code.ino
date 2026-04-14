#define BLYNK_TEMPLATE_ID "TMPL3TcenC7UX"
#define BLYNK_TEMPLATE_NAME "food spoilage detection"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Initialize LCD (Address 0x27 is common, if not working try 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

char auth[] = "7EVX6MLdiH3BbsCg8swIH4MebfiYLoWp";
char ssid[] = "Redmi 12 5G";
char pass[] = "password";

#define GAS_SENSOR 34
#define FIRE_SENSOR 35 
#define RED_LED 2
#define GREEN_LED 15
#define BUZZER 4

int GAS_THRESHOLD = 1300;
int FIRE_THRESHOLD = 2000; 

BlynkTimer timer;
bool gasNotified = false;
bool fireNotified = false;

void checkSensors() {
  int gasValue = analogRead(GAS_SENSOR);
  int fireValue = analogRead(FIRE_SENSOR);
  int fireIntensity = 4095 - fireValue; 

  bool isGasAlert = (gasValue > GAS_THRESHOLD);
  bool isFireAlert = (fireIntensity > FIRE_THRESHOLD);

  // --- LCD UPDATE LOGIC ---
  lcd.clear();
  if (isGasAlert || isFireAlert) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER, HIGH);

    lcd.setCursor(0, 0);
    lcd.print("!! WARNING !!");
    lcd.setCursor(0, 1);
    
    if (isFireAlert) {
        lcd.print("FIRE DETECTED!");
        if(!fireNotified) { Blynk.logEvent("fire_alert", "Fire detected in Fridge!"); fireNotified = true; }
    } else {
        lcd.print("FOOD SPOILED!");
        if(!gasNotified) { Blynk.logEvent("food_spoiled", "Food Spoilage Detected in Fridge!"); gasNotified = true; }
    }
  } 
  else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BUZZER, LOW);
    gasNotified = false;
    fireNotified = false;

    // Normal Display
    lcd.setCursor(0, 0);
    lcd.print("Gas: "); lcd.print(gasValue);
    lcd.setCursor(0, 1);
    lcd.print("Fire: "); lcd.print(fireIntensity);
  }

  // Update Blynk
  Blynk.virtualWrite(V12, gasValue);
  Blynk.virtualWrite(V13, fireIntensity);
}

void setup() {
  Serial.begin(115200);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Booting...");

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  WiFi.begin(ssid, pass);
  Blynk.config(auth);
  
  timer.setInterval(1500L, checkSensors); // Increased slightly for LCD stability
}

void loop() {
  Blynk.run();
  timer.run();
}
