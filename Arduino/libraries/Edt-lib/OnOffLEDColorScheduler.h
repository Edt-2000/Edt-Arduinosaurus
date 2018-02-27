#include <Arduino.h>

class OnOffLEDColorScheduler
{
private:
	int _pin;
	uint8_t _output;
	uint8_t _fade;
	bool _reversed;

	struct Strobo {
		bool active;
		int loop;
		float fpl;
	};

	Strobo _strobo;
public:
	
	OnOffLEDColorScheduler() {
	}

	OnOffLEDColorScheduler(int pin, bool reversed) {
		_pin = pin;
		_fade = 255;
		_reversed = reversed;

// TODO: own the pins

		_strobo.active = false;
	}

	inline void fade(int speed) {
		_fade = speed;
	}

	inline void disableFade() {
		_fade = 255;
	}
	
	inline void output(int output) {
		_output = output;

		if(_reversed) {
			analogWrite(_pin, 255 - output);
		}
		else {
			analogWrite(_pin, output);
		}
	}

	inline void strobo(int fps) {
		output(0);
		
		_strobo.active = fps > 0;
		_strobo.loop = 0;
		_strobo.fpl = (255.0 / fps);
	}

	inline void loop() {
		if (_strobo.active) {

			output(0);

			if((_strobo.loop++) > _strobo.fpl)
			{
				_strobo.loop = 0;

				output(255);
			}
		}
		else {
			if (_fade < 255) {
				// prevent overflowing of uint8_t's

				// TODO: sync with fastled
				int add = (2 * _fade);
				if(add == 0) {
					add++;
				}

				if(255 - add < _fade) {
					_fade = 255;
					output(0);
				}
				else {
					_fade += add;

					if(_output < _fade) {
						output(0);
					}
					else {
						output(_output - _fade);
					}
				}
			}
		}
	}
};
