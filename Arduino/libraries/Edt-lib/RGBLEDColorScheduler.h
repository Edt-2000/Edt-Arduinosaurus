#include <Arduino.h>
#include "FastLED.h"
#include "FadeMode.h"

#define COLOR_INVERSE 255
#define COLOR_CORRECTION 97

#define INTENSITY_BOOST 0.3

#define RAINBOW_POSITIONS 6

class RGBLEDColorScheduler
{
  private:
	int _pins[3];

	CHSV _color;
	CRGB _rgb;

	uint8_t _fade;
	uint8_t _rainbowPos;

	FadeMode _fadeMode;
	uint8_t _fadeBackup;
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

		_color.hue = 0;
		_color.saturation = 0;
		_color.value = 0;

		_fade = 255;

		_strobo.active = false;
	}

	void fade(int speed, FadeMode fadeMode = FadeMode::FadeToBlack)
	{
		_fade = speed;
		_fadeMode = fadeMode;

		if (_fadeMode == FadeMode::FadeOneByOne)
		{
			_fadeBackup = _color.value;
		}
	}

	void disableFade()
	{
		_fade = 255;
	}

	// void output(int redOutput, int greenOutput, int blueOutput)
	// {
	// 	_outputs[0] = redOutput;
	// 	_outputs[1] = greenOutput;
	// 	_outputs[2] = blueOutput;

	// 	if (_reversed)
	// 	{
	// 		analogWrite(_pins[0], 255 - _outputs[0]);
	// 		analogWrite(_pins[1], 255 - _outputs[1]);
	// 		analogWrite(_pins[2], 255 - _outputs[2]);
	// 	}
	// 	else
	// 	{
	// 		analogWrite(_pins[0], _outputs[0]);
	// 		analogWrite(_pins[1], _outputs[1]);
	// 		analogWrite(_pins[2], _outputs[2]);
	// 	}
	// }

	void solid(int hue, int saturation, int value)
	{
		_color.hue = (COLOR_INVERSE - (hue - COLOR_CORRECTION));
		_color.saturation = saturation;
		_color.value = value + ((255 - value) * INTENSITY_BOOST);
	}

	void solid(int hue1, int hue2, int saturation, int value)
	{
		_color.hue = ((COLOR_INVERSE - (hue1 - COLOR_CORRECTION)) / 2) + ((COLOR_INVERSE - (hue2 - COLOR_CORRECTION)) / 2);
		_color.saturation = saturation;
		_color.value = value + ((255 - value) * INTENSITY_BOOST);
	}

	void rainbow(int hue, int deltaHue)
	{
		_color.hue = (COLOR_INVERSE - (hue - COLOR_CORRECTION)) + ((deltaHue / 127.0) * _rainbowPos);
		_color.saturation = 255;
		_color.value = 240;
	}

	void intensity(int intensity) {
		if(intensity == 0) {
			_color.value = 0;
		}
		else {
			_color.hue = (COLOR_INVERSE - (0 - COLOR_CORRECTION)) - (intensity / 2.5);
			_color.saturation = 255;
			_color.value = intensity - 1;
		}
	}

	void twinkle(int hue, int saturation, int value, int intensity, bool blackOut = true)
	{
		_color.hue = (COLOR_INVERSE - (hue - COLOR_CORRECTION));
		_color.saturation = saturation;
		_color.value = intensity > random8() ? (value + ((255 - value) * INTENSITY_BOOST)) : 0;
	}

	void strobo(int fps)
	{
		_color.value = 0;

		_strobo.active = fps > 0;
		_strobo.loop = 0;
		_strobo.fpl = (255.0 / fps);
	}

	void loop()
	{
		if (_strobo.active)
		{
			_color.value = 0;

			if ((_strobo.loop++) > _strobo.fpl)
			{
				_strobo.loop = 0;

				_color.value = 255;
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

					if (_color.value > _fade)
					{
						_color.value -= _fade;
					}
					else
					{
						_color.value = 0;
					}
				}
				break;
			case FadeMode::FadeOneByOne:
				if (_fade < 255)
				{
					if (_fade > random8())
					{
						_fadeBackup = _color.value;
						_color.value = 0;
					}
					else
					{
						if (_fade > 255 - 17)
						{
							_fade = 255;
							_color.value = 0;
						}
						else
						{
							_fade += ((_fade) / 16) + 1;
							_color.value = _fadeBackup - _fade;
						}
					}
				}

				break;
			}
		}

		if (_color.value == 0)
		{
			analogWrite(_pins[0], 0);
			analogWrite(_pins[1], 0);
			analogWrite(_pins[2], 0);
		}
		else if (_color.value == 255)
		{
			analogWrite(_pins[0], 255);
			analogWrite(_pins[1], 255);
			analogWrite(_pins[2], 255);
		}
		else
		{
			hsv2rgb_rainbow(_color, _rgb);

			analogWrite(_pins[0], _rgb.red);
			analogWrite(_pins[1], _rgb.green);
			analogWrite(_pins[2], _rgb.blue);
		}
	}
};
