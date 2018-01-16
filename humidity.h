#pragma once

#include "mbed.h"

#define DHT11LIB_VERSION "0.4.1"

namespace iotea {
    namespace puerth {
        class HumiditySensor {
        public:
            HumiditySensor(PinName pin_) noexcept;
            int read() noexcept;

        private:
            DigitalInOut _pin;
            Timer _timer;
        };
    }
}
