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

	OSC::SingleColorCommand singleColor;
	OSC::DualColorCommand dualColor;
	OSC::RainbowCommand rainbow;
	OSC::VuMeterCommand vuMeter;
	OSC::TwinkleCommand twinkle;
	OSC::StroboCommand strobo;
	OSC::KittCommand kitt;

	FastLEDColorScheduler _colorScheduler;

  public:

	EdtRGBLED(const char *pattern, uint8_t const nrOfLeds) : StructMessageConsumer(13)
	{
		_pattern = pattern;

		_nrOfLeds = nrOfLeds;
		_leds = new CRGB[_nrOfLeds];

		_colorScheduler = FastLEDColorScheduler(_leds, _nrOfLeds);

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

			if (dualColor.hue1 > 0)
			{
				_colorScheduler.solid(dualColor.start, dualColor.end, dualColor.hue1, 240, 255);

				if (dualColor.hue2 > 0 && dualColor.percentage > 0)
				{
					_colorScheduler.twinkle(dualColor.start, dualColor.end, dualColor.hue2, 240, 255, dualColor.percentage, false);
				}
			}

			if (command == OSC::ColorCommands::DualPulse || dualColor.hue1 == 0)
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
				_colorScheduler.twinkle(twinkle.start, twinkle.end, twinkle.hue, 240, 255, twinkle.intensity);
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
			
			if(kitt.position > 0) {
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