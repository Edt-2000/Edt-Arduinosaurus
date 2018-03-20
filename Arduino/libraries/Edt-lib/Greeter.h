#pragma once

#include <OSCArduino.h>
#include <OSCMessageConsumer.h>
#include <OSCMessageProducer.h>
#include <OSCMessageDefinitions.h>

class EdtGreeter : public OSC::MessageConsumer, public OSC::MessageProducer
{
  public:
  	OSC::Message message;
	bool notRead = false;

	EdtGreeter(const char *pattern)
	{
		message = OSC::Message();
		message.setAddress(pattern);
		message.addInt(1);
	}

	void loop()
	{
		message.setValidData(notRead);
	}

	OSC::Message *generateMessage()
	{
		notRead = false;
		return &message;
	}

	const char *pattern()
	{
		return message.address;
	}

	void callbackMessage(OSC::Message *msg)
	{
		notRead = true;
	}
};
