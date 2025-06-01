#include "heltec_lora.h"
#include <heltec.h>                 // HeltecAutomation/Heltec ESP32
#include "esphome/core/log.h"       

namespace esphome {
namespace heltec_lora {

static const char *TAG = "heltec_lora";  

static mqtt::MQTTClientComponent *mqtt_{nullptr};
HeltecLoRaComponent *HeltecLoRaComponent::global = nullptr;

// ─── Component life-cycle ───────────────────────────────────────────────
void HeltecLoRaComponent::setup() {
  ESP_LOGI(TAG, "initialising SX1262 …");         

  // bool Display, bool LoRa, bool Serial, bool PABOOST, long Band
  Heltec.begin(false,       /* OLED off   */
               true,        /* LoRa on    */
               true,        /* Serial on  */
               true,        /* PA boost   */
               915E6);      /* 915 MHz    */

  global = this;
}

// ─── LoRa TX helper (called from your YAML lambdas) ────────────────────
void HeltecLoRaComponent::send_packet(const std::vector<uint8_t> &data) {
  ESP_LOGI(TAG, "TX → %.4s , %u B", data.data(), data.size());

  Heltec.LoRa.beginPacket();
  Heltec.LoRa.write(data.data(), data.size());
  Heltec.LoRa.endPacket();
}

// ─── simple C hooks so you don’t need C++ in YAML ──────────────────────
void register_heltec_lora_component(mqtt::MQTTClientComponent *mqtt) {
  mqtt_ = mqtt;
}

void heltec_lora_send(const std::vector<uint8_t> &data) {
  if (HeltecLoRaComponent::global)
    HeltecLoRaComponent::global->send_packet(data);
}

}  // namespace heltec_lora
}  // namespace esphome
