#include "io-tea/node/node.h"
#include "mbed.h"
#include "humidity.h"

int main() {
  iotea::node::Node node(iotea::NodeName::NODE_PUERTH);
  node.addSensor(std::make_unique<iotea::puerth::HumiditySensor>(D3)); //change pin
  node.run();
}
