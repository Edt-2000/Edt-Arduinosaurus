#pragma once

#ifdef _MSC_VER
#include "RGBColorScheduler.h"
#include "FadeMode.h"
#include "../../../Arduino/libraries/Edt-lib/OSCMessageDefinitions.h"
#include "../../../../OSC-light/OSC-light/OSCArduino.h"
#else
#include <OSCArduino.h>
#include <OSCMessageConsumer.h>
#include <OSCMessageDefinitions.h>
#include <FastLED.h>
#include <RGBColorScheduler.h>
#include <FadeMode.h>
#endif

namespace OSC {
	namespace Device {
		class EdtRGB : public OSC::StructMessageConsumer<OSC::ColorCommands, uint8_t>
		{
		private:
			const char *_pattern;

			OSC::SingleColorCommand singleColor;
			OSC::DualColorCommand dualColor;
			OSC::RainbowCommand rainbow;
			OSC::VuMeterCommand vuMeter;
			OSC::TwinkleCommand twinkle;
			OSC::StroboCommand strobo;
			OSC::KittCommand kitt;
			
#ifndef _MSC_VER
			RGBColorScheduler _colorScheduler;
#endif
		public:
#ifdef _MSC_VER
			RGBColorScheduler _colorScheduler;
#endif
			EdtRGB(const char *pattern, uint8_t const redChannel, uint8_t const greenChannel, uint8_t const blueChannel, Tlc5940 * tlc) : StructMessageConsumer(13)
			{
				_pattern = pattern;

				_colorScheduler = RGBColorScheduler(redChannel, greenChannel, blueChannel, 1, tlc);

				addEnumToStructMapping<OSC::SingleColorCommand>(OSC::ColorCommands::SinglePulse, &singleColor);
				addEnumToStructMapping<OSC::SingleColorCommand>(OSC::ColorCommands::SingleSolid, &singleColor);
				addEnumToStructMapping<OSC::SingleColorCommand>(OSC::ColorCommands::SingleSpark, &singleColor);
				addEnumToStructMapping<OSC::RainbowCommand>(OSC::ColorCommands::RainbowPulse, &rainbow);
				addEnumToStructMapping<OSC::RainbowCommand>(OSC::ColorCommands::RainbowSolid, &rainbow);
				addEnumToStructMapping<OSC::RainbowCommand>(OSC::ColorCommands::RainbowSpark, &rainbow);
				addEnumToStructMapping<OSC::VuMeterCommand>(OSC::ColorCommands::VuMeter, &vuMeter);
				addEnumToStructMapping<OSC::TwinkleCommand>(OSC::ColorCommands::Twinkle, &twinkle);
				addEnumToStructMapping<OSC::StroboCommand>(OSC::ColorCommands::Strobo, &strobo);
				addEnumToStructMapping<OSC::DualColorCommand>(OSC::ColorCommands::DualPulse, &dualColor);
				addEnumToStructMapping<OSC::DualColorCommand>(OSC::ColorCommands::DualSolid, &dualColor);
				addEnumToStructMapping<OSC::DualColorCommand>(OSC::ColorCommands::DualSparkle, &dualColor);
				addEnumToStructMapping<OSC::KittCommand>(OSC::ColorCommands::Kitt, &kitt);
			}

			const char *pattern()
			{
				return _pattern;
			}

			void test()
			{
				_colorScheduler.solid(120, 255, 254);
				_colorScheduler.fade(2);
			}

			void callbackEnum(OSC::ColorCommands command)
			{
				switch (command)
				{

				case OSC::ColorCommands::SinglePulse:
				case OSC::ColorCommands::SingleSolid:
				case OSC::ColorCommands::SingleSpark:

					if (singleColor.value > 0)
					{
						_colorScheduler.solid(singleColor.hue, singleColor.saturation, singleColor.value);
					}

					if (command == OSC::ColorCommands::SinglePulse || singleColor.value == 0)
					{
						_colorScheduler.fade(singleColor.duration, FadeMode::FadeToBlack);
					}
					else if (command == OSC::ColorCommands::SingleSpark)
					{
						_colorScheduler.fade(singleColor.duration, FadeMode::FadeOneByOne);
					}
					else
					{
						_colorScheduler.disableFade();
					}

					break;

				case OSC::ColorCommands::DualPulse:
				case OSC::ColorCommands::DualSolid:
				case OSC::ColorCommands::DualSparkle:

					_colorScheduler.solid(dualColor.hue1, dualColor.hue2, 255, 254, dualColor.percentage);
					
					if (command == OSC::ColorCommands::DualPulse)
					{
						_colorScheduler.fade(dualColor.duration, FadeMode::FadeToBlack);
					}
					else if (command == OSC::ColorCommands::DualSparkle)
					{
						_colorScheduler.fade(dualColor.duration, FadeMode::FadeOneByOne);
					}
					else
					{
						_colorScheduler.disableFade();
					}

					break;

				case OSC::ColorCommands::RainbowPulse:
				case OSC::ColorCommands::RainbowSolid:
				case OSC::ColorCommands::RainbowSpark:

					if (rainbow.deltaHue > 0)
					{
						_colorScheduler.rainbow(rainbow.hue, rainbow.deltaHue);
					}

					if (command == OSC::ColorCommands::RainbowPulse || rainbow.deltaHue == 0)
					{
						_colorScheduler.fade(rainbow.duration, FadeMode::FadeToBlack);
					}
					else if (command == OSC::ColorCommands::RainbowSpark)
					{
						_colorScheduler.fade(rainbow.duration, FadeMode::FadeOneByOne);
					}
					else
					{
						_colorScheduler.disableFade();
					}

					break;

				case OSC::ColorCommands::VuMeter:

					if (vuMeter.intensity > 0)
					{
						_colorScheduler.intensity(vuMeter.intensity);
					}
					else
					{
						_colorScheduler.fade(127);
					}

					break;

				case OSC::ColorCommands::Twinkle:

					_colorScheduler.disableFade();

					if (twinkle.intensity > 0)
					{
						_colorScheduler.solid(twinkle.hue, 255, twinkle.intensity - 1);
					}
					else
					{
						_colorScheduler.fade(127);
					}

					break;

				case OSC::ColorCommands::Strobo:

					_colorScheduler.strobo(strobo.hue, strobo.intensity);

					break;

				case OSC::ColorCommands::Kitt:

					// No implementation

					break;
				}
			}

			void animationLoop()
			{
				_colorScheduler.loop();
			}
		};


	}
}