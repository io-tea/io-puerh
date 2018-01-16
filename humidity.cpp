#include "humidity.h"

using namespace iotea::puerth;

HumiditySensor::HumiditySensor(PinName pin) noexcept : _pin(pin) {
	_timer.start();
}

int HumiditySensor::read() noexcept {
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
	_pin.output();
	_pin = 0;
	wait_ms(18);
	_pin = 1;
	wait_us(40);
	_pin.input();

	// ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = 10000;
	while(_pin == 0)
			if (loopCnt-- == 0) return 0;

	loopCnt = 10000;
	while(_pin == 1)
			if (loopCnt-- == 0) return 0;

	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
	for (int i=0; i<40; i++)
	{
			loopCnt = 10000;
			while(_pin == 0)
					if (loopCnt-- == 0) return 0;

			//unsigned long t = micros();
			Timer t;
			t. start();

			loopCnt = 10000;
			while(_pin == 1)
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
