#pragma once

#include "io-tea/sensor/sensor.h"

#include "mbed.h"

#define DHT11LIB_VERSION "0.4.1"

namespace iotea {
    namespace puerth {
        class HumiditySensor : public iotea::sensor::Sensor {
        public:
            HumiditySensor(PinName pin_) noexcept;
            virtual std::list<protocol::Message> getMessages() override;

        private:
            AnalogIn pin_;
            int read_() noexcept;
        };
    }
}
