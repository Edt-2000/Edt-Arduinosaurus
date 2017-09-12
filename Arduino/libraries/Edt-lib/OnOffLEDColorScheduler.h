#include <Arduino.h>

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
		analogWrite(_pin, 0);
		
		_strobo.active = fps > 0;
		_strobo.loop = 0;
		_strobo.fpl = (255.0 / fps);
	}

	void loop() {
		if (_strobo.active) {

			analogWrite(_pin, 0);

			if((_strobo.loop++) > _strobo.fpl)
			{
				_strobo.loop = 0;

				analogWrite(_pin, 255);
			}
		}
		else {
			if (_blackoutSpeed < 255) {
				long add = ((255 - _blackoutSpeed) / 48) + 1;
				if((long)_blackoutSpeed + add > 255) {
					_blackoutSpeed = 255;
				}
				else {
					_blackoutSpeed += add;
				}

				analogWrite(_pin, 255 - _blackoutSpeed);
			}
		}
	}
private:
	int _pin;
	// TODO: rename blackout speed
	uint8_t _blackoutSpeed;

	struct Strobo {
		bool active;
		int loop;
		float fpl;
	};

	Strobo _strobo;
};
