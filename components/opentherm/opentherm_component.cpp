#include "opentherm_component.h"

namespace esphome {
namespace opentherm {

static OpenTherm *global_ot = nullptr;

void IRAM_ATTR OpenThermComponent::handle_interrupt() {
  if (global_ot != nullptr)
    global_ot->handleInterrupt();
}

OpenThermComponent::OpenThermComponent(int pIn, int pOut)
    : PollingComponent(1000), in_pin(pIn), out_pin(pOut) {
  ot = new OpenTherm(in_pin, out_pin);
  global_ot = ot;
}

void OpenThermComponent::setup() {
  ESP_LOGI("OpenTherm", "Initializing on IN=%d OUT=%d", in_pin, out_pin);
  ot->begin(handle_interrupt);
}

void OpenThermComponent::update() {
  bool enable_ch = target_temperature > 0.0f;

  unsigned long response = ot->setBoilerStatus(enable_ch, true, false);
  auto status = ot->getLastResponseStatus();

  switch (status) {
    case OpenThermResponseStatus::SUCCESS:
      if (target_temperature > 0.0f)
        ot->setBoilerTemperature(target_temperature);
      {
        float temp = ot->getBoilerTemperature();
        if (temp > 0.0f) boiler_temperature = temp;
        boiler_modulation = ot->getModulation();
        central_heating = ot->isCentralHeatingActive(response);
      }
      break;
    case OpenThermResponseStatus::INVALID:
      ESP_LOGE("OpenTherm", "Invalid response: 0x%08lX", response);
      break;
    case OpenThermResponseStatus::TIMEOUT:
      ESP_LOGW("OpenTherm", "Timeout waiting for response");
      break;
    case OpenThermResponseStatus::NONE:
      ESP_LOGE("OpenTherm", "OpenTherm communication uninitialized");
      break;
  }
}

void OpenThermComponent::set_target_temperature(float temperature) {
  ESP_LOGI("OpenTherm", "Setting target temperature: %.2f°C", temperature);
  target_temperature = temperature;
}

}  // namespace opentherm
}  // namespace esphome