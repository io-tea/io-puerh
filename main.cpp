#include "io-tea/node.h"
#include "mbed.h"
#include "humidity.h"

iotea::puerth::HumiditySensor humiditySensor = iotea::puerth::HumiditySensor(D3);

int main() {
  setupSerial("PUERH - humidity sensor");
  setupNodeRadio(0xABCDEF01);
  setupTicker();

  time_t lastStatsTime = time(nullptr) - 1;
  for (int tick = 0;; ++tick) {
      time_t now = time(nullptr);
      if (lastStatsTime != now) {
          lastStatsTime = now;
          printStatus();
      }

      if (tick) {
          tick = false;
          sendCoapMessage("g", std::to_string(humiditySensor.read()));
      }
  }
}
