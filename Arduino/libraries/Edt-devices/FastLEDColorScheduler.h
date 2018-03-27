#pragma once

#include "FadeMode.h"

#ifdef _MSC_VER
#include "../Edt-devicesTest/stdafx.h"

int floorf(double d) { return (int)d; }
int ceilf(double d) { return(int)d; }
#else
#include "FastLED.h"
#endif

namespace OSC {
	namespace Device {
		class FastLEDColorScheduler
		{
		private:
			struct LedState
			{
			public:
				uint8_t fade;
			};

			CRGB *_leds;
			LedState *_ledState;
			FadeMode _fadeMode;

			uint8_t _start;
			uint8_t _center;
			uint8_t _end;
			uint8_t _length;
			uint8_t _nrOfLeds;

			uint8_t normalizeLedNrDown(uint8_t percentage)
			{
				return floorf((percentage / 127.0) * _nrOfLeds);
			}
			uint8_t normalizeLedNrUp(uint8_t percentage)
			{
				return ceilf((percentage / 127.0) * _nrOfLeds);
			}

			struct Strobo
			{
				bool active;
				uint8_t loop;
				float fpl;
				CRGB color;
			};
			Strobo _strobo;

		public:
			FastLEDColorScheduler();
			FastLEDColorScheduler(CRGB *leds, uint8_t nrOfLeds);

			void solid(uint8_t start, uint8_t end, uint8_t h, uint8_t s, uint8_t v);
			void fade(uint8_t start, uint8_t end, uint8_t duration, FadeMode mode = FadeMode::FadeToBlack);
			void disableFade(uint8_t start, uint8_t end);
			void rainbow(uint8_t start, uint8_t end, uint8_t h, uint8_t dh);
			void rainbow(uint8_t start, uint8_t center, uint8_t end, uint8_t h, uint8_t dh, uint8_t intensity);
			void twinkle(uint8_t start, uint8_t end, uint8_t h, uint8_t s, uint8_t v, uint8_t intensity, bool blackOut = true);
			void strobo(uint8_t h, uint8_t intensity);
			void kitt(uint8_t position, uint8_t length, uint8_t h);

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