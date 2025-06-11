#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// WiFi credentials
const char* ssid = "iPhone";
const char* password = "password883";

// Firebase credentials
#define API_KEY "AIzaSyDatSpnWpeTRWiPzm9-ovcDsWIdphNJDhk"
#define DATABASE_URL "https://esp32s3-data-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// User credentials
#define USER_EMAIL "ece21010304027nikhil@gmail.com"
#define USER_PASSWORD "Nikhil"

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32-S3 Starting...");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\n Connected to WiFi");

  // Firebase config
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.print("Connecting to Firebase");
  unsigned long startTime = millis();
  while (!Firebase.ready()) {
    Serial.print(".");
    delay(300);

    if (millis() - startTime > 20000) {
      Serial.println("\nFirebase not ready. Check credentials or internet.");
      return;
    }
  }

  Serial.println("\nFirebase is ready");
}

void loop() {
  if (!Firebase.ready()) {
    Serial.println(" Firebase not ready in loop");
    delay(3000);
    return;
  }

  int sensorValue = random(0, 1024);  // Simulated sensor data

  Serial.println("Sending: " + String(sensorValue));

  if (Firebase.RTDB.setInt(&fbdo, "/sensor/value", sensorValue)) {
    Serial.println("Data sent: " + String(sensorValue));
  } else {
    Serial.println("Send failed: " + fbdo.errorReason());
  }

  delay(5000);

if (Firebase.RTDB.getInt(&fbdo, "/sensor/value")) {
  int received = fbdo.intData();
  Serial.println("Received: " + String(received));
} else {
  Serial.println("Read failed: " + fbdo.errorReason());
}

  delay(9000);
}


