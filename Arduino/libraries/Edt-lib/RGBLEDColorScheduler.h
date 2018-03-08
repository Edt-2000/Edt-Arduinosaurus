#include <Arduino.h>
#include "FastLED.h"
#include "FadeMode.h"

#define COLOR_INVERSE 255
#define COLOR_CORRECTION 98

#define INTENSITY_BOOST 0.3

#define RAINBOW_POSITIONS 6

class RGBLEDColorScheduler
{
  private:
	int _pins[3];

	CRGB _color;

	uint8_t _fade;
	uint8_t _rainbowPos;

	FadeMode _fadeMode;
	CRGB _fadeBackup;
	bool _reversed;

	struct Strobo
	{
		bool active;
		int loop;
		float fpl;
	};

	Strobo _strobo;

  public:
	RGBLEDColorScheduler()
	{
	}

	RGBLEDColorScheduler(int redPin, int greenPin, int bluePin, bool reversed, int rainbowPos)
	{
		pinMode(redPin, OUTPUT);
		pinMode(greenPin, OUTPUT);
		pinMode(bluePin, OUTPUT);

		_pins[0] = redPin;
		_pins[1] = greenPin;
		_pins[2] = bluePin;

		digitalWrite(redPin, 0);
		digitalWrite(greenPin, 0);
		digitalWrite(bluePin, 0);

		_reversed = reversed;
		_rainbowPos = rainbowPos * (255 / RAINBOW_POSITIONS);

		_color.setColorCode(CRGB::HTMLColorCode::Black);

		_fade = 255;

		_strobo.active = false;
	}

	void fade(int speed, FadeMode fadeMode = FadeMode::FadeToBlack)
	{
		_fade = speed;
		_fadeMode = fadeMode;

		if (_fadeMode == FadeMode::FadeOneByOne)
		{
			_fadeBackup = _color;
		}
	}

	void disableFade()
	{
		_fade = 255;
	}
	
	void solid(int hue, int saturation, int value)
	{
		fill_solid(&_color, 1, CHSV((COLOR_INVERSE - (hue - COLOR_CORRECTION)), saturation, value + ((255 - value) * INTENSITY_BOOST)));
	}

	void solid(int hue1, int hue2, int saturation, int value)
	{
		fill_solid(&_color, 1, CHSV(((COLOR_INVERSE - (hue1 - COLOR_CORRECTION)) / 2) + ((COLOR_INVERSE - (hue2 - COLOR_CORRECTION)) / 2), saturation, value + ((255 - value) * INTENSITY_BOOST)));

		// _color.hue = ((COLOR_INVERSE - (hue1 - COLOR_CORRECTION)) / 2) + ((COLOR_INVERSE - (hue2 - COLOR_CORRECTION)) / 2);
		// _color.saturation = saturation;
		// _color.value = value + ((255 - value) * INTENSITY_BOOST);
	}

	void rainbow(int hue, int deltaHue)
	{
		fill_solid(&_color, 1, CHSV((COLOR_INVERSE - (hue - COLOR_CORRECTION)) + ((deltaHue / 127.0) * _rainbowPos), 255, 255));

		// _color.hue = (COLOR_INVERSE - (hue - COLOR_CORRECTION)) + ((deltaHue / 127.0) * _rainbowPos);
		// _color.saturation = 255;
		// _color.value = 240;
	}

	void intensity(int intensity) {
		if(intensity == 0) {
			_color.setColorCode(CRGB::HTMLColorCode::Black);
		}
		else {
			// _color.hue = (COLOR_INVERSE - (0 - COLOR_CORRECTION)) - (intensity / 2.5);
			// _color.saturation = 255;
			// _color.value = intensity - 1;

			fill_solid(&_color, 1, CHSV( (COLOR_INVERSE - (0 - COLOR_CORRECTION)) - (intensity / 2.5), 255, intensity + ((255 - intensity) * INTENSITY_BOOST)));
		}
	}

	void twinkle(int hue, int saturation, int value, int intensity, bool blackOut = true)
	{
		fill_solid(&_color, 1, CHSV((COLOR_INVERSE - (hue - COLOR_CORRECTION)), saturation, intensity > random8() ? (value + ((255 - value) * INTENSITY_BOOST)) : 0));

		// _color.hue = (COLOR_INVERSE - (hue - COLOR_CORRECTION));
		// _color.saturation = saturation;
		// _color.value = intensity > random8() ? (value + ((255 - value) * INTENSITY_BOOST)) : 0;
	}

	void strobo(int fps)
	{
		_color.setColorCode(CRGB::HTMLColorCode::Black);

		_strobo.active = fps > 0;
		_strobo.loop = 0;
		_strobo.fpl = (255.0 / fps);
	}

	void loop()
	{
		if (_strobo.active)
		{
			_color.setColorCode(CRGB::HTMLColorCode::Black);

			if ((_strobo.loop++) > _strobo.fpl)
			{
				_strobo.loop = 0;

				_color.setColorCode(CRGB::HTMLColorCode::White);
			}
		}
		else
		{
			switch (_fadeMode)
			{
			case FadeMode::FadeToBlack:
				if (_fade < 255)
				{
					if (_fade > 255 - 62)
					{
						_fade = 255;
					}
					else
					{
						_fade += ((_fade) / 4) + 1;
					}

					fadeToBlackBy(&_color, 1, _fade);

					// if (_color.value > _fade)
					// {
					// 	_color.value -= _fade;
					// }
					// else
					// {
					// 	_color.value = 0;
					// }
				}
				break;
			case FadeMode::FadeOneByOne:
				if (_fade < 255)
				{
					if (_fade > random8())
					{
						_fadeBackup = _color;
						_color.setColorCode(CRGB::HTMLColorCode::Black);
					}
					else
					{
						if (_fade > 255 - 17)
						{
							_fade = 255;

							_color.setColorCode(CRGB::HTMLColorCode::Black);
						}
						else
						{
							_fade += ((_fade) / 16) + 1;

							_color = _fadeBackup;
							fadeToBlackBy(&_color, 1, _fade);
							
							//_color.value = _fadeBackup - _fade;
						}
					}
				}

				break;
			}
		}

		analogWrite(_pins[0], _color.red);
		analogWrite(_pins[1], _color.green);
		analogWrite(_pins[2], _color.blue);
	}
};
