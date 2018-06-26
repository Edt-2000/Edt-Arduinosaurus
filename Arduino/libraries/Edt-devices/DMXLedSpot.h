#pragma once

#include "..\DMX\DMXSerial.h"
#include <DMXSlave.h>
#include "FadeMode.h"

#ifdef _MSC_VER
#include "../Edt-devicesTest/stdafx.h"
#else
#include "FastLED.h"
#endif

namespace OSC
{
namespace DMX
{
class LedSpot : public Slave
{
  private:
	enum Mode
	{
		Uninitialized = 0,
		Color = 1,
		Strobo = 2
	} _mode;

	CRGB _color[2];
	CRGB _fadeBackup[2];

	uint8_t _fade;
	FadeMode _fadeMode;

	inline void switchMode(Mode mode)
	{
		if (mode == _mode)
		{
			return;
		}

		if (mode == Mode::Color)
		{
			// switch to brightness control
			DMXSerial.write(_address, 255);
		}
		else if (mode == Mode::Strobo)
		{
			// switch to off to allow for flash control
			DMXSerial.write(_address, 0);
		}

		_mode = mode;
	}

	inline void output()
	{
		DMXSerial.write(_address + 1, _color[0].r);
		DMXSerial.write(_address + 2, _color[0].g);
		DMXSerial.write(_address + 3, _color[0].b);

		// use second color's red channel for white channel
		DMXSerial.write(_address + 4, _color[1].r);
	}

  public:
	void initialize(uint16_t address, uint8_t maximumBrightness, uint8_t minimumBrightness)
	{
		_address = address;
		_maximumBrightness = maximumBrightness;
		_minimumBrightness = minimumBrightness;

		DMXSerial.write(address, 0);
	}

	void loop()
	{
		if (_mode == Mode::Color)
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

					fadeToBlackBy(_color, 2, _fade);
				}
				break;

			case FadeMode::FadeOneByOne:
				if (_fade < 255)
				{
					if (_fade > random8())
					{
						_fadeBackup[0] = _color[0];
						_fadeBackup[1] = _color[1];

						fill_solid(_color, 2, CRGB::HTMLColorCode::Black);
					}
					else
					{
						if (_fade > 255 - 17)
						{
							_fade = 255;

							fill_solid(_color, 2, CRGB::HTMLColorCode::Black);
						}
						else
						{
							_fade += ((_fade) / 16) + 1;

							_color[0] = _fadeBackup[0];
							_color[1] = _fadeBackup[1];

							fadeToBlackBy(_color, 2, _fade);
						}
					}
				}
				break;
			}

			output();
		}
	}

	void solid(uint8_t h, uint8_t s, uint8_t v)
	{
		switchMode(Mode::Color);

		// use white led when h & s are zero (grey scale)
		if (h == 0 && s == 0)
		{
			_color[0] = CRGB::HTMLColorCode::Black;
			_color[1].r = clampValue(v);
		}
		else
		{
			_color[0].setHSV(h, s, clampValue(v));
			_color[1] = CRGB::HTMLColorCode::Black;
		}

		output();
	}

	void solid(uint8_t h1, uint8_t h2, uint8_t s, uint8_t v, uint8_t percentage)
	{
		switchMode(Mode::Color);

		if (percentage > random8())
		{
			_color[0].setHSV(h2, s, clampValue(v));
			_color[1] = CRGB::HTMLColorCode::Black;
		}
		else
		{
			_color[0].setHSV(h1, s, clampValue(v));
			_color[1] = CRGB::HTMLColorCode::Black;
		}

		output();
	}

	void intensity(uint8_t intensity)
	{
		switchMode(Mode::Color);

		if (intensity == 0)
		{
			_color[0] = CRGB::HTMLColorCode::Black;
		}
		else
		{
			_color[0].setHSV(85 - (intensity / 2.5), 255, clampValue(intensity));
		}

		_color[1] = CRGB::HTMLColorCode::Black;

		output();
	}

	void fade(uint8_t fade, FadeMode mode = FadeMode::FadeToBlack)
	{
		_fade = fade;
		_fadeMode = mode;
	}

	void disableFade()
	{
		_fade = 255;
	}

	void strobo(uint8_t h, uint8_t intensity)
	{
		if (intensity == 0)
		{
			switchMode(Mode::Color);

			_color[0] = CRGB::HTMLColorCode::Black;
			_color[1] = CRGB::HTMLColorCode::Black;
		}
		else
		{
			switchMode(Mode::Strobo);

			// strobo range is 135 - 239
			uint8_t stroboSpeed = ((239 - 135) * ((double)intensity) / 255.0) + 135;

			// no value clamping since strobo should be FULL POWAH
			_color[0].setHSV(h, 255, 255);
			// cheat to get brighter flashes
			_color[1].setHSV(0, 255, h);

			DMXSerial.write(_address, stroboSpeed);
		}

		output();
	}
};
} // namespace Device
} // namespace OSC