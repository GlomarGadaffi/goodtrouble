// good_trouble.ino  — ESP32 (Arduino-ESP32 core)
// Expressive RF protest beacon: cycles through your own protest strings while
// the lower-MAC half rotates under a FIXED, made-up, locally-administered OUI.
// Spectrum-polite by design: low power, ordinary interval. Speech, not a blockade.

#include <BLEDevice.h>
#include <BLEAdvertising.h>

// ---- YOUR MESSAGES (cycles through, one per rotation) -----------------------
static const char* MESSAGES[] = {
  "GOOD_TROUBLE",
  "CONSENT_DENIED",
  "I_AM_NOT_DATA",
  "NO_TRACKING",
  "STOP_WATCHING",
  "TRACK_THIS",
  "PRIVACY_NOW",
  "NOT_YOUR_NODE",
};
static const size_t N_MSG = sizeof(MESSAGES)/sizeof(MESSAGES[0]);
static size_t msgIdx = 0;

// ---- THE CROWD'S SHARED OUI -------------------------------------------------
// Locally-administered (0x02 bit set in first octet) = honest "self-made, not a
// vendor." Do NOT use a real vendor's OUI or any camera-vendor OUI.
static const uint8_t TROUBLE_OUI[3] = { 0x02, 0x47, 0x54 };  // 'GT'

static const uint32_t ROTATE_MS = 2000;
static const int8_t   TX_POWER  = ESP_PWR_LVL_N12;  // LOW: be one of many

BLEAdvertising* adv;

static void setRotatingAddr() {
  uint8_t mac[6];
  memcpy(mac, TROUBLE_OUI, 3);
  for (int i = 3; i < 6; i++) mac[i] = esp_random() & 0xFF;
  esp_base_mac_addr_set(mac);
}

void buildAndStart() {
  setRotatingAddr();
  std::string msg = MESSAGES[msgIdx];
  msgIdx = (msgIdx + 1) % N_MSG;            // next string on next rotation

  BLEDevice::init(msg);
  BLEDevice::setPower((esp_power_level_t)TX_POWER);
  adv = BLEDevice::getAdvertising();
  BLEAdvertisementData data;
  data.setName(msg);
  std::string mfg; mfg += (char)0xFF; mfg += (char)0xFF;  // 0xFFFF = experimental
  mfg.append(msg);
  data.setManufacturerData(mfg);
  adv->setAdvertisementData(data);
  adv->setMinInterval(0x00A0);   // ~100ms, ordinary
  adv->setMaxInterval(0x00F0);
  adv->start();
}

void setup() { buildAndStart(); }

void loop() {
  delay(ROTATE_MS);
  adv->stop();
  BLEDevice::deinit(false);
  buildAndStart();
}
