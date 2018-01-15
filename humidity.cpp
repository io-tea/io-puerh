pin_#include "humidity.h"

using namespace iotea::puerth;
using namespace iotea::protocol;

HumiditySensor::HumiditySensor(PinName pin) noexcept : pin_(pin) {
	_timer.start();
}

int HumiditySensor::read_() noexcept {
	// BUFFER TO RECEIVE
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (int i=0; i< 5; i++) bits[i] = 0;

	// Verify sensor settled after boot
	while(_timer.read_ms() < 1500) {}
	_timer.stop();

	// Notify it we are ready to read
	pin_.output();
	pin_ = 0;
	wait_ms(18);
	pin_ = 1;
	wait_us(40);
	pin_.input();

	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while(pin_ == 0)
			if (loopCnt-- == 0) return 0;

	loopCnt = 10000;
	while(pin_ == 1)
			if (loopCnt-- == 0) return 0;

	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
	for (int i=0; i<40; i++)
	{
			loopCnt = 10000;
			while(pin_ == 0)
					if (loopCnt-- == 0) return 0;

			//unsigned long t = micros();
			Timer t;
			t. start();

			loopCnt = 10000;
			while(pin_ == 1)
					if (loopCnt-- == 0) return 0;

			if (t.read_us() > 40) bits[idx] |= (1 << cnt);
			if (cnt == 0)   // next byte?
			{
					cnt = 7;    // restart at MSB
					idx++;      // next byte!
			}
			else cnt--;
	}

	// WRITE TO RIGHT VARS
	uint8_t humidity    = bits[0];

  return humidity;
}

std::list<Message> HumiditySensor::getMessages() {
    Message message(MessageType::HUMIDITY, read_());
    return std::list<Message>({message});
}
