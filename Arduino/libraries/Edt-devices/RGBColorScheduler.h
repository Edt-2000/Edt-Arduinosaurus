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
			CRGB *_leds;
			uint8_t _nrOfLeds;

			uint8_t _fade;

			FadeMode _fadeMode;
			CRGB *_fadeBackup;

			Tlc5940 * _tlc;

			struct Strobo
			{
				bool active;
				int loop;
				float fpl;
				CRGB color;
			};
			Strobo _strobo;

		public:
			RGBColorScheduler();
			RGBColorScheduler(CRGB * leds, uint8_t nrOfLeds, Tlc5940 * tlc);

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