#pragma once

#include <OSCArduino.h>
#include <OSCMessageConsumer.h>
#include <OSCMessageDefinitions.h>
#include <OnOffLEDColorScheduler.h>

// TODO: remove this macro
// #define normalize(x) x

class EdtLED : public OSC::AMessageConsumerUsingStructs<OSC::ColorCommands>
{
private:
	const char * _pattern;
	int _pin;

	OnOffLEDColorScheduler _colorScheduler;

public:
	
	EdtLED(const char * pattern, int outputPin) {
		_pattern = pattern;

		_pin = outputPin;
		pinMode(_pin, OUTPUT);

		analogWrite(_pin, normalize(0));
		
		_colorScheduler = OnOffLEDColorScheduler(_pin);
	}

	const char * address() {
		return _pattern;
	}

	void test() {
		analogWrite(_pin, normalize(255));
		_colorScheduler.blackout(4);
	}

	void callbackMode(OSC::ColorCommands command) { // OSC::Message * msg) {
		switch(command) {

		}

		// _mode = (Mode)msg->getInt(0);

		// switch (_mode) {

		// case SinglePulse:
		// case SingleSolid:

		// 	_l = msg->getInt(5);
			
		// 	if (_l > 0) {
		// 		analogWrite(_pin, normalize(_l));
		// 	}

		// 	if (_mode == SinglePulse || _l == 0) {
		// 		_duration = msg->getInt(6);
				
		// 		colorScheduler.blackout(_duration);
		// 	}
		// 	else {
		// 		colorScheduler.disableBlackout();
		// 	}

		// 	break;

		// case RainbowPulse:
		// case RainbowSolid:

		// 	_l = 255;

		// 	analogWrite(_pin, normalize(_l));
			
		// 	if(_mode == RainbowPulse) {
		// 		_duration = msg->getInt(5);

		// 		colorScheduler.blackout(_duration);				
		// 	}
		// 	else {
		// 		colorScheduler.disableBlackout();
		// 	}

		// 	break;

		// case VUMeter:

		// 	_l = msg->getInt(6);
			
		// 	if (_l > 0) {
		// 		analogWrite(_pin, normalize(_l));
		// 		colorScheduler.disableBlackout();
		// 	}

		// 	break;

		// case Twinkle:
		
		// 	colorScheduler.disableBlackout();

		// 	_intensity = (float)(msg->getInt(4));

		// 	if (_intensity > 0) {

		// 		if (_intensity > random8()) {
		// 			analogWrite(_pin, normalize(255));
		// 		}
		// 		else {
		// 			analogWrite(_pin, normalize(0));
		// 		}
		// 	}
		// 	else {
		// 		colorScheduler.blackout(127);
		// 	}

		// 	break;

		// case Strobo:

		// 	_intensity = msg->getInt(2);

		// 	colorScheduler.strobo(_intensity);

		// 	break;
		// }
	}

	void animationLoop() {
		
		colorScheduler.loop();
	}
};
