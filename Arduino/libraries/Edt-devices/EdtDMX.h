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
#include "DMXSlaveConfig.h"
#include "DMXLedSpot.h"
#include "DMXThreeChannelLed.h"
#include "DMXShowTecCompactPar.h"
#include "DMXFixedSingleChannel.h"

namespace OSC
{
namespace Device
{
class EdtDMX : public OSC::MessageConsumer<OSC::StructMessage<OSC::EdtMessage, uint8_t>>
{
  private:
	const char *_pattern;
	OSC::DMX::Slave **_slaves;
	uint8_t _slaveCount = 0;

  public:
	EdtDMX(const char *pattern) : MessageConsumer()
	{
		_pattern = pattern;
	}

	const char *pattern()
	{
		return _pattern;
	}

	void initialize()
	{
		if (_slaveCount > 0)
		{
			delete[] _slaves;
		}

		_slaveCount = OSC::DMX::Slave::getSlaveCount();

		if (_slaveCount > 0)
		{
			_slaves = new OSC::DMX::Slave *[_slaveCount];
			for (uint8_t i = 0; i < _slaveCount; ++i)
			{
				OSC::DMX::SlaveConfig config = OSC::DMX::Slave::getSlaveConfig(i);

				switch (config.type)
				{
				case OSC::DMX::SlaveType::LedSpot:
					_slaves[i] = (OSC::DMX::Slave *)new OSC::DMX::LedSpot();
					break;

				case OSC::DMX::SlaveType::ShowTecCompactPar:
					_slaves[i] = (OSC::DMX::Slave *)new OSC::DMX::ShowTecCompactPar();
					break;
				
				case OSC::DMX::SlaveType::ThreeChannelLed:
					_slaves[i] = (OSC::DMX::Slave *)new OSC::DMX::ThreeChannelLed();
					break;

				case OSC::DMX::SlaveType::FixedSingleChannel:
					_slaves[i] = (OSC::DMX::Slave *)new OSC::DMX::FixedSingleChannel();
					break;

				case OSC::DMX::SlaveType::Unknown:
				default:
					continue;
				};

				_slaves[i]->initialize(config.address, config.maximumBrightness, config.minimumBrightness);

				_slaves[i]->solid(120, 255, 255);
				_slaves[i]->fade(2);
			}
		}
	}

	void callbackMessage(OSC::StructMessage<OSC::EdtMessage, uint8_t> *message)
	{
		// TODO: put loop in switch
		for (uint8_t i = 0; i < _slaveCount; ++i)
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
					_slaves[i]->solid(singleColor.hue, singleColor.saturation, singleColor.value);
				}

				if (command == OSC::ColorCommands::SinglePulse || singleColor.value == 0)
				{
					_slaves[i]->fade(singleColor.duration, FadeMode::FadeToBlack);
				}
				else if (command == OSC::ColorCommands::SingleSpark)
				{
					_slaves[i]->fade(singleColor.duration, FadeMode::FadeOneByOne);
				}
				else
				{
					_slaves[i]->disableFade();
				}

				break;

			case OSC::ColorCommands::DualPulse:
			case OSC::ColorCommands::DualSolid:
			case OSC::ColorCommands::DualSparkle:

				_slaves[i]->solid(dualColor.hue1, dualColor.hue2, 255, 254, dualColor.percentage);

				if (command == OSC::ColorCommands::DualPulse)
				{
					_slaves[i]->fade(dualColor.duration, FadeMode::FadeToBlack);
				}
				else if (command == OSC::ColorCommands::DualSparkle)
				{
					_slaves[i]->fade(dualColor.duration, FadeMode::FadeOneByOne);
				}
				else
				{
					_slaves[i]->disableFade();
				}

				break;

			case OSC::ColorCommands::VuMeter:

				if (vuMeter.intensity > 0)
				{
					_slaves[i]->intensity(vuMeter.intensity);
				}
				else
				{
					_slaves[i]->fade(127);
				}

				break;

			case OSC::ColorCommands::Twinkle:

				_slaves[i]->disableFade();

				if (twinkle.intensity > 0)
				{
					_slaves[i]->solid(twinkle.hue, 255, twinkle.intensity - 1);
				}
				else
				{
					_slaves[i]->fade(127);
				}

				break;

			case OSC::ColorCommands::Strobo:

				_slaves[i]->strobo(strobo.hue, strobo.intensity);

				break;

			case OSC::ColorCommands::Chase:
			case OSC::ColorCommands::Bash:
			case OSC::ColorCommands::RainbowPulse:
			case OSC::ColorCommands::RainbowSolid:
			case OSC::ColorCommands::RainbowSpark:

				// No implementation

				break;
			}
		}

		if (message->messageStruct.command == OSC::ColorCommands::DMXConfig)
		{

			auto dmxConfigCommand = message->messageStruct.commands.dmxConfig;
			if (dmxConfigCommand.clear > 0)
			{
				OSC::DMX::Slave::clearSlaveConfig();
			}

			if (dmxConfigCommand.slaveAddress > 0)
			{
				auto config = OSC::DMX::SlaveConfig();
				config.type = (OSC::DMX::SlaveType)dmxConfigCommand.slaveType;
				config.address = dmxConfigCommand.slaveAddress;
				config.maximumBrightness = dmxConfigCommand.maximumBrightness;
				config.minimumBrightness = dmxConfigCommand.minimumBrightness;

				OSC::DMX::Slave::setSlaveConfig(config);
			}

			if (dmxConfigCommand.reset > 0)
			{
				initialize();
			}
		}
	}

	void animationLoop()
	{
		for (uint8_t i = 0; i < _slaveCount; ++i)
		{
			_slaves[i]->loop();
		}
	}
};

} // namespace Device
} // namespace OSC