
/*
  =========================================
  ||        COMPONENT       ||    PIN    ||
  =========================================
  ||  Motor IN1             ||     0     ||
  ||  Motor IN2             ||     2     ||
  ||  Ultra Echo Air        ||     5     ||
  ||  Ultra Trigger Air     ||     4     ||
  =========================================

*/

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Wire.h>

const char* ssid = "INDOMIE";
const char* password = "12345678";

// Initialize Telegram BOT
#define BOTtoken "6566753255:AAEaKLJ9qetd5yrI07ptQj-FYCwyIgR9mYM"
#define CHAT_ID "1279174157"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 500;
unsigned long lastTimeBotRan;

#define RAINDROP_SENSOR_PIN 14
const int echoAirPin = 5;
const int trigAirPin = 4;
const int IN1 = 0; 
const int IN2 = 2; 

long durationAir, jarakAir;

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n\n";
      welcome += "Sistem ini merupakan sistem monitoring pintu ";
      welcome += "air dengan ESP8266 dilengkapi sensor ultrasonic ";
      welcome += "dan motor DC yang telah berhasil terintegrasi ";
      welcome += "dengan aplikasi Telegram secara real-time.";
      bot.sendMessage(chat_id, welcome, "");
    }

   
  }
}

void setup() {
   Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif
  
  pinMode(trigAirPin, OUTPUT);
  pinMode(echoAirPin, INPUT);
  pinMode(RAINDROP_SENSOR_PIN, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void rainDropSensor() {
  int raindropValue = digitalRead(RAINDROP_SENSOR_PIN); // Membaca nilai analog dari sensor
  Serial.print("Raindrop Sensor Value: ");
  Serial.println(raindropValue); // Menampilkan nilai sensor di Serial Monitor
  
  // Jika ingin mengkategorikan nilai sensor menjadi intensitas hujan
  if (raindropValue == 0) {
    // buka atas
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    delay(5000);

    // posisi diam
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    delay(15000);
    
    //tutup bawah
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    delay(5000);

    // posisi diam
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

  } else {
    String message = "LAPORAN KEADAAN AIR\n\n";
    message += "Turun Hujan \n";
    message += "Status : Level Air akan meningkat";
    bot.sendMessage(CHAT_ID, message, "");
  }
}

void hcsrAirValue() {
  digitalWrite(trigAirPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(trigAirPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigAirPin, LOW);
  durationAir = pulseIn(echoAirPin, HIGH);
  jarakAir = (durationAir/2) / 29.1;
  Serial.println("jarak air:");
  Serial.print(jarakAir);
  Serial.println(" cm");

  if (jarakAir >= 7 && jarakAir <= 9) {

    String message = "LAPORAN KEADAAN AIR\n\n";
    message += "Tinggi Air : "+String(jarakAir)+" Cm\n";
    message += "Status : Level Air Tinggi";
    bot.sendMessage(CHAT_ID, message, "");
    
    // buka atas
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    delay(5000);

    // posisi diam
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    delay(5000);

    // tutup bawah
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    delay(5000);

    // posisi diam
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    
  } else if (jarakAir >= 10 && jarakAir <= 12) {
    
    String message = "LAPORAN KEADAAN AIR\n\n";
    message += "Tinggi Air : "+String(jarakAir)+" Cm\n";
    message += "Status : Level Air Sedang";
    bot.sendMessage(CHAT_ID, message, "");
    
  } else {
    
    String message = "LAPORAN KEADAAN AIR\n\n";
    message += "Tinggi Air : "+String(jarakAir)+" Cm\n";
    message += "Status : Level Air Rendah";
    bot.sendMessage(CHAT_ID, message, "");
  }
  
}

void loop() {
  Serial.print(jarakAir);
  
  hcsrAirValue();
  rainDropSensor();
  
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
