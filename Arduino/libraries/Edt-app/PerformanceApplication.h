#pragma once

#include "AbstractApplication.h"

#include "Definitions.h"

#include "Arduino.h"
#include "OSCArduino.h"
#include "Statemachine.h"
#include "Time.h"

struct Simple
{
	uint8_t int0 : 8;
};

class TestDevice : public OSC::MessageConsumer<OSC::Message>
{
  private:
	const char *_pattern;
    bool _state = false;
  public:
	TestDevice(const char *pattern) : MessageConsumer()
	{
		_pattern = pattern;
	}

	const char *pattern()
	{
		return _pattern;
	}

	void callbackMessage(OSC::Message *message)
	{
		_state = !_state;
		digitalWrite(13, _state ? HIGH : LOW);
	}
};

class PerformanceApplication : public AbstractApplication
{
  public:
	OSC::Arduino<OSC::Message> osc;

	TestDevice device = TestDevice("/TD");

	void setupStatus()
	{
		status.setup(13, HIGH);
	}

	void setupNetwork()
	{
		Serial.begin(57600);


		//Ethernet.begin(MAC_PING, IPAddress(10, 0, 0, 20));

		//udp.begin(PORT_BROADCAST);
	}

	void setupOsc()
	{
		osc = OSC::Arduino<OSC::Message>(1, 0);
		//osc.bindUDP(&udp, IPAddress(10, 0, 0, 1), 12345);
		osc.bindStream(&Serial);
		osc.addConsumer(&device);
	}

	void applicationLoop()
	{
		osc.loop(time.tOSC);
	}
};