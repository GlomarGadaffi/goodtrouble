// good_trouble_config.ino — ESP32
// Boot -> if unconfigured (or BOOT button held), open a SoftAP + captive form.
// Save your message to NVS, drop the AP, start the beacon. The config surface
// is transient by design: set once, then there is no network surface left.
// Spectrum-polite: low power, ordinary interval. Speech, not a blockade.

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <BLEDevice.h>
#include <BLEAdvertising.h>

static const char* PREFIX = "NO_DRAGNET//";
static const uint8_t TROUBLE_OUI[3] = { 0x02, 0x53, 0x49 };  // shared, local-admin
static const int8_t  TX_POWER = ESP_PWR_LVL_N12;
static const uint32_t ROTATE_MS = 2000;
static const int CONFIG_BTN = 0;   // BOOT button: hold on power-up to reconfigure

Preferences prefs;
WebServer server(80);
DNSServer dns;
String tail;
BLEAdvertising* adv;
bool configMode = false;

const char* FORM =
  "<!doctype html><meta name=viewport content='width=device-width,initial-scale=1'>"
  "<style>body{font:16px sans-serif;margin:2em;background:#111;color:#eee}"
  "input{font-size:1.2em;width:100%;padding:.5em;margin:.5em 0}"
  "button{font-size:1.2em;padding:.6em 1.2em}</style>"
  "<h2>your sign</h2><form action=/save method=post>"
  "<p>shared chant: <b>NO_DRAGNET//</b></p>"
  "<input name=t maxlength=14 placeholder='your tail (e.g. I_AM_HERE)'>"
  "<button>set &amp; go</button></form>";

void handleRoot(){ server.send(200,"text/html",FORM); }

void startBeacon();

void handleSave(){
  tail = server.arg("t"); if(tail=="") tail="I_AM_HERE";
  prefs.begin("sitin",false); prefs.putString("tail",tail); prefs.end();
  server.send(200,"text/html",
    "<meta http-equiv=refresh content='2;url=about:blank'>"
    "<body style='font:18px sans-serif;background:#111;color:#0f0'>"
    "saved. broadcasting now. you can close this and turn wifi off.");
  delay(1500);
  dns.stop(); server.stop(); WiFi.softAPdisconnect(true); WiFi.mode(WIFI_OFF);
  configMode = false;
  startBeacon();
}

void runPortal(){
  configMode = true;
  WiFi.mode(WIFI_AP);
  WiFi.softAP("good-trouble-setup");          // the AP name is itself a sign; rename freely
  dns.start(53,"*",WiFi.softAPIP());    // captive: any domain -> the form
  server.onNotFound(handleRoot);
  server.on("/",handleRoot);
  server.on("/save",HTTP_POST,handleSave);
  server.begin();
}

void setRotatingAddr(){
  uint8_t m[6]; memcpy(m,TROUBLE_OUI,3);
  for(int i=3;i<6;i++) m[i]=esp_random()&0xFF;
  esp_base_mac_addr_set(m);
}
std::string msg(){ std::string s=PREFIX; s+=tail.c_str(); if(s.size()>26)s=s.substr(0,26); return s; }
void advOnce(){
  setRotatingAddr(); std::string m=msg();
  BLEDevice::init(m); BLEDevice::setPower((esp_power_level_t)TX_POWER);
  adv=BLEDevice::getAdvertising();
  BLEAdvertisementData d; d.setName(m);
  std::string mfg; mfg+=(char)0xFF; mfg+=(char)0xFF; mfg.append(m);
  d.setManufacturerData(mfg);
  adv->setAdvertisementData(d); adv->setMinInterval(0x00A0); adv->setMaxInterval(0x00F0);
  adv->start();
}
void startBeacon(){ advOnce(); }

void setup(){
  pinMode(CONFIG_BTN,INPUT_PULLUP);
  prefs.begin("sitin",true); tail=prefs.getString("tail",""); prefs.end();
  bool held = digitalRead(CONFIG_BTN)==LOW;
  if(tail=="" || held) runPortal();     // unconfigured or button held -> portal
  else startBeacon();                   // configured -> straight to broadcasting
}

void loop(){
  if(configMode){ dns.processNextRequest(); server.handleClient(); return; }
  delay(ROTATE_MS); adv->stop(); BLEDevice::deinit(false); advOnce();
}
