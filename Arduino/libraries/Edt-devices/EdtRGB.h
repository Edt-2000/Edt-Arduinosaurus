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
		class EdtRGB : public OSC::MessageConsumer<OSC::StructMessage<OSC::EdtMessage, uint8_t>>
		{
		private:
			const char *_pattern;
			CRGB *_leds;
			int _nrOfLeds;
			
#ifndef _MSC_VER
			RGBColorScheduler _colorScheduler;
#endif
		public:
#ifdef _MSC_VER
			RGBColorScheduler _colorScheduler;
#endif
			EdtRGB(const char *pattern, uint8_t nrOfLeds, Tlc5940 * tlc) : MessageConsumer()
			{
				_pattern = pattern;

				_nrOfLeds = nrOfLeds;
				_leds = new CRGB[_nrOfLeds];

				_colorScheduler = RGBColorScheduler(_leds, nrOfLeds, tlc);
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

			void callbackMessage(OSC::StructMessage<OSC::EdtMessage, uint8_t> * message)
			{
				// todo: remove these variables
				auto command = message->messageStruct.command;
				auto dualColor = message->messageStruct.commands.dualColor;
				auto kitt = message->messageStruct.commands.kitt;
				auto rainbow = message->messageStruct.commands.rainbow;
				auto singleColor = message->messageStruct.commands.singleColor;
				auto strobo = message->messageStruct.commands.strobo;
				auto twinkle = message->messageStruct.commands.twinkle;
				auto vuMeter = message->messageStruct.commands.vuMeter;

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