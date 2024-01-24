#include <communication.h>

bool Communication::beginRadio(RadioLib_Wrapper<radio_module>::Radio_Config &radio_config)
{
  // Create a radio object
  _radio = new RadioLib_Wrapper<radio_module>(nullptr, 5, "SX1262");

  // Initialize the radio
  if (!_radio->begin(radio_config))
  {
    return false;
  }

  return true;
}

bool Communication::sendRadio(byte *bytes, size_t size)
{
  // Send the message
  if (!_radio->transmit_bytes(bytes, size))
  {
    return false;
  }

  return true;
}

bool Communication::receiveRadio(byte *&msg, uint16_t &msg_length, float &rssi, float &snr, double &frequency)
{
  // Receive the message
  if (!_radio->receive_bytes(msg, msg_length, rssi, snr, frequency))
  {
    return false;
  }

  return true;
}

void Communication::beginWiFi(Config::WiFi_Config &wifi_config)
{
  // Copy the config
  this->wifi_config = wifi_config;

  // Assign functions to WiFi events
  // Connect to WiFi event
  WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info)
               { WiFiStationConnected(event, info); },
               WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);

  // Disconnect from WiFi event
  WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info)
               { WiFiStationDisconnected(event, info); },
               WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  // WiFi.setTxPower(WIFI_POWER_19_5dBm); // Set WiFi RF power output to highest level
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_config.ssid, wifi_config.pass);

  Serial.println("Connecting to " + String(wifi_config.ssid) + " network ...");
}

void Communication::WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  // WiFi
  Serial.println("Connected to WiFi");
  Serial.println("RSSI: " + String(WiFi.RSSI()) + " dBm");
  delay(1000); // REQUIRED: Otherwise wrong IP address is assigned
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  // UDP
  tcUdp.begin(wifi_config.tcPort);
  tmUdp.begin(wifi_config.tmPort);
  Serial.println("UDP ports opened");

  connecetedToWiFi = true;
}

void Communication::WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  /*
  Known errors:
    If you get error 2, check ssid and password. If those are correct an ESP restart can also fix it
    It is fine to get the 202 error when connecting for the first time

  Reasons why WiFi got disconnected:
    WIFI_REASON_UNSPECIFIED              = 1,
    WIFI_REASON_AUTH_EXPIRE              = 2,
    WIFI_REASON_AUTH_LEAVE               = 3,
    WIFI_REASON_ASSOC_EXPIRE             = 4,
    WIFI_REASON_ASSOC_TOOMANY            = 5,
    WIFI_REASON_NOT_AUTHED               = 6,
    WIFI_REASON_NOT_ASSOCED              = 7,
    WIFI_REASON_ASSOC_LEAVE              = 8,
    WIFI_REASON_ASSOC_NOT_AUTHED         = 9,
    WIFI_REASON_DISASSOC_PWRCAP_BAD      = 10,
    WIFI_REASON_DISASSOC_SUPCHAN_BAD     = 11,
    WIFI_REASON_IE_INVALID               = 13,
    WIFI_REASON_MIC_FAILURE              = 14,
    WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT   = 15,
    WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT = 16,
    WIFI_REASON_IE_IN_4WAY_DIFFERS       = 17,
    WIFI_REASON_GROUP_CIPHER_INVALID     = 18,
    WIFI_REASON_PAIRWISE_CIPHER_INVALID  = 19,
    WIFI_REASON_AKMP_INVALID             = 20,
    WIFI_REASON_UNSUPP_RSN_IE_VERSION    = 21,
    WIFI_REASON_INVALID_RSN_IE_CAP       = 22,
    WIFI_REASON_802_1X_AUTH_FAILED       = 23,
    WIFI_REASON_CIPHER_SUITE_REJECTED    = 24,

    WIFI_REASON_BEACON_TIMEOUT           = 200,
    WIFI_REASON_NO_AP_FOUND              = 201,
    WIFI_REASON_AUTH_FAIL                = 202,
    WIFI_REASON_ASSOC_FAIL               = 203,
    WIFI_REASON_HANDSHAKE_TIMEOUT        = 204,
  */
  connecetedToWiFi = false;
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Reconnecting");

  // Stop UDP and WiFI
  tmUdp.stop();
  tcUdp.stop();
  WiFi.disconnect();

  // Reconnect to WiFi
  // WiFi.begin(wifi_config.ssid, wifi_config.pass);
  WiFi.reconnect();
}