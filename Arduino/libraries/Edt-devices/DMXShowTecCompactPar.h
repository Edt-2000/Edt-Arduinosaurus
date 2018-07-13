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
		class ShowTecCompactPar : public Slave
		{
		private:
			int _address;
			int _minimumBrightness;
			int _maximumBrightness;

			enum Mode
			{
				Uninitialized = 0,
				Color = 1,
				Strobo = 2
			} _mode;

			CRGB _color;
			CRGB _fadeBackup;

			uint8_t _fade;

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
					DMXSerial.write(_address + 4, 0);
					DMXSerial.write(_address + 7, 0);
				}
				else if (mode == Mode::Strobo)
				{
					// switch to off to allow for flash control
					DMXSerial.write(_address, 255);
					DMXSerial.write(_address + 7, 0);
				}

				_mode = mode;
			}

			inline void output()
			{
				DMXSerial.write(_address + 1, _color.r);
				DMXSerial.write(_address + 2, _color.g);
				DMXSerial.write(_address + 3, _color.b);
			}

		public:
			void initialize(uint16_t address, uint8_t maximumBrightness, uint8_t minimumBrightness)
			{
				_address = address;
				_maximumBrightness = maximumBrightness;
				_minimumBrightness = minimumBrightness;

				_color = CRGB::HTMLColorCode::Black;

				output();
			}

			void loop()
			{
				if (_mode == Mode::Color)
				{
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
					}

					output();
				}
			}

			void solid(uint8_t h, uint8_t s, uint8_t v)
			{
				switchMode(Mode::Color);

				_color.setHSV(h, s, clampValue(v));

				output();
			}

			void solid(uint8_t h1, uint8_t h2, uint8_t s, uint8_t v, uint8_t percentage)
			{
				switchMode(Mode::Color);

				if (percentage > random8())
				{
					_color.setHSV(h2, s, clampValue(v));
				}
				else
				{
					_color.setHSV(h1, s, clampValue(v));
				}

				output();
			}

			void intensity(uint8_t intensity)
			{
				switchMode(Mode::Color);

				if (intensity == 0)
				{
					_color = CRGB::HTMLColorCode::Black;
				}
				else
				{
					_color.setHSV(85 - (intensity / 2.5), 255, clampValue(intensity));
				}

				output();
			}

			void fade(uint8_t fade, FadeMode mode = FadeMode::FadeToBlack)
			{
				_fade = fade;
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

					_color = CRGB::HTMLColorCode::Black;
				}
				else
				{
					switchMode(Mode::Strobo);

					// strobo range is 10 - 255
					uint8_t stroboSpeed = ((255 - 10) * ((double)intensity) / 255.0) + 10;

					// strobo is always FULL POWAH
					_color.setHSV(h, 255, 255);

					DMXSerial.write(_address + 4, stroboSpeed);
				}

				output();
			}
		};
	} // namespace Device
} // namespace OSC