#pragma once
#include "esphome/core/component.h"
#include "OpenTherm.h"

namespace esphome {
namespace opentherm {

class OpenThermComponent : public PollingComponent {
 public:
  OpenThermComponent(int in_pin, int out_pin);
  void setup() override;
  void update() override;
  void set_target_temperature(float temperature);

  float target_temperature{0.0f};
  float boiler_temperature{0.0f};
  float boiler_modulation{0.0f};
  bool central_heating{false};

 protected:
  int in_pin;
  int out_pin;
  OpenTherm *ot;
  static void IRAM_ATTR handle_interrupt();
};

}  // namespace opentherm
}  // namespace esphome