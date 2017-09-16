#pragma once

#include <OSCArduino.h>
#include <OSCMessageConsumer.h>
#include <OSCMessageDefinitions.h>
#include <FastLED.h>
#include <FastLEDColorScheduler.h>

class EdtRGBLED : public OSC::AMessageConsumerUsingStructs<OSC::ColorCommands>
{
private:
	const char * _pattern;
	CRGB *_leds;
	int _nrOfLeds;
	
	int _start;
	int _end;
	int _length;

	FastLEDColorScheduler _colorScheduler;

	CHSV chsv(OSC::Color color) {
		return CHSV(color.hue, color.saturation, color.value);
	}

	OSC::SingleColorCommand singleColor;

public:
	
	EdtRGBLED(const char * pattern, uint8_t const nrOfLeds) {
		_pattern = pattern;

		_nrOfLeds = nrOfLeds;
		_leds = new CRGB[nrOfLeds];

		_colorScheduler = FastLEDColorScheduler(_leds, nrOfLeds);

		stageStruct(SinglePulse, &singleColor);
		stageStruct(SingleSolid, &singleColor);
	}

	const char * address() {
		return _pattern;
	}

	template<uint8_t dataPin, uint8_t clockPin>	void configurePins() {
		FastLED.addLeds<APA102, dataPin, clockPin, BRG>(_leds, _nrOfLeds);

		OSC::Color startColor = { 32, 32, 32 };
		fill_solid(_leds, _nrOfLeds, chsv(startColor));
	}

	void test() {
		OSC::Color startPulse = { 255, 255, 255 };
		fill_solid(_leds, _nrOfLeds, chsv(startPulse));

		_colorScheduler.blackout(0, _nrOfLeds, 2);
	}

	void callbackEnum(OSC::ColorCommands command) {
		

		switch(command) {	

		case SinglePulse:
		case SingleSolid:

			_start = singleColor.getRealStart();
			_end = singleColor.getRealEnd();
			_length = end - start;

			if(singleColor.color.value > 0) {
				fill_solid(_leds + _start, _length, chsv(singleColor.color));
			}

			if(command == SinglePulse || singleColor.color.value == 0) {
				_colorScheduler.blackout(_start, _end, singleColor.duration);
			}
			else {
				_colorScheduler.disableBlackout(_start, _end);
			}

			break;
		}
		

		// case RainbowPulse:
		// case RainbowSolid:

		// 	_start = floorf((msg->getInt(1) / 127.0) * _nrOfLeds);
		// 	_end = ceilf((msg->getInt(2) / 127.0) * _nrOfLeds);
			
		// 	_hue = msg->getInt(3);
		// 	_deltahue = round((msg->getInt(4) / 127.0) * (255.0 / _nrOfLeds));

		// 	if (_deltahue > 0) {
		// 		fill_rainbow(&_leds[_start], _end - _start, _hue, _deltahue);
		// 	}

		// 	if (_mode == RainbowPulse || _deltahue == 0) {
		// 		_duration = msg->getInt(5);

		// 		colorScheduler.blackout(_start, _end, _duration);
		// 	}
		// 	else {
		// 		colorScheduler.disableBlackout(_start, _end);
		// 	}

		// 	break;


		// case VUMeter:

		// 	_start = floorf((msg->getInt(1) / 127.0) * _nrOfLeds);
		// 	_center = round((msg->getInt(3) / 127.0) * _nrOfLeds);
		// 	_end = ceilf((msg->getInt(2) / 127.0) * _nrOfLeds);

		// 	_hue = msg->getInt(4);
		// 	_deltahue = msg->getInt(5);
		// 	_intensity = (float)(msg->getInt(6));

		// 	if (_start != _center) {
				
		// 		int leds = (_center - _start) * (_intensity / 255.0);

		// 		colorScheduler.blackout(_start, _center - leds, 127);
		// 		colorScheduler.disableBlackout(_center - leds, _center);

		// 		fill_rainbow_reverse(&_leds[_center - leds - 1], leds, _hue, _deltahue / (_center - _start));
		// 	}

		// 	if (_center != _end) {

		// 		int leds = (_end - _center) * (_intensity / 255.0);

		// 		colorScheduler.blackout(_end - (_end - _center - leds), _end, 127);
		// 		colorScheduler.disableBlackout(_center, _center + leds);

		// 		fill_rainbow(&_leds[_center], leds, _hue, _deltahue / (_end - _center));
		// 	}

		// 	break;

		// case Twinkle:
		
		// 	_start = floorf((msg->getInt(1) / 127.0) * _nrOfLeds);
		// 	_end = ceilf((msg->getInt(2) / 127.0) * _nrOfLeds);

		// 	colorScheduler.disableBlackout(_start, _end);

		// 	_color1.h = msg->getInt(3);
		// 	_color1.s = 255;
		// 	_color1.l = 255;
		// 	_intensity = (float)(msg->getInt(4));

		// 	_color2.l = 0;

		// 	if (_intensity > 0) {

		// 		for (int i = _start; i < _end; i++) {
		// 			if (_intensity > random8()) {
		// 				_leds[i] = _color1.chsv();
		// 			}
		// 			else {
		// 				_leds[i] = _color2.chsv();
		// 			}
		// 		}
		// 	}
		// 	else {
		// 		colorScheduler.blackout(_start, _end, 127);
		// 	}

		// 	break;

		// case Strobo:

		// 	// stobo only has 2 parameters
		// 	_hue = msg->getInt(1);
		// 	_intensity = msg->getInt(2);

		// 	colorScheduler.strobo(_hue, _intensity);

		// 	break;
		// }
	}

	void animationLoop() {
		
		_colorScheduler.loop();
	}
};
