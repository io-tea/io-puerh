#include "humidity.h"

using namespace iotea::puerth;
using namespace iotea::protocol;

HumiditySensor::HumiditySensor(PinName pin) noexcept : pin_(pin) {}

int HumiditySensor::read_() noexcept {
  // BUFFER TO RECEIVE
	uint8_t bits[5];
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (int i=0; i< 5; i++) bits[i] = 0;

	// REQUEST SAMPLE
	pinMode(pin_, OUTPUT);
	digitalWrite(pin_, LOW);
	delay(18);
	digitalWrite(pin_, HIGH);
	delayMicroseconds(40);
	pinMode(pin_, INPUT);

	unsigned int loopCnt = 10000;
	// READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
	for (int i=0; i<40; i++)
	{
		loopCnt = 10000;
		while(digitalRead(pin) == LOW)
			if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

		unsigned long t = micros();

		loopCnt = 10000;
		while(digitalRead(pin) == HIGH)
			if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

		if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
		if (cnt == 0)   // next byte?
		{
			cnt = 7;    // restart at MSB
			idx++;      // next byte!
		}
		else cnt--;
	}

	// WRITE TO RIGHT VARS
	int humidity    = bits[0];
	int temperature = bits[2];

  return humidity;
}

std::list<Message> HumiditySensor::getMessages() {
    Message message(MessageType::HUMIDITY, read_());
    return std::list<Message>({message});
}
