#pragma once

#ifdef _MSC_VER
#include "../Edt-devicesTest/stdafx.h"
#include "../../../Arduino/libraries/Edt-lib/OSCMessageDefinitions.h"
#include "../../../../OSC-light/OSC-light/OSCArduino.h"
#else
#include <OSCArduino.h>
#include <OSCMessageConsumer.h>
#include <OSCMessageDefinitions.h>
#endif

#include "DMXSlave.h"

namespace OSC {
	namespace Device {
		class EdtDMX : public OSC::MessageConsumer<OSC::StructMessage<OSC::EdtMessage, uint8_t>>
		{
		private:
			const char *_pattern;
			DMXSlave *_slave;

		public:
			// TODO: create own slaves from setup
			EdtDMX(const char *pattern, DMXSlave *slave) : MessageConsumer()
			{
				_pattern = pattern;
				_slave = slave;

				// TODO
				_slave->initialize(4);
			}

			const char *pattern()
			{
				return _pattern;
			}

			void test()
			{
				_slave->solid(120, 255, 255);
				_slave->fade(2);
			}

			void callbackMessage(OSC::StructMessage<OSC::EdtMessage, uint8_t> * message)
			{
				// todo: remove these variables
				auto command = message->messageStruct.command;
				auto dualColor = message->messageStruct.commands.dualColor;
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
						_slave->solid(singleColor.hue, singleColor.saturation, singleColor.value);
					}

					if (command == OSC::ColorCommands::SinglePulse || singleColor.value == 0)
					{
						_slave->fade(singleColor.duration, FadeMode::FadeToBlack);
					}
					else if (command == OSC::ColorCommands::SingleSpark)
					{
						_slave->fade(singleColor.duration, FadeMode::FadeOneByOne);
					}
					else
					{
						_slave->disableFade();
					}

					break;

				case OSC::ColorCommands::DualPulse:
				case OSC::ColorCommands::DualSolid:
				case OSC::ColorCommands::DualSparkle:

					_slave->solid(dualColor.hue1, dualColor.hue2, 255, 254, dualColor.percentage);

					if (command == OSC::ColorCommands::DualPulse)
					{
						_slave->fade(dualColor.duration, FadeMode::FadeToBlack);
					}
					else if (command == OSC::ColorCommands::DualSparkle)
					{
						_slave->fade(dualColor.duration, FadeMode::FadeOneByOne);
					}
					else
					{
						_slave->disableFade();
					}

					break;

				case OSC::ColorCommands::RainbowPulse:
				case OSC::ColorCommands::RainbowSolid:
				case OSC::ColorCommands::RainbowSpark:

					// No implementation

					break;

				case OSC::ColorCommands::VuMeter:

					if (vuMeter.intensity > 0)
					{
						_slave->intensity(vuMeter.intensity);
					}
					else
					{
						_slave->fade(127);
					}

					break;

				case OSC::ColorCommands::Twinkle:

					_slave->disableFade();

					if (twinkle.intensity > 0)
					{
						_slave->solid(twinkle.hue, 255, twinkle.intensity - 1);
					}
					else
					{
						_slave->fade(127);
					}

					break;

				case OSC::ColorCommands::Strobo:

					_slave->strobo(strobo.hue, strobo.intensity);

					break;

				case OSC::ColorCommands::Chase:
				case OSC::ColorCommands::Bash:

					// No implementation

					break;
				}
			}

			void animationLoop()
			{
				_slave->loop();
			}
		};


	}
}