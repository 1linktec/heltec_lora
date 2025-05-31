#include "heltec_lora.h"

namespace esphome {
namespace heltec_lora {

// Define the static pointer
static HeltecLoRaComponent lora_obj;
HeltecLoRaComponent *g_heltec_lora = &lora_obj;

void register_heltec_lora_component(mqtt::MQTTClientComponent *mqtt) {
  g_heltec_lora->mqtt_ = mqtt;
  // Register it so ESPHome calls setup(), loop() etc.
  App.register_component(g_heltec_lora);
}

}  // namespace heltec_lora
}  // namespace esphome
