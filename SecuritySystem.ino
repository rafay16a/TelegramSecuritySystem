#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Initialize LCD with the I2C address 0x27
LiquidCrystal_PCF8574 lcd(0x27);

// WiFi credentials
const char* ssid = "Krajinn";
const char* password = "rafay12345";

// Telegram bot credentials
#define BOTtoken "6742731613:AAEoDiwpoze80l-Z6QxMGviZv-u0ODT2yeM"  // Your Bot Token
#define CHAT_ID "5643114560"     // Your Chat ID

// Pin definitions
const int Sensor = 16;  // D0 on ESP8266
const int LEDR = 0;     // D3 on ESP8266
const int LEDG = 2;     // D4 on ESP8266
const int Buzzer = 14;  // D5 on ESP8266

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup() {
  Serial.begin(115200);
  Wire.begin(); // Initialize I2C with default pins for ESP8266
  lcd.begin(16, 2);
  lcd.setBacklight(255);

  pinMode(Sensor, INPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int a = 0;
  lcd.setCursor(0, 0);
  lcd.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    lcd.setCursor(a % 16, 1); // Update position on second line
    lcd.print(".");
    delay(500);
    a++;
    if (a >= 16) {
      lcd.setCursor(0, 1);
      lcd.print("                "); // Clear second line after 16 attempts
    }
  }

  Serial.println("");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi connected");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(1000);
  lcd.clear();

  // Initialize Telegram bot
  configTime(0, 0, "pool.ntp.org"); // Get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org

  bot.sendMessage(CHAT_ID, "System started", "");
  lcd.setCursor(0, 0);
  lcd.print("System started");
  delay(1000);
  lcd.clear();
}

void loop() {
  bool value = digitalRead(Sensor);
  Serial.println(value);
  if (value == HIGH) { // When motion is detected
    Serial.println("Motion Detected");
    digitalWrite(LEDR, HIGH);
    digitalWrite(Buzzer, HIGH);
    digitalWrite(LEDG, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Motion Detected");
    bot.sendMessage(CHAT_ID, "Motion detected!!", "");
  } else { // When no motion is detected
    digitalWrite(LEDR, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(LEDG, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("No Motion      ");
  }

  delay(100); // Add a short delay to debounce sensor reading
}
