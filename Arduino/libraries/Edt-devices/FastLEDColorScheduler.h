#pragma once

#include "FadeMode.h"
#include "Animations.h"
#include "LedState.h"

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

			Animations _animations;
		public:
			FastLEDColorScheduler();
			FastLEDColorScheduler(CRGB *leds, uint8_t nrOfLeds);

			void solid(uint8_t start, uint8_t end, uint8_t h, uint8_t s, uint8_t v);
			void solid(uint8_t start, uint8_t end, CHSV color);
			void fade(uint8_t start, uint8_t end, uint8_t duration, FadeMode mode = FadeMode::FadeToBlack);
			void disableFade(uint8_t start, uint8_t end);
			void rainbow(uint8_t start, uint8_t end, uint8_t h, uint8_t dh);
			void rainbow(uint8_t start, uint8_t center, uint8_t end, uint8_t h, uint8_t dh, uint8_t intensity);
			void twinkle(uint8_t start, uint8_t end, uint8_t h, uint8_t s, uint8_t v, uint8_t intensity, bool blackOut = true);
			void strobo(uint8_t h, uint8_t intensity);
			void chase(uint8_t h, uint8_t speed, uint8_t style);
			void bash(uint8_t h, uint8_t intensity);

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