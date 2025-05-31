#pragma once

#include "esphome/core/component.h"
#include "esphome/components/mqtt/mqtt_client.h"
#include <heltec.h>  // Heltec library

namespace esphome {
namespace heltec_lora {

class HeltecLoRaComponent : public Component {
 public:
  // We'll store a pointer to MQTT so we can publish incoming data
  mqtt::MQTTClientComponent *mqtt_ = nullptr;

  void setup() override {
    // Initialize Heltec (LoRa=on, Display=off, Serial=on, PA Boost=on, freq=915E6)
    Heltec.begin(false, true, true, true, 915E6);

    LoRa.setSpreadingFactor(7);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);

    // If needed: Radio.SetDio2AsRfSwitch(true); or other advanced calls

    ESP_LOGI("heltec_lora", "LoRa setup complete");
  }

  void loop() override {
    // Check for incoming LoRa packet
    int packetSize = LoRa.parsePacket();
    if (packetSize <= 0) return;

    // Example parse if you follow DEST(4)+SRC(4)+OPCODE(1)+payload
    if (packetSize < 9) {
      // read and discard incomplete
      while (LoRa.available()) LoRa.read();
      return;
    }

    uint8_t buf[256];
    int i = 0;
    while (LoRa.available() && i < 256) {
      buf[i++] = LoRa.read();
    }

    char dest[5] = {0}, src[5] = {0};
    memcpy(dest, &buf[0], 4);
    memcpy(src, &buf[4], 4);

    char op = buf[8];
    std::string payload((char*)&buf[9], i - 9);

    ESP_LOGI("heltec_lora", "RX from %s, opcode=%c, payload=%s", src, op, payload.c_str());

    // If you want to publish to MQTT
    if (this->mqtt_) {
      // e.g. pick a topic
      char topic[40];
      snprintf(topic, sizeof(topic),
               (op == 'S') ? "lora/%.4s/sensor" : "lora/%.4s/rx",
               src);
      this->mqtt_->publish(topic, payload);
    }
  }

  // Send data over LoRa
  void send_packet(const std::vector<uint8_t> &data) {
    LoRa.beginPacket();
    for (auto b : data) {
      LoRa.write(b);
    }
    LoRa.endPacket();
    ESP_LOGI("heltec_lora", "Sent %d bytes", (int)data.size());
  }
};

// We’ll keep a single static pointer to our component
// so we can reference it from the main YAML code easily.
extern HeltecLoRaComponent *g_heltec_lora;

// Register function: link to MQTT + register with ESPHome
void register_heltec_lora_component(mqtt::MQTTClientComponent *mqtt);

}  // namespace heltec_lora
}  // namespace esphome
