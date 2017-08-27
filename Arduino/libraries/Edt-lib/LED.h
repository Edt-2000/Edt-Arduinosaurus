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

			colorScheduler.blackout(_duration);

			_l = msg->getInt(1);

			if (_l > 0) {

				analogWrite(_pin, _l);
			}

			if (_mode == SinglePulse || _l == 0) {
				_duration = msg->getInt(2);

				colorScheduler.blackout(_duration);
			}
			else {
				colorScheduler.disableBlackout();
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
		Strobo = 200
	};

	const char * _pattern;

	int _mode;
	int _duration;
	int _intensity;

	int _pin;
	int _l;
};
