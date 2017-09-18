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
	
	int _start;
	int _center;
	int _end;
	int _length;

	FastLEDColorScheduler _colorScheduler;

	inline int normalizeLedNrDown(int percentage) {
		return floorf((percentage / 127.0) * _nrOfLeds);
	}
	inline int normalizeLedNrUp(int percentage) {
		return ceilf((percentage / 127.0) * _nrOfLeds);
	}

public:
	
	OSC::SingleColorCommand singleColor;
	OSC::RainbowCommand rainbow;
	OSC::VuMeterCommand vuMeter;
	OSC::TwinkleCommand twinkle;
	OSC::StroboCommand strobo;

	EdtRGBLED(const char * pattern, uint8_t const nrOfLeds) {
		_pattern = pattern;

		_nrOfLeds = nrOfLeds;
		_leds = new CRGB[nrOfLeds];

		_colorScheduler = FastLEDColorScheduler(_leds, nrOfLeds);

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

			_start = normalizeLedNrDown(singleColor.start);
			_end = normalizeLedNrUp(singleColor.end);
			_length = _end - _start;

			if(singleColor.value > 0) {
				fill_solid(_leds + _start, _length, CHSV(singleColor.hue, singleColor.saturation, singleColor.value));
			}

			if(command == OSC::ColorCommands::SinglePulse || singleColor.value == 0) {
				_colorScheduler.fade(_start, _end, singleColor.duration);
			}
			else {
				_colorScheduler.disableFade(_start, _end);
			}

			break;
		
		 case OSC::ColorCommands::RainbowPulse:
		 case OSC::ColorCommands::RainbowSolid:

			_start = normalizeLedNrDown(rainbow.start);
			_end = normalizeLedNrUp(rainbow.end);
			_length = _end - _start;
			
			if(rainbow.deltaHue > 0) {
				fill_rainbow(&_leds[_start], _length, rainbow.hue, rainbow.deltaHue);
			}

		 	if (command == OSC::ColorCommands::RainbowPulse || rainbow.deltaHue == 0) {
		 		_colorScheduler.fade(_start, _end, rainbow.duration);
		 	}
		 	else {
		 		_colorScheduler.disableFade(_start, _end);
		 	}

		 	break;

		 case OSC::ColorCommands::VuMeter:

		 	_start = normalizeLedNrDown(vuMeter.start);
		 	_center = normalizeLedNrDown(vuMeter.center);
		 	_end = normalizeLedNrUp(vuMeter.end);

			if (_start != _center) {
				
				int leds = (_center - _start) * (vuMeter.intensity / 255.0);

				_colorScheduler.fade(_start, _center - leds, 127);
				_colorScheduler.disableFade(_center - leds, _center);

				fill_rainbow_reverse(&_leds[_center - leds - 1], leds, vuMeter.hue, vuMeter.deltaHue / (_center - _start));
			}
			if (_center != _end) {

				int leds = (_end - _center) * (vuMeter.intensity / 255.0);

				_colorScheduler.fade(_end - (_end - _center - leds), _end, 127);
				_colorScheduler.disableFade(_center, _center + leds);

				fill_rainbow(&_leds[_center], leds, vuMeter.hue, vuMeter.deltaHue / (_end - _center));
			}

			break;

		 case OSC::ColorCommands::Twinkle:

			_start = normalizeLedNrDown(twinkle.start);
			_end = normalizeLedNrDown(twinkle.end);

			_colorScheduler.disableFade(_start, _end);
			
		 	if (twinkle.intensity > 0) {

		 		for (int i = _start; i < _end; i++) {
		 			if (twinkle.intensity > random8()) {
		 				_leds[i] = CHSV(twinkle.hue, 240, 255);
		 			}
		 			else {
		 				_leds[i] = CHSV(0,0,0);
		 			}
		 		}
		 	}
		 	else {
		 		_colorScheduler.fade(_start, _end, 127);
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
