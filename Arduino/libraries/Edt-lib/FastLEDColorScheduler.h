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

	int _start;
	int _center;
	int _end;
	int _length;
	int _nrOfLeds;

	inline int normalizeLedNrDown(int percentage) {
		return floorf((percentage / 127.0) * _nrOfLeds);
	}
	inline int normalizeLedNrUp(int percentage) {
		return ceilf((percentage / 127.0) * _nrOfLeds);
	}

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

	inline void fade(int start, int end, int speed) {
		_start = normalizeLedNrDown(start);
		_end = normalizeLedNrUp(end);

		for (int i = _start; i < _end; i++) {
			_ledState[i].fade = speed;
		}
	}

	inline void disableFade(int start, int end) {
		_start = normalizeLedNrDown(start);
		_end = normalizeLedNrUp(end);

		for (int i = _start; i < _end; i++) {
			_ledState[i].fade = 255;
		}
	}

	inline void solid(int start, int end, int hue, int saturation, int value) {
		_start = normalizeLedNrDown(start);
		_end = normalizeLedNrUp(end);
		_length = _end - _start;

		fill_solid(_leds + _start, _length, CHSV(hue, saturation, value));
	}

	inline void rainbow(int start, int end, int hue, int deltaHue) {
		_start = normalizeLedNrDown(start);
		_end = normalizeLedNrUp(end);
		_length = _end - _start;
		
		fill_rainbow(_leds + _start, _length, hue, deltaHue);
	}

	inline void rainbow(int start, int center, int end, int hue, int deltaHue, int intensity) {
		_start = normalizeLedNrDown(start);
		_center = normalizeLedNrDown(center);
		_end = normalizeLedNrUp(end);

		if (_start != _center) {
			
			int leds = (_center - _start) * (intensity / 255.0);

			fade(_start, _center - leds, 127);
			disableFade(_center - leds, _center);

			fill_rainbow_reverse(&_leds[_center - leds - 1], leds, hue, deltaHue / (_center - _start));
		}
		if (_center != _end) {

			int leds = (_end - _center) * (intensity / 255.0);

			fade(_end - (_end - _center - leds), _end, 127);
			disableFade(_center, _center + leds);

			fill_rainbow(&_leds[_center], leds, hue, deltaHue / (_end - _center));
		}
	}

	inline void twinkle(int start, int end, int hue, int saturation, int value, int intensity) {
		_start = normalizeLedNrDown(start);
		_end = normalizeLedNrDown(end);

		for (int i = _start; i < _end; i++) {
			if (intensity > random8()) {
				_leds[i] = CHSV(hue, saturation, value);
			}
			else {
				_leds[i] = CHSV(0,0,0);
			}
		}
	}

	inline void strobo(int hue, int fps) {
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

	inline void loop() {
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
