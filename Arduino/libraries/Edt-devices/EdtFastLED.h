#pragma once

#include "FastLEDColorScheduler.h"

#ifdef _MSC_VER
#include "FadeMode.h"
#include "../../../Arduino/libraries/Edt-lib/OSCMessageDefinitions.h"
#include "../../../../OSC-light/OSC-light/OSCArduino.h"
#include "../../../../OSC-light/OSC-light/OSCMessageConsumer.h"
#include "../../../../OSC-light/OSC-light/OSCStructMessage.h"
#else
#include <OSCArduino.h>
#include <OSCMessageConsumer.h>
#include <OSCStructMessage.h>
#include <OSCMessageDefinitions.h>
#include <FastLED.h>
#include <FadeMode.h>
#endif

namespace OSC {
	namespace Device {
		class EdtFastLED : public OSC::MessageConsumer<OSC::StructMessage<OSC::EdtMessage, uint8_t>>
		{
		private:
			const char *_pattern;
			CRGB *_leds;
			int _nrOfLeds;

#ifndef _MSC_VER
			FastLEDColorScheduler _colorScheduler;
#endif
		public:
#ifdef _MSC_VER
			FastLEDColorScheduler _colorScheduler;
#endif

			EdtFastLED(const char *pattern, int const nrOfLeds) : MessageConsumer()
			{
				_pattern = pattern;

				_nrOfLeds = nrOfLeds;
				_leds = new CRGB[_nrOfLeds];

				_colorScheduler = FastLEDColorScheduler(_leds, _nrOfLeds);
			}

			const char *pattern()
			{
				return _pattern;
			}

			template <uint8_t dataPin, uint8_t clockPin>
			void configurePins()
			{
				FastLED.addLeds<APA102, dataPin, clockPin, BGR>(_leds, _nrOfLeds);

				_colorScheduler.solid(0, 127, 32, 240, 32);
			}

			void test()
			{
				_colorScheduler.solid(0, 127, 120, 240, 255);
				_colorScheduler.fade(0, 127, 2);
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
						_colorScheduler.solid(singleColor.start, singleColor.end, singleColor.hue, singleColor.saturation, singleColor.value);
					}

					if (command == OSC::ColorCommands::SinglePulse || singleColor.value == 0)
					{
						_colorScheduler.fade(singleColor.start, singleColor.end, singleColor.duration, FadeMode::FadeToBlack);
					}
					else if (command == OSC::ColorCommands::SingleSpark)
					{
						_colorScheduler.fade(singleColor.start, singleColor.end, singleColor.duration, FadeMode::FadeOneByOne);
					}
					else
					{
						_colorScheduler.disableFade(singleColor.start, singleColor.end);
					}

					break;

				case OSC::ColorCommands::DualPulse:
				case OSC::ColorCommands::DualSolid:
				case OSC::ColorCommands::DualSparkle:

					_colorScheduler.solid(dualColor.start, dualColor.end, dualColor.hue1, 255, 255);

					if (dualColor.percentage > 0)
					{
						_colorScheduler.twinkle(dualColor.start, dualColor.end, dualColor.hue2, 255, 255, dualColor.percentage, false);
					}

					if (command == OSC::ColorCommands::DualPulse)
					{
						_colorScheduler.fade(dualColor.start, dualColor.end, dualColor.duration, FadeMode::FadeToBlack);
					}
					else if (command == OSC::ColorCommands::DualSparkle)
					{
						_colorScheduler.fade(dualColor.start, dualColor.end, dualColor.duration, FadeMode::FadeOneByOne);
					}
					else
					{
						_colorScheduler.disableFade(dualColor.start, dualColor.end);
					}

					break;

				case OSC::ColorCommands::RainbowPulse:
				case OSC::ColorCommands::RainbowSolid:
				case OSC::ColorCommands::RainbowSpark:

					if (rainbow.deltaHue > 0)
					{
						_colorScheduler.rainbow(rainbow.start, rainbow.end, rainbow.hue, rainbow.deltaHue);
					}

					if (command == OSC::ColorCommands::RainbowPulse || rainbow.deltaHue == 0)
					{
						_colorScheduler.fade(rainbow.start, rainbow.end, rainbow.duration, FadeMode::FadeToBlack);
					}
					else if (command == OSC::ColorCommands::RainbowSpark)
					{
						_colorScheduler.fade(rainbow.start, rainbow.end, rainbow.duration, FadeMode::FadeOneByOne);
					}
					else
					{
						_colorScheduler.disableFade(rainbow.start, rainbow.end);
					}

					break;

				case OSC::ColorCommands::VuMeter:

					_colorScheduler.rainbow(vuMeter.start, vuMeter.center, vuMeter.end, vuMeter.hue, vuMeter.deltaHue, vuMeter.intensity);

					break;

				case OSC::ColorCommands::Twinkle:

					_colorScheduler.disableFade(twinkle.start, twinkle.end);

					if (twinkle.intensity > 0)
					{
						_colorScheduler.twinkle(twinkle.start, twinkle.end, twinkle.hue, 255, 255, twinkle.intensity);
					}
					else
					{
						_colorScheduler.fade(twinkle.start, twinkle.end, 127);
					}

					break;

				case OSC::ColorCommands::Strobo:

					_colorScheduler.strobo(strobo.hue, strobo.intensity);

					break;

				case OSC::ColorCommands::Kitt:

					if (kitt.position > 0) {
						_colorScheduler.kitt(kitt.position, kitt.length, kitt.hue);
					}
					else {
						_colorScheduler.fade(0, 127, 127);
					}

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