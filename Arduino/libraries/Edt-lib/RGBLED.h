#pragma once

#include <OSCArduino.h>
#include <OSCMessageConsumer.h>
#include <OSCMessageDefinitions.h>
#include <FastLED.h>
#include <FastLEDColorScheduler.h>
#include <FadeMode.h>

class EdtRGBLED : public OSC::StructMessageConsumer<OSC::ColorCommands, uint8_t>
{
  private:
	const char *_pattern;
	CRGB *_leds;
	int _nrOfLeds;

    union OSCBuffer {
		OSC::SingleColorCommand singleColor;
		OSC::DualColorCommand dualColor;
		OSC::RainbowCommand rainbow;
		OSC::VuMeterCommand vuMeter;
		OSC::TwinkleCommand twinkle;
		OSC::StroboCommand strobo;
		OSC::KittCommand kitt;
	} buffer;

	FastLEDColorScheduler _colorScheduler;

  public:

	EdtRGBLED(const char *pattern, uint8_t const nrOfLeds) : StructMessageConsumer(12)
	{
		_pattern = pattern;

		_nrOfLeds = nrOfLeds;
		_leds = new CRGB[_nrOfLeds];

		_colorScheduler = FastLEDColorScheduler(_leds, _nrOfLeds);

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

	template <uint8_t dataPin, uint8_t clockPin>
	void configurePins()
	{
		FastLED.addLeds<APA102, dataPin, clockPin, BRG>(_leds, _nrOfLeds);

		_colorScheduler.solid(0, 127, 32, 240, 32);
	}

	void test()
	{
		_colorScheduler.solid(0, 127, 0, 240, 255);
		_colorScheduler.fade(0, 127, 2);
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
				_colorScheduler.solid(buffer.singleColor.start, buffer.singleColor.end, buffer.singleColor.hue, buffer.singleColor.saturation, buffer.singleColor.value);
			}

			if (command == OSC::ColorCommands::SinglePulse || buffer.singleColor.value == 0)
			{
				_colorScheduler.fade(buffer.singleColor.start, buffer.singleColor.end, buffer.singleColor.duration, FadeMode::FadeToBlack);
			}
			else if (command == OSC::ColorCommands::SingleSpark)
			{
				_colorScheduler.fade(buffer.singleColor.start, buffer.singleColor.end, buffer.singleColor.duration, FadeMode::FadeOneByOne);
			}
			else
			{
				_colorScheduler.disableFade(buffer.singleColor.start, buffer.singleColor.end);
			}

			break;

		case OSC::ColorCommands::DualPulse:
		case OSC::ColorCommands::DualSolid:
		case OSC::ColorCommands::DualSparkle:

			if (buffer.dualColor.hue1 > 0)
			{
				_colorScheduler.solid(buffer.dualColor.start, buffer.dualColor.end, buffer.dualColor.hue1, 240, 255);

				if (buffer.dualColor.hue2 > 0 && buffer.dualColor.percentage > 0)
				{
					_colorScheduler.twinkle(buffer.dualColor.start, buffer.dualColor.end, buffer.dualColor.hue2, 240, 255, buffer.dualColor.percentage, false);
				}
			}

			if (command == OSC::ColorCommands::DualPulse || buffer.dualColor.hue1 == 0)
			{
				_colorScheduler.fade(buffer.dualColor.start, buffer.dualColor.end, buffer.dualColor.duration, FadeMode::FadeToBlack);
			}
			else if (command == OSC::ColorCommands::DualSparkle)
			{
				_colorScheduler.fade(buffer.dualColor.start, buffer.dualColor.end, buffer.dualColor.duration, FadeMode::FadeOneByOne);
			}
			else
			{
				_colorScheduler.disableFade(buffer.dualColor.start, buffer.dualColor.end);
			}

			break;

		case OSC::ColorCommands::RainbowPulse:
		case OSC::ColorCommands::RainbowSolid:
		case OSC::ColorCommands::RainbowSpark:

			if (buffer.rainbow.deltaHue > 0)
			{
				_colorScheduler.rainbow(buffer.rainbow.start, buffer.rainbow.end, buffer.rainbow.hue, buffer.rainbow.deltaHue);
			}

			if (command == OSC::ColorCommands::RainbowPulse || buffer.rainbow.deltaHue == 0)
			{
				_colorScheduler.fade(buffer.rainbow.start, buffer.rainbow.end, buffer.rainbow.duration, FadeMode::FadeToBlack);
			}
			else if (command == OSC::ColorCommands::RainbowSpark)
			{
				_colorScheduler.fade(buffer.rainbow.start, buffer.rainbow.end, buffer.rainbow.duration, FadeMode::FadeOneByOne);
			}
			else
			{
				_colorScheduler.disableFade(buffer.rainbow.start, buffer.rainbow.end);
			}

			break;

		case OSC::ColorCommands::VuMeter:

			_colorScheduler.rainbow(buffer.vuMeter.start, buffer.vuMeter.center, buffer.vuMeter.end, buffer.vuMeter.hue, buffer.vuMeter.deltaHue, buffer.vuMeter.intensity);

			break;

		case OSC::ColorCommands::Twinkle:

			_colorScheduler.disableFade(buffer.twinkle.start, buffer.twinkle.end);

			if (buffer.twinkle.intensity > 0)
			{
				_colorScheduler.twinkle(buffer.twinkle.start, buffer.twinkle.end, buffer.twinkle.hue, 240, 255, buffer.twinkle.intensity);
			}
			else
			{
				_colorScheduler.fade(buffer.twinkle.start, buffer.twinkle.end, 127);
			}

			break;

		case OSC::ColorCommands::Strobo:

			_colorScheduler.strobo(buffer.strobo.hue, buffer.strobo.intensity);

			break;

		case OSC::ColorCommands::Kitt:
			
			if(buffer.kitt.position > 0) {
				_colorScheduler.kitt(buffer.kitt.position, buffer.kitt.length, buffer.kitt.hue);
			}
			else {
				_colorScheduler.fade(0, 127, 127);
			}

			break;
		}
	}

	void
	animationLoop()
	{

		_colorScheduler.loop();
	}
};
