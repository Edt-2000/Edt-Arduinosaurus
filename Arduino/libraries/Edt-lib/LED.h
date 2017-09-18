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
	
	OSC::SingleColorCommand singleColor;
	OSC::RainbowCommand rainbow;
	OSC::VuMeterCommand vuMeter;
	OSC::TwinkleCommand twinkle;
	OSC::StroboCommand strobo;

	EdtLED(const char * pattern, int outputPin) {
		_pattern = pattern;

		_pin = outputPin;
		pinMode(_pin, OUTPUT);

		_colorScheduler = OnOffLEDColorScheduler(_pin, false);
		_colorScheduler.output(0);

		stageStruct(OSC::ColorCommands::SinglePulse, &singleColor, sizeof(OSC::SingleColorCommand));
		stageStruct(OSC::ColorCommands::SingleSolid, &singleColor, sizeof(OSC::SingleColorCommand));
		stageStruct(OSC::ColorCommands::RainbowPulse, &rainbow, sizeof(OSC::RainbowCommand));
		stageStruct(OSC::ColorCommands::RainbowSolid, &rainbow, sizeof(OSC::RainbowCommand));
		stageStruct(OSC::ColorCommands::VuMeter, &vuMeter, sizeof(OSC::VuMeterCommand));
		stageStruct(OSC::ColorCommands::Twinkle, &twinkle, sizeof(OSC::TwinkleCommand));
		stageStruct(OSC::ColorCommands::Strobo, &strobo, sizeof(OSC::StroboCommand));
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

			if(singleColor.value > 0) {
				_colorScheduler.output(singleColor.value);
			}
			
			if (command == OSC::ColorCommands::SinglePulse || singleColor.value == 0) {
				_colorScheduler.fade(singleColor.duration);
			}
			else {
				_colorScheduler.disableFade();
			}

		 	break;

		case OSC::ColorCommands::RainbowPulse:
		case OSC::ColorCommands::RainbowSolid:

			_colorScheduler.output(127);
			
			if(command == OSC::ColorCommands::RainbowPulse) {
				_colorScheduler.fade(rainbow.duration);				
			}
			else {
				_colorScheduler.disableFade();
			}

			break;

		case OSC::ColorCommands::VuMeter:

			if (vuMeter.intensity > 0) {
				_colorScheduler.output(vuMeter.intensity / 2);
				_colorScheduler.disableFade();
			}

			break;

		case OSC::ColorCommands::Twinkle:
		
			_colorScheduler.disableFade();

			if (twinkle.intensity > 0) {

				if (twinkle.intensity > random8()) {
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

			_colorScheduler.strobo(strobo.intensity);

			break;
		}
	}

	void animationLoop() {
		
		_colorScheduler.loop();
	}
};
