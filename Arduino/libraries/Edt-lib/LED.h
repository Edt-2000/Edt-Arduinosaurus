#pragma once

#include <OSCArduino.h>
#include <OnOffLEDColorScheduler.h>

class EdtLED : public OSC::IMessageConsumer
{
public:
	OnOffLEDColorScheduler colorScheduler;
	
	EdtLED(const char * pattern, int outputPin) {
		_pattern = pattern;

		_pin = outputPin;
		pinMode(_pin, OUTPUT);

		analogWrite(_pin, 0);
		
		colorScheduler = OnOffLEDColorScheduler(_pin);
	}

	const char * address() {
		return _pattern;
	}

	void callback(OSC::Message * msg) {
		_mode = (Mode)msg->getInt(0);

		switch (_mode) {

		case SinglePulse:
		case SingleSolid:
		case RainbowPulse:
		case RainbowSolid:
		case VUMeter:

			if(_mode == VUMeter) {
				_l = msg->getInt(6);
			}
			else {
				_l = msg->getInt(1);
			}

			if (_l > 0) {
				analogWrite(_pin, _l);
			}

			if (_mode == SinglePulse || _l == 0) {
				if(_mode == SinglePulse || _mode == SingleSolid) {
					_duration = msg->getInt(6);
				}
				else {
					_duration = msg->getInt(5);
				}

				colorScheduler.blackout(_duration);
			}
			else {
				colorScheduler.disableBlackout();
			}

			break;

		case Twinkle:
		
			colorScheduler.disableBlackout();

			_intensity = (float)(msg->getInt(4));

			if (_intensity > 0) {

				if (_intensity > random8()) {
					analogWrite(_pin, 255);
				}
				else {
					analogWrite(_pin, 0);
				}
			}
			else {
				colorScheduler.blackout(127);
			}

			break;

		case Strobo:

			// stobo only has 1 parameters
			_intensity = msg->getInt(1);

			colorScheduler.strobo(_intensity);

			break;
		}
	}

	void animationLoop() {
		
		colorScheduler.loop();
	}
private:
	enum Mode {
		SingleSolid = 1,
		SinglePulse = 2,
		RainbowSolid = 3,
		RainbowPulse = 4,
		VUMeter = 100,
		Twinkle = 101,
		Strobo = 200
	};

	const char * _pattern;

	int _mode;
	int _duration;
	int _intensity;

	int _pin;
	int _l;
};
