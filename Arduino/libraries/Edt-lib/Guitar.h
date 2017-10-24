#pragma once

#include <OSCArduino.h>
#include <OSCMessageConsumer.h>
#include <OSCMessageProducer.h>

#include <Wire.h>

#include <MCP3008.h>
#include <MCP23016.h>

class EdtGuitar : public OSC::MessageProducer
{
  private:
	MCP3008 _mcp3008 = MCP3008(1, 1, 4, 12, 13, 16);
	MCP23016 _mcp23016 = MCP23016(0);

	const char *_pattern;
	OSC::Message _message;

  public:
	EdtGuitar(const char *pattern)
	{
		_pattern = pattern;

		Wire.begin();

		_mcp3008.begin();

		_mcp23016.mode(6, OUTPUT);
		_mcp23016.mode(7, OUTPUT);

		_mcp23016.begin();

		_message = OSC::Message();
		_message.setAddress(_pattern);
		_message.reserveAtLeast(4);
		_message.addInt(0);
		_message.addInt(0);
		_message.addInt(0);
		_message.addInt(0);
	}

	const char *pattern()
	{
		return _pattern;
	}

	void loop()
	{

		//_message.setInt(0, input);
		//_message.setInt(1, _mcp3008.analogRead(7));

		bool f = false;

		while (true)
		{
			int input = _mcp3008.analogRead(0);

			_mcp23016.digitalWrite(6, input > 512 ? HIGH : LOW);
			_mcp23016.digitalWrite(7, input > 512 ? HIGH : LOW);

			//_message.setInt(2, _mcp23016.digitalRead(0));
			//_message.setInt(3, _mcp23016.digitalRead(1));

			_mcp23016.cycle();
			delay(100);

			f = !f;

			digitalWrite(5, f ? HIGH : LOW);
		}
	}

	OSC::Message *generateMessage()
	{
		_message.setValidData(true);

		return &_message;
	}

	void animationLoop()
	{
	}
};
