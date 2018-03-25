#pragma once

#include "FadeMode.h"

#ifdef _MSC_VER
#include "../Edt-devicesTest/stdafx.h"
#else
#include "FastLED.h"
#include "SparkFun_Tlc5940.h"
#endif

namespace OSC {
	namespace Device {
		class RGBColorScheduler
		{
		private:
			struct Color
			{
				uint8_t h;
				uint8_t s;
				uint8_t l;

				CHSV chsv()
				{
					return CHSV(h, s, l);
				}
			};

			uint8_t _channels[3];

			CRGB _color;

			uint8_t _fade;
			uint8_t _rainbowPos;

			FadeMode _fadeMode;
			CRGB _fadeBackup;

			Tlc5940 * _tlc;

			struct Strobo
			{
				bool active;
				int loop;
				float fpl;
				Color color;
			};
			Strobo _strobo;

		public:
			RGBColorScheduler();
			RGBColorScheduler(uint8_t redChannel, uint8_t greenChannel, uint8_t blueChannel, uint8_t rainbowPos, Tlc5940 * tlc);

			void solid(uint8_t h, uint8_t s, uint8_t v);
			void solid(uint8_t h1, uint8_t h2, uint8_t s, uint8_t v, uint8_t percentage);
			void fade(uint8_t duration, FadeMode mode = FadeMode::FadeToBlack);
			void disableFade();
			void rainbow(uint8_t h, uint8_t dh);
			void intensity(uint8_t intensity);
			void strobo(uint8_t h, uint8_t intensity);

			void loop();

#ifdef _MSC_VER
			Command getCommandColor();
			Command getCommandTwinkle();
			Command getCommandFade();
			Command commandColor;
			Command commandTwinkle;
			Command commandFade;
#endif
		};
	}
}