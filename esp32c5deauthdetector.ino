#include <WiFi.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 27
#define LED_COUNT 1

const char* WIFI_SSID = "ssid";
const char* WIFI_PASS = "passwd";

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned long deauthEndTime = 0;
unsigned long orangeUntil = 0;

bool deauthed = false;

const unsigned long ORANGE_DURATION = 3000;

void setGreen() {
  strip.setPixelColor(0, strip.Color(0, 255, 0));
  strip.show();
}

void setRed() {
  strip.setPixelColor(0, strip.Color(255, 0, 0));
  strip.show();
}

void setOrange() {
  strip.setPixelColor(0, strip.Color(255, 165, 0));
  strip.show();
}

void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {

  if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
    uint8_t reason = info.wifi_sta_disconnected.reason;

    if (
      reason == 2  ||   // AUTH_EXPIRE
      reason == 4  ||   // ASSOC_EXPIRE
      reason == 8  ||   // DEAUTH
      reason == 9  ||   // NOT_AUTHED
      reason == 10      // NOT_ASSOCED
    ) {
      deauthed = true;
      setRed();
    }
  }

  if (event == ARDUINO_EVENT_WIFI_STA_CONNECTED) {
    if (deauthed) {
      deauthed = false;
      orangeUntil = millis() + ORANGE_DURATION;
      setOrange();
    } else {
      setGreen();
    }
  }
}

void setup() {
  strip.begin();
  strip.setBrightness(40);
  setGreen();

  WiFi.mode(WIFI_MODE_STA);
  WiFi.onEvent(WiFiEvent);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
  if (orangeUntil != 0 && millis() > orangeUntil) {
    orangeUntil = 0;
    setGreen();
  }

  delay(50);
}
