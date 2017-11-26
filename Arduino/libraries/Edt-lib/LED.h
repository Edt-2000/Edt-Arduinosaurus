#pragma once

#include <OSCArduino.h>
#include <OSCMessageConsumer.h>
#include <OSCMessageDefinitions.h>
#include <OnOffLEDColorScheduler.h>

class EdtLED : public OSC::StructMessageConsumer<OSC::ColorCommands>
{
private:
	const char * _pattern;
	int _pin;

	OnOffLEDColorScheduler _colorScheduler;

public:
	EdtLED(const char * pattern, int outputPin) : StructMessageConsumer(7) {
		_pattern = pattern;

		_pin = outputPin;
		pinMode(_pin, OUTPUT);

		_colorScheduler = OnOffLEDColorScheduler(_pin, false);
		_colorScheduler.output(0);

		addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::SinglePulse, &EdtLEDMessageBuffer::buffer);
		addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::SingleSolid, &EdtLEDMessageBuffer::buffer);
		addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::RainbowPulse, &EdtLEDMessageBuffer::buffer);
		addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::RainbowSolid, &EdtLEDMessageBuffer::buffer);
		addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::VuMeter, &EdtLEDMessageBuffer::buffer);
		addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::Twinkle, &EdtLEDMessageBuffer::buffer);
		addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::Strobo, &EdtLEDMessageBuffer::buffer);
	}
	
	const char * pattern() {
		return _pattern;
	}

	void test() {
		_colorScheduler.output(255);
		_colorScheduler.fade(4);
	}

	void callbackEnum(OSC::ColorCommands command) {
		switch(command) {

		case OSC::ColorCommands::SinglePulse:
		case OSC::ColorCommands::SingleSolid:

			if(EdtLEDMessageBuffer::buffer.singleColor.value > 0) {
				_colorScheduler.output(EdtLEDMessageBuffer::buffer.singleColor.value);
			}
			
			if (command == OSC::ColorCommands::SinglePulse || EdtLEDMessageBuffer::buffer.singleColor.value == 0) {
				_colorScheduler.fade(EdtLEDMessageBuffer::buffer.singleColor.duration);
			}
			else {
				_colorScheduler.disableFade();
			}

		 	break;

		case OSC::ColorCommands::RainbowPulse:
		case OSC::ColorCommands::RainbowSolid:

			_colorScheduler.output(127);
			
			if(command == OSC::ColorCommands::RainbowPulse) {
				_colorScheduler.fade(EdtLEDMessageBuffer::buffer.rainbow.duration);				
			}
			else {
				_colorScheduler.disableFade();
			}

			break;

		case OSC::ColorCommands::VuMeter:

			if (EdtLEDMessageBuffer::buffer.vuMeter.intensity > 0) {
				_colorScheduler.output(EdtLEDMessageBuffer::buffer.vuMeter.intensity / 2);
				_colorScheduler.disableFade();
			}

			break;

		case OSC::ColorCommands::Twinkle:
		
			_colorScheduler.disableFade();

			if (EdtLEDMessageBuffer::buffer.twinkle.intensity > 0) {

				if (EdtLEDMessageBuffer::buffer.twinkle.intensity > random8()) {
					_colorScheduler.output(255);
				}
				else {
					_colorScheduler.output(0);
				}
			}
			else {
				_colorScheduler.fade(127);
			}

			break;

		case OSC::ColorCommands::Strobo:

			_colorScheduler.strobo(EdtLEDMessageBuffer::buffer.strobo.intensity);

			break;
		}
	}

	void animationLoop() {
		
		_colorScheduler.loop();
	}
};
