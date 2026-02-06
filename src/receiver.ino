#include <WiFi.h>
#include <esp_now.h>
 
#define ESPNOW_WIFI_CHANNEL 6
#define LED 2
#define IN1 21
#define IN2 19
 
volatile bool ledState = false;
volatile int speed = 250;
 
// Called when data is received
void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  
  
  if (len > 0) {
    if (data[0] == '1') {
      ledState = true;
      Serial.println("Solenoid ON");
    }
    else if (data[0] == '0') {
      ledState = false;
      Serial.println("Solenoid OFF");
 
      if (data[1] == '0'){
        speed = 100;
      } else if(data[1] == '1'){
        speed = 250;
      }
    }
  }
}
 
void setup() {
  Serial.begin(115200);
 
  pinMode(LED, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
 
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
 
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    ESP.restart();
  }
 
  esp_now_register_recv_cb(onReceive);
 
  Serial.println("ESP-NOW Receiver ready");
}
 
void loop() {
 
  if (ledState){
    digitalWrite(LED, HIGH);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    delay(speed);
 
    digitalWrite(LED, LOW);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    delay(speed - 10);
  }
 
  delay(10);
}