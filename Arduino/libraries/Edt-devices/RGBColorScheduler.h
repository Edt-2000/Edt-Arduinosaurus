#pragma once

#include "FadeMode.h"
#include "Animations.h" 
#include "LedState.h"

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
			LedState _ledState;
			FadeMode _fadeMode;

			CRGB *_fadeBackup;

			Tlc5940 * _tlc;
			struct tlcAddress {
				unsigned address : 4;
			};

			Animations _animations;
			
		public:
			uint8_t nrOfLeds;

			RGBColorScheduler();
			RGBColorScheduler(CRGB * leds, uint8_t nrOfLeds, Tlc5940 * tlc);

			void solid(uint8_t h, uint8_t s, uint8_t v);
			void solid(CHSV color);
			void solid(uint8_t h1, uint8_t h2, uint8_t s, uint8_t v, uint8_t percentage);
			void fade(uint8_t duration, FadeMode mode = FadeMode::FadeToBlack);
			void disableFade();
			void rainbow(uint8_t h, uint8_t dh);
			void intensity(uint8_t intensity);
			void strobo(uint8_t h, uint8_t intensity);

			void loop();
			void output();

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