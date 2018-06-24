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
		class FixedSingleChannel : public Slave
		{
		private:
			inline void output()
			{
				DMXSerial.write(_address, _minimumBrightness);
			}

		public:
			void initialize(int address, int maximumBrightness, int minimumBrightness)
			{
				_address = address;
				_maximumBrightness = maximumBrightness;
				_minimumBrightness = minimumBrightness;

				output();
			}

			void loop()
			{
				output();
			}

			void solid(uint8_t h, uint8_t s, uint8_t v)
			{
				// do nothing
			}

			void solid(uint8_t h1, uint8_t h2, uint8_t s, uint8_t v, uint8_t percentage)
			{
				// do nothing
			}

			void intensity(uint8_t intensity)
			{
				// do nothing
			}

			void fade(uint8_t fade, FadeMode mode = FadeMode::FadeToBlack)
			{
				// do nothing
			}

			void disableFade()
			{
				// do nothing
			}

			void strobo(uint8_t h, uint8_t intensity)
			{
				// do nothing
			}
		};
	} // namespace Device
} // namespace OSC