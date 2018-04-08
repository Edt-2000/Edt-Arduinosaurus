#pragma once

#include "AbstractApplication.h"

#include "Definitions.h"

#include "Arduino.h"
#include "OSCArduino.h"
#include "Ethernet.h"
#include "EthernetUdp.h"
#include "Statemachine.h"
#include "Time.h"

bool messageSend = false;

class TestDevice : public OSC::MessageConsumer<OSC::Message>, public OSC::MessageProducer<OSC::Message>
{
  private:
	OSC::Message _message;

	long _sendTime;

  public:
	TestDevice(const char *pattern) : MessageConsumer()
	{
		_message.setAddress(pattern);
		_message.reserveAtLeast(2);
		_message.addInt(0);
		_message.addInt(0);
		_message.setValidData(true);

		_sendTime = micros();
	}

	const char *pattern()
	{
		return _message.address;
	}

	void loop()
	{
	}

	void callbackMessage(OSC::Message *message)
	{
		auto now = micros();
		auto i = message->getInt(0);

		_message.setInt(0, i + 1);
		_message.setInt(1, now - _sendTime);
		_message.setValidData(false);

		digitalWrite(13, (i % 2) ? HIGH : LOW);

		messageSend = false;
	}

	OSC::Message *generateMessage()
	{
		_sendTime = micros();

		_message.setValidData(!messageSend);

		messageSend = true;

		return &_message;
	}
};

class PerformanceApplication : public AbstractApplication
{
  public:
	OSC::Arduino<OSC::Message> osc;

	TestDevice device = TestDevice("/TD");

	EthernetUDP udp;

	void setupStatus()
	{
		status.setup(13, HIGH);
	}

	void setupNetwork()
	{
		Serial.begin(57600);

		Ethernet.begin(MAC_LED, IP_TRAK);

		udp.begin(PORT_BROADCAST);
	}

	void setupOsc()
	{
		osc = OSC::Arduino<OSC::Message>(1, 1);
		osc.bindBoth(&udp, IP_BROADCAST, PORT_BROADCAST, &Serial);
		osc.addConsumer(&device);
		osc.addProducer(&device);

		messageSend = false;
	}

	void applicationLoop()
	{
		// slow down the application
		// otherwise, partial messages from Serial are being read and those will fail to parse.
		// on uno's only, since the usb works differently on a Leonardo
		osc.loop(time.tOSC);
	}
};