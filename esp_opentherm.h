#include "esphome.h"
#include "OpenTherm.h"

const int inPin = 21; //2 for Arduino, 4 for ESP8266 (D2), 21 for ESP32
const int outPin = 22; //3 for Arduino, 5 for ESP8266 (D1), 22 for ESP32

float target_temperature = 0,
      boiler_temperature = 0,
      boiler_modulation = 0,
      return_temperature = 0, //optional, may be 0 with your boiler
      boiler_pressure = 0; //optional, may be 0 with your boiler

bool central_heating = false;

OpenTherm ot(inPin, outPin);

void ICACHE_RAM_ATTR handleInterrupt()
{
    ot.handleInterrupt();
}

class OpenThermComponent : public PollingComponent, public CustomAPIDevice
{
public:
    OpenThermComponent() : PollingComponent(1000) {}

    void setup() override
    {
        register_service(&OpenThermComponent::set_target_temperature, "set_target_temperature", {"temperature"});

        ESP_LOGI("OpenThermComponent", "Setup");
        ot.begin(handleInterrupt);
    }

    void update() override
    {
        bool enableCentralHeating = target_temperature > 0;

        unsigned long response = ot.setBoilerStatus(enableCentralHeating, true, false);
        OpenThermResponseStatus responseStatus = ot.getLastResponseStatus();

        if (responseStatus == OpenThermResponseStatus::SUCCESS)
        {
            if (target_temperature > 0)
            {
                ot.setBoilerTemperature(target_temperature);
            }

            float temp = ot.getBoilerTemperature();
            if (temp > 0)
            {
                boiler_temperature = temp;
            }

            boiler_modulation = ot.getModulation();
            return_temperature = ot.getReturnTemperature();
            boiler_pressure = ot.getPressure();
            central_heating = ot.isCentralHeatingActive(response);
        }
        else if (responseStatus == OpenThermResponseStatus::NONE)
        {
            ESP_LOGE("OpenThermComponent", "Error: OpenTherm is not initialized");
        }
        else if (responseStatus == OpenThermResponseStatus::INVALID)
        {
            ESP_LOGE("OpenThermComponent", "Error: Invalid boiler response %lu", response);
        }
        else if (responseStatus == OpenThermResponseStatus::TIMEOUT)
        {
            ESP_LOGW("OpenThermComponent", "Error: Response timeout");
        }
    }

    void set_target_temperature(float temperature)
    {
        ESP_LOGI("OpenThermApi", "Set target temperature to %.2f", temperature);
        target_temperature = temperature;
    }
};

class OpenThermSensor : public PollingComponent
{
public:
    Sensor *target_temperature_sensor = new Sensor();
    Sensor *boiler_temperature_sensor = new Sensor();
    Sensor *boiler_modulation_sensor = new Sensor();
    Sensor *return_temperature_sensor = new Sensor();
    Sensor *boiler_pressure_sensor = new Sensor();

    OpenThermSensor() : PollingComponent(5000) {}

    void update() override
    {
        target_temperature_sensor->publish_state(target_temperature);
        boiler_temperature_sensor->publish_state(boiler_temperature);
        boiler_modulation_sensor->publish_state(boiler_modulation);
        return_temperature_sensor->publish_state(return_temperature);
        boiler_pressure_sensor->publish_state(boiler_pressure);
    }
};

class OpenThermBinarySensor : public PollingComponent, public BinarySensor
{
public:
    OpenThermBinarySensor() : PollingComponent(5000) {}

    void update() override
    {
        publish_state(central_heating);
    }
};
