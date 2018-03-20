#pragma once

#include <OSCArduino.h>
#include <OSCMessageConsumer.h>
#include <OSCMessageDefinitions.h>
#include <FastLED.h>
#include <RGBLEDColorScheduler.h>
#include <FadeMode.h>

class EdtRGB : public OSC::StructMessageConsumer<OSC::ColorCommands, uint8_t>
{
  private:
	const char *_pattern;

	RGBLEDColorScheduler _colorScheduler;

	union OSCBuffer {
		OSC::SingleColorCommand singleColor;
		OSC::DualColorCommand dualColor;
		OSC::RainbowCommand rainbow;
		OSC::VuMeterCommand vuMeter;
		OSC::TwinkleCommand twinkle;
		OSC::StroboCommand strobo;
		OSC::KittCommand kitt;
	} buffer;

  public:
	EdtRGB(const char *pattern, uint8_t const redChannel, uint8_t const greenChannel, uint8_t const blueChannel, Tlc5940 * tlc) : StructMessageConsumer(13)
	{
		_pattern = pattern;

		_colorScheduler = RGBLEDColorScheduler(redChannel, greenChannel, blueChannel, 1, tlc);

		addEnumToStructMapping<OSC::SingleColorCommand>(OSC::ColorCommands::SinglePulse, &buffer.singleColor);
		addEnumToStructMapping<OSC::SingleColorCommand>(OSC::ColorCommands::SingleSolid, &buffer.singleColor);
		addEnumToStructMapping<OSC::SingleColorCommand>(OSC::ColorCommands::SingleSpark, &buffer.singleColor);
		addEnumToStructMapping<OSC::RainbowCommand>(OSC::ColorCommands::RainbowPulse, &buffer.rainbow);
		addEnumToStructMapping<OSC::RainbowCommand>(OSC::ColorCommands::RainbowSolid, &buffer.rainbow);
		addEnumToStructMapping<OSC::RainbowCommand>(OSC::ColorCommands::RainbowSpark, &buffer.rainbow);
		addEnumToStructMapping<OSC::VuMeterCommand>(OSC::ColorCommands::VuMeter, &buffer.vuMeter);
		addEnumToStructMapping<OSC::TwinkleCommand>(OSC::ColorCommands::Twinkle, &buffer.twinkle);
		addEnumToStructMapping<OSC::StroboCommand>(OSC::ColorCommands::Strobo, &buffer.strobo);
		addEnumToStructMapping<OSC::DualColorCommand>(OSC::ColorCommands::DualPulse, &buffer.dualColor);
		addEnumToStructMapping<OSC::DualColorCommand>(OSC::ColorCommands::DualSolid, &buffer.dualColor);
		addEnumToStructMapping<OSC::DualColorCommand>(OSC::ColorCommands::DualSparkle, &buffer.dualColor);
		addEnumToStructMapping<OSC::KittCommand>(OSC::ColorCommands::Kitt, &buffer.kitt);
	}

	const char *pattern()
	{
		return _pattern;
	}

	void test()
	{
		_colorScheduler.solid(0, 255, 240);
		_colorScheduler.fade(2);
	}

	void callbackEnum(OSC::ColorCommands command)
	{
		switch (command)
		{

		case OSC::ColorCommands::SinglePulse:
		case OSC::ColorCommands::SingleSolid:
		case OSC::ColorCommands::SingleSpark:

			if (buffer.singleColor.value > 0)
			{
				_colorScheduler.solid(buffer.singleColor.hue, buffer.singleColor.saturation, buffer.singleColor.value);
			}

			if (command == OSC::ColorCommands::SinglePulse || buffer.singleColor.value == 0)
			{
				_colorScheduler.fade(buffer.singleColor.duration, FadeMode::FadeToBlack);
			}
			else if (command == OSC::ColorCommands::SingleSpark)
			{
				_colorScheduler.fade(buffer.singleColor.duration, FadeMode::FadeOneByOne);
			}
			else
			{
				_colorScheduler.disableFade();
			}

			break;

		case OSC::ColorCommands::DualPulse:
		case OSC::ColorCommands::DualSolid:
		case OSC::ColorCommands::DualSparkle:

			if (buffer.dualColor.hue1 > 0)
			{
				_colorScheduler.solid(buffer.dualColor.hue1, buffer.dualColor.hue2, 240, 254);
			}

			if (command == OSC::ColorCommands::DualPulse || buffer.dualColor.hue1 == 0)
			{
				_colorScheduler.fade(buffer.dualColor.duration, FadeMode::FadeToBlack);
			}
			else if (command == OSC::ColorCommands::DualSparkle)
			{
				_colorScheduler.fade(buffer.dualColor.duration, FadeMode::FadeOneByOne);
			}
			else
			{
				_colorScheduler.disableFade();
			}

			break;

		case OSC::ColorCommands::RainbowPulse:
		case OSC::ColorCommands::RainbowSolid:
		case OSC::ColorCommands::RainbowSpark:

			if (buffer.rainbow.deltaHue > 0)
			{
				_colorScheduler.rainbow(buffer.rainbow.hue, buffer.rainbow.deltaHue);
			}

			if (command == OSC::ColorCommands::RainbowPulse || buffer.rainbow.deltaHue == 0)
			{
				_colorScheduler.fade(buffer.rainbow.duration, FadeMode::FadeToBlack);
			}
			else if (command == OSC::ColorCommands::RainbowSpark)
			{
				_colorScheduler.fade(buffer.rainbow.duration, FadeMode::FadeOneByOne);
			}
			else
			{
				_colorScheduler.disableFade();
			}

		case OSC::ColorCommands::VuMeter:

			if (buffer.vuMeter.intensity > 0)
			{
				_colorScheduler.intensity(buffer.vuMeter.intensity);
			}
			else
			{
				_colorScheduler.fade(127);
			}

			break;

		case OSC::ColorCommands::Twinkle:

			_colorScheduler.disableFade();

			if (buffer.twinkle.intensity > 0)
			{
				_colorScheduler.solid(buffer.twinkle.hue, 240, buffer.twinkle.intensity - 1);
			}
			else
			{
				_colorScheduler.fade(127);
			}

			break;

		case OSC::ColorCommands::Strobo:

			_colorScheduler.strobo(buffer.strobo.intensity);

			break;
		}
	}

	void animationLoop()
	{
		_colorScheduler.loop();
	}
};
