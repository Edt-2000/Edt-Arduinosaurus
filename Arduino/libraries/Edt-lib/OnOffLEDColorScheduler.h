#include <Arduino.h>

// TODO: use this macro to flip the output when needed
#define normalize(x) 255 - x

class OnOffLEDColorScheduler
{
public:
	OnOffLEDColorScheduler() {
	}

	OnOffLEDColorScheduler(int pin) {
		_pin = pin;
		_blackoutSpeed = 255;
		
		_strobo.active = false;
	}

	void blackout(int speed) {
		_blackoutSpeed = speed;
	}
	void disableBlackout() {
		_blackoutSpeed = 255;
	}

	void strobo(int fps) {
		analogWrite(_pin, normalize(255));
		
		_strobo.active = fps > 0;
		_strobo.loop = 0;
		_strobo.fpl = (255.0 / fps);
	}

	void loop() {
		if (_strobo.active) {

			analogWrite(_pin, normalize(255));

			if((_strobo.loop++) > _strobo.fpl)
			{
				_strobo.loop = 0;

				analogWrite(_pin, normalize(0));
			}
		}
		else {
			if (_blackoutSpeed < 255) {
				long add = (2 * _blackoutSpeed);

				if(add == 0) {
					add++;
				}

				if((long)_blackoutSpeed + add > 255) {
					_blackoutSpeed = 255;
				}
				else {
					_blackoutSpeed += add;
				}

				analogWrite(_pin, normalize(_blackoutSpeed));
			}
		}
	}
private:
	int _pin;
	// TODO: rename blackout speed
	// TODO: remember output to blend fadeouts better
	uint8_t _blackoutSpeed;

	struct Strobo {
		bool active;
		int loop;
		float fpl;
	};

	Strobo _strobo;
};
