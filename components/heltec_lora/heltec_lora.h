#pragma once
#include "esphome/core/component.h"
#include "esphome/components/mqtt/mqtt_client.h"
#include <vector>

// Heltec LoRa library
#include <Heltec.h>

namespace esphome {
namespace heltec_lora {

class HeltecLoRaComponent : public esphome::Component {
 public:
  mqtt::MQTTClientComponent *mqtt_ { nullptr };

  void setup() override;
  void loop() override;

  void send_packet(const std::vector<uint8_t> &data);

 private:
  void handle_rx_();
};

extern HeltecLoRaComponent *g_heltec_lora;

// helper called from on_boot
void register_heltec_lora_component(mqtt::MQTTClientComponent *mqtt);

// free-function wrapper used by lambdas
void heltec_lora_send(const std::vector<uint8_t> &data);

}  // namespace heltec_lora
}  // namespace esphome
