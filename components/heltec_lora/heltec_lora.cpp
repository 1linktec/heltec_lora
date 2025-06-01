#include "heltec_lora.h"

namespace esphome {
namespace heltec_lora {

HeltecLoRaComponent *g_heltec_lora = nullptr;

void HeltecLoRaComponent::setup() {
  Heltec.begin(false /*Display*/, true /*LoRa*/, true /*Serial*/, true /*PABoost*/, 915E6);
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  ESP_LOGI("heltec_lora", "LoRa radio initialised");
}

void HeltecLoRaComponent::loop() { this->handle_rx_(); }

void HeltecLoRaComponent::send_packet(const std::vector<uint8_t> &data) {
  LoRa.beginPacket();
  for (auto b : data)
    LoRa.write(b);
  LoRa.endPacket();
  ESP_LOGI("heltec_lora", "TX %u bytes", (unsigned) data.size());
}

void HeltecLoRaComponent::handle_rx_() {
  int len = LoRa.parsePacket();
  if (len <= 0) return;

  std::vector<uint8_t> buf;
  while (LoRa.available())
    buf.push_back(LoRa.read());

  ESP_LOGI("heltec_lora", "RX %u bytes", (unsigned) buf.size());

  if (mqtt_ != nullptr) {
    mqtt_->publish("lora/raw", std::string(buf.begin(), buf.end()));
  }
}

/* --------- helpers --------- */

void register_heltec_lora_component(mqtt::MQTTClientComponent *mqtt) {
  g_heltec_lora = new HeltecLoRaComponent();
  g_heltec_lora->mqtt_ = mqtt;
  App.register_component(g_heltec_lora);
}

void heltec_lora_send(const std::vector<uint8_t> &data) {
  if (g_heltec_lora != nullptr)
    g_heltec_lora->send_packet(data);
}

}  // namespace heltec_lora
}  // namespace esphome
