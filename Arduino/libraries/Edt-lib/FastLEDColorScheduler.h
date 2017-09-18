#include "FastLED.h"

class FastLEDColorScheduler
{
private:
	struct Color {
		int h;
		int s;
		int l;

		CHSV chsv() {
			return CHSV(h, s, l);
		}
	};
	
	struct LedState {
	public:
		uint8_t fade;
	};

	struct Strobo {
		bool active;
		int loop;
		float fpl;
		Color color;
	};

	CRGB *_leds;
	LedState *_ledState;
	int _nrOfLeds;

	Strobo _strobo;

public:
	FastLEDColorScheduler() {

	}

	FastLEDColorScheduler(CRGB* leds, int nrOfLeds) {
		_leds = leds;
		_ledState = new LedState[nrOfLeds];
		_nrOfLeds = nrOfLeds;

		for (int i = 0; i < nrOfLeds; i++) {
			_ledState[i].fade = 255;
		}
	}

	void fade(int start, int stop, int speed) {
		for (int i = start; i < stop; i++) {
			_ledState[i].fade = speed;
		}
	}
	void disableFade(int start, int stop) {
		for (int i = start; i < stop; i++) {
			_ledState[i].fade = 255;
		}
	}

	void strobo(int hue, int fps) {
		disableFade(0, _nrOfLeds);

		fill_solid(_leds, _nrOfLeds, 0);

		_strobo.active = fps > 0;
		_strobo.loop = 0;
		_strobo.fpl = (255.0 / fps);
		if (hue == 0) {
			_strobo.color.h = 0;
			_strobo.color.s = 0;
			_strobo.color.l = 255;
		}
		else {
			_strobo.color.h = hue;
			_strobo.color.s = 255;
			_strobo.color.l = 255;
		}
	}

	void loop() {
		if (_strobo.active) {

			fill_solid(_leds, _nrOfLeds, 0);

			if((_strobo.loop++) > _strobo.fpl)
			{
				_strobo.loop = 0;
				
				fill_solid(_leds, _nrOfLeds, _strobo.color.chsv());
			}
		}
		else {
			for (int i = 0; i < _nrOfLeds; i++) {
				if (_ledState[i].fade < 255) {
					long add = ((255 - _ledState[i].fade) / 48) + 1;
					if((long)_ledState[i].fade + add > 255) {
						_ledState[i].fade = 255;
					}
					else {
						_ledState[i].fade += add;
					}
	
					fadeToBlackBy(_leds + i, 1, _ledState[i].fade);
				}
			}
		}
	}
};
