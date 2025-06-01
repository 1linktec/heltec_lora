// heltec_lora.cpp
#include <Heltec.h>              
#include "heltec_lora.h"
#include "esphome/core/log.h"

namespace esphome {
namespace heltec_lora {

static const char *TAG = "heltec_lora";

static mqtt::MQTTClientComponent *mqtt_{nullptr};
HeltecLoRaComponent *HeltecLoRaComponent::global = nullptr;

// ─── life-cycle ────────────────────────────────────────────────────────────
void HeltecLoRaComponent::setup() {
  ESP_LOGI(TAG, "initialising SX1262 …");

  // board helper just brings up SPI, RF switch, etc.
  Heltec.begin(false, true, true, true, 915E6);

  // radio driver itself
  if (!LoRa.begin(915E6)) {
    ESP_LOGE(TAG, "LoRa init failed!");
    return;
  }
  global = this;
}

// ─── TX helper ─────────────────────────────────────────────────────────────
void HeltecLoRaComponent::send_packet(const std::vector<uint8_t> &data) {
  ESP_LOGI(TAG, "TX → %.4s , %u B", data.data(), data.size());
  LoRa.beginPacket();
  LoRa.write(data.data(), data.size());
  LoRa.endPacket(/*async =*/false);
}

// ─── C wrappers for YAML lambdas ───────────────────────────────────────────
void register_heltec_lora_component(mqtt::MQTTClientComponent *mqtt) { mqtt_ = mqtt; }
void heltec_lora_send(const std::vector<uint8_t> &data) {
  if (HeltecLoRaComponent::global) HeltecLoRaComponent::global->send_packet(data);
}

}  // namespace heltec_lora
}  // namespace esphome
