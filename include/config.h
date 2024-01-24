#pragma once

// Main libraries
#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// Our libraries
#include <RadioLib_wrapper.h>
#include <Ccsds_packets.h>

// Used radio module
#define radio_module SX1262
#define radio_module_family Sx126x
#define radio_module_rf_switching Disabled

class Config
{
public:
  // CONFIG STRUCT DECLARATIONS
  struct WiFi_Config
  {
    char ssid[32];
    char pass[32];
    unsigned int tmPort;
    unsigned int tcPort;
  };

  // CONFIG DEFINITIONS
  // RADIO
  RadioLib_Wrapper<radio_module>::Radio_Config radio_config{
      .frequency = 434.5, // Frequency
      .cs = 8,            // Chip select
      .dio0 = 13,         // Busy
      .dio1 = 14,         // Interrupt action
      .family = RadioLib_Wrapper<radio_module>::Radio_Config::Chip_Family::radio_module_family,
      .rf_switching = RadioLib_Wrapper<radio_module>::Radio_Config::Rf_Switching::radio_module_rf_switching,
      // If using GPIO pins for RX/TX switching, define pins used for RX and TX control
      .rx_enable = -1,
      .tx_enable = -1,
      .reset = 12,
      .sync_word = 0xF4,
      .tx_power = 10,
      .spreading = 11,
      .coding_rate = 8,
      .signal_bw = 62.5,
      .frequency_correction = true,
      .spi_bus = &SPI // SPI bus used by radio
  };
  // Create radio object and pass error function if not passed will use serial print
  RadioLib_Wrapper<radio_module> radio = RadioLib_Wrapper<radio_module>(nullptr, 5);

  // WIFI
  /**
   * @brief WiFi config
   * @param ssid The SSID (Name) of the network to connect to
   * @param pass The password of the network to connect to
   * @param tmPort The port to send telemetry messages to
   * @param tcPort The port to receive telecommand messages from
   * @note IMPORTANT: When changing the network configuration, comment out the old configuration
   * and add a comment about the new network configuration being changed to.
   */
  WiFi_Config wifi_config{
      "Samsung S20", // Gundars phone hotspot name
      "123456789",   // Gundars phone hotspot password
      10055,         // This ports should not change
      10045          // This ports should not change
  };

  // SPI0
  const int SPI0_RX = 11;
  const int SPI0_TX = 10;
  const int SPI0_SCK = 9;

  // PC Serial
  const int PC_BAUDRATE = 115200;
};