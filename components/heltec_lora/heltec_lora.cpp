#include "heltec_lora.h"
#include <heltec.h>
#include "esphome/core/application.h"            //  <-- add
#include "esphome/core/log.h"

namespace esphome {
namespace heltec_lora {

static const char *TAG = "heltec_lora";

static mqtt::MQTTClientComponent *mqtt_{nullptr};
HeltecLoRaComponent *HeltecLoRaComponent::global = nullptr;

/* ───── Constructor: register with ESPHome ───── */
HeltecLoRaComponent::HeltecLoRaComponent() {
  App.register_component(this);                  // <-- KEY LINE
}

/* ───── global instance ───── */
static HeltecLoRaComponent HELTEC_LORA_INSTANCE;   // keeps object alive

/* ───── life-cycle ───── */
void HeltecLoRaComponent::setup() {
  ESP_LOGI(TAG, "initialising SX1262 …");
  Heltec.begin(false, true, true, true, 915E6);
  global = this;
}

/* ───── transmit helper ───── */
void HeltecLoRaComponent::send_packet(const std::vector<uint8_t> &data) {
  ESP_LOGI(TAG, "TX → %.4s , %u B", data.data(), data.size());
  Heltec.LoRa.beginPacket();
  Heltec.LoRa.write(data.data(), data.size());
  Heltec.LoRa.endPacket();
}

/* ───── C hooks ───── */
void register_heltec_lora_component(mqtt::MQTTClientComponent *mqtt) { mqtt_ = mqtt; }

void heltec_lora_send(const std::vector<uint8_t> &data) {
  if (HeltecLoRaComponent::global)
    HeltecLoRaComponent::global->send_packet(data);
}

}  // namespace heltec_lora
}  // namespace esphome
