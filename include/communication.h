#pragma once
#include <config.h>

class Communication
{
public:
  RadioLib_Wrapper<radio_module> *_radio;

  // Wifi
  WiFiUDP tcUdp;
  WiFiUDP tmUdp;

  // Config needs to be copied to this variable in beginWiFi()
  // for us to be able to access config values in WiFi events
  Config::WiFi_Config wifi_config;

  bool connecetedToWiFi = false;
  bool remoteIpKnown = false;
  unsigned long lastUdpReceivedMillis = 0;

  int rotatorPositionMessageIndex = 0;
  unsigned long lastRotatorPositionSendMillis = 0;

  unsigned long last_radio_packet_millis = 0;

  /**
   * @brief Initialise the Communication Radio
   * @param config Payload config object
   */
  bool beginRadio(RadioLib_Wrapper<SX1262>::Radio_Config &radio_config);

  /**
   * @brief Sends the provided message using LoRa
   * @param bytes The message to send
   * @param size The size of the message
   * @return Whether the message was sent successfully
   */
  bool sendRadio(byte *bytes, size_t size);

  /**
   * @brief Receives a message using LoRa
   * @param msg The message to receive
   * @param msg_length The size of the message
   * @param rssi The RSSI of the received message
   * @param snr The SNR of the received message
   * @param frequency The frequency of the received message
  */
  bool receiveRadio(byte *&msg, uint16_t &msg_length, float &rssi, float &snr, double &frequency);

  void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);
  void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);

  void beginWiFi(Config::WiFi_Config &wifi_config);
};
