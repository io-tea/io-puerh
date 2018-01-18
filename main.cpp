#include "mbed.h"
#include "io-tea/node.h"
#include "humidity.h"

iotea::puerth::HumiditySensor humiditySensor(D3);

int main() {
    setupSerial("PUERH - humidity sensor");
    setupNodeRadio(0xABCDEF01);
    setupTicker();

    time_t lastStatsTime = time(nullptr) - 1;
    while (true) {
        sleep();

        time_t now = time(nullptr);
        if (lastStatsTime != now) {
            lastStatsTime = now;
            printStatus();
        }

        if (tick) {
            tick = false;

            uint8_t humidity = 0;
            while(humidity == 0 || humidity > 120) {
              humidity = humiditySensor.read();
            }
            sendCoapMessage("h", std::to_string(humidity));
        }
    }
}
