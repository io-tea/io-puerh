#pragma once

#include "mbed.h"

#define DHT11LIB_VERSION "0.4.1"

namespace iotea {
    namespace puerth {
        class HumiditySensor {
        public:
            HumiditySensor(DigitalInOut pin_) noexcept;
            uint8_t read() noexcept;

        private:
            DigitalInOut pin_;
        };
    }
}
