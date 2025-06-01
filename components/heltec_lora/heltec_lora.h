#pragma once
#include "esphome/core/component.h"
#include "esphome/components/mqtt/mqtt_client.h"
#include <vector>

namespace esphome {
namespace heltec_lora {

class HeltecLoRaComponent : public Component {
 public:
  void setup() override;
  void loop() override {}

  void send_packet(const std::vector<uint8_t> &data);

  // so your lambdas can reach the object
  static HeltecLoRaComponent *global;
};

/// helper wrappers used by your YAML lambdas
void register_heltec_lora_component(mqtt::MQTTClientComponent *mqtt);
void heltec_lora_send(const std::vector<uint8_t> &data);

}  // namespace heltec_lora
}  // namespace esphome
