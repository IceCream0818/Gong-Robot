#include <WiFi.h>
#include <esp_now.h>
 
#define ESPNOW_WIFI_CHANNEL 6
#define btnPin 21
 
uint8_t broadcastAddress[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
bool ledToggle = false;
 
void setup() {
  Serial.begin(115200);
  
  pinMode(btnPin, INPUT);
 
  WiFi.mode(WIFI_STA);
  WiFi.setChannel(ESPNOW_WIFI_CHANNEL);
 
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    ESP.restart();
  }
 
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = ESPNOW_WIFI_CHANNEL;
  peerInfo.encrypt = false;
 
  esp_now_add_peer(&peerInfo);
 
  Serial.println("ESP-NOW Sender ready");
}
 
void loop() {
  char msg[3];
  ledToggle = (digitalRead(btnPin) == HIGH) ? true : false;
 
  msg[0] = ledToggle ? '1' : '0';
  msg[1] = '1'; // Speed setting (0 for fast, 1 for slow)
  msg[2] = '\0';
 
  esp_now_send(broadcastAddress, (uint8_t *)msg, sizeof(msg));
 
  Serial.printf("Sent: %s\n", msg);
  delay(100);
}