# Control you boiler with ESPHome and OpenTherm protocol

**Based on:**
* [OpenTherm Arduino/ESP8266/ESP32 Library](https://github.com/ihormelnyk/opentherm_library)
* [OpenTherm Adapter](https://ihormelnyk.com/opentherm_adapter)

**Main board** - ESP8266 WeMos D1 Mini, or other Arduino/ESP32 board. In this example ESP32 board used.

**Instruction:**
* Create file "esp_opentherm.h" in folder "config\esphome\opentherm", where "config" is HA configuration folder.
* Set **inPin** and **outPin**
  * 2 and 3 for Arduino
  * 4 and 5 for ESP8266
  * 21 and 22 for ESP32
* Edit your ".yaml" file like "opentherm_boiler.yaml"
* Connect board and adapter to tour boiler
* Enjoy :)


**Service esphome.opentherm_boiler_set_target_temperature** - set heating temperature. 0 for disable heating:

```
- service: esphome.opentherm_boiler_set_target_temperature
  data:
    temperature: 60
```