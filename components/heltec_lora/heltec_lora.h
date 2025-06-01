#pragma once
#include "esphome/core/component.h"          //  <-- add
#include <vector>

namespace esphome {
namespace mqtt { class MQTTClientComponent; }

namespace heltec_lora {

class HeltecLoRaComponent : public Component {   // derives from Component
 public:
  HeltecLoRaComponent();                         // ctor registers itself
  void setup() override;                         // called by ESPHome
  void send_packet(const std::vector<uint8_t> &data);

  static HeltecLoRaComponent *global;            // ptr used by C helpers
};

/// C-helpers exposed to YAML lambdas
void register_heltec_lora_component(mqtt::MQTTClientComponent *mqtt);
void heltec_lora_send(const std::vector<uint8_t> &data);

}  // namespace heltec_lora
}  // namespace esphome
