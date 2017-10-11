#pragma once

#include <OSCArduino.h>
#include <OSCMessageConsumer.h>
#include <OSCMessageDefinitions.h>
#include <FastLED.h>
#include <FastLEDColorScheduler.h>

class EdtRGBLED : public OSC::StructMessageConsumer<OSC::ColorCommands>
{
private:
	const char * _pattern;
	CRGB *_leds;
	int _nrOfLeds;

	FastLEDColorScheduler _colorScheduler;

public:
	
	OSC::SingleColorCommand singleColor;
	OSC::RainbowCommand rainbow;
	OSC::VuMeterCommand vuMeter;
	OSC::TwinkleCommand twinkle;
	OSC::StroboCommand strobo;

	EdtRGBLED(const char * pattern, uint8_t const nrOfLeds) : StructMessageConsumer(7) {
		_pattern = pattern;

		_nrOfLeds = nrOfLeds;
		_leds = new CRGB[nrOfLeds];

		_colorScheduler = FastLEDColorScheduler(_leds, nrOfLeds);

		addEnumToStructMapping<OSC::SingleColorCommand>(OSC::ColorCommands::SinglePulse, &singleColor);
		addEnumToStructMapping<OSC::SingleColorCommand>(OSC::ColorCommands::SingleSolid, &singleColor);
		addEnumToStructMapping<OSC::RainbowCommand>(OSC::ColorCommands::RainbowPulse, &rainbow);
		addEnumToStructMapping<OSC::RainbowCommand>(OSC::ColorCommands::RainbowSolid, &rainbow);
		addEnumToStructMapping<OSC::VuMeterCommand>(OSC::ColorCommands::VuMeter, &vuMeter);
		addEnumToStructMapping<OSC::TwinkleCommand>(OSC::ColorCommands::Twinkle, &twinkle);
		addEnumToStructMapping<OSC::StroboCommand>(OSC::ColorCommands::Strobo, &strobo);
	}

	const char * pattern() {
		return _pattern;
	}

	template<uint8_t dataPin, uint8_t clockPin>	void configurePins() {
		FastLED.addLeds<APA102, dataPin, clockPin, BRG>(_leds, _nrOfLeds);

		fill_solid(_leds, _nrOfLeds, CHSV(32, 240, 32));
	}

	void test() {
		fill_solid(_leds, _nrOfLeds, CHSV(0, 240, 255));
		_colorScheduler.fade(0, _nrOfLeds, 2);
	}

	void callbackEnum(OSC::ColorCommands command) {
		
		switch(command) {	

		case OSC::ColorCommands::SinglePulse:
		case OSC::ColorCommands::SingleSolid:

			if(singleColor.value > 0) {
				_colorScheduler.solid(singleColor.start, singleColor.end, singleColor.hue, singleColor.saturation, singleColor.value);
			}

			if(command == OSC::ColorCommands::SinglePulse || singleColor.value == 0) {
				_colorScheduler.fade(singleColor.start, singleColor.end, singleColor.duration);
			}
			else {
				_colorScheduler.disableFade(singleColor.start, singleColor.end);
			}

			break;
		
		 case OSC::ColorCommands::RainbowPulse:
		 case OSC::ColorCommands::RainbowSolid:

			if(rainbow.deltaHue > 0) {
				_colorScheduler.rainbow(rainbow.start, rainbow.end, rainbow.hue, rainbow.deltaHue);
			}

		 	if (command == OSC::ColorCommands::RainbowPulse || rainbow.deltaHue == 0) {
		 		_colorScheduler.fade(rainbow.start, rainbow.end, rainbow.duration);
		 	}
		 	else {
		 		_colorScheduler.disableFade(rainbow.start, rainbow.end);
		 	}

		 	break;

		 case OSC::ColorCommands::VuMeter:

			 _colorScheduler.rainbow(vuMeter.start, vuMeter.center, vuMeter.end, vuMeter.hue, vuMeter.deltaHue, vuMeter.intensity);
			 
		 	break;

		 case OSC::ColorCommands::Twinkle:

			 _colorScheduler.disableFade(twinkle.start, twinkle.end);
			
		 	if (twinkle.intensity > 0) {
				_colorScheduler.twinkle(twinkle.start, twinkle.end, twinkle.hue, 240, 255, twinkle.intensity);		 		
		 	}
		 	else {
		 		_colorScheduler.fade(twinkle.start, twinkle.end, 127);
		 	}

			break;

		case OSC::ColorCommands::Strobo:

			_colorScheduler.strobo(strobo.hue, strobo.intensity);

			break;
		}
	}

	void animationLoop() {
		
		_colorScheduler.loop();
	}
};
