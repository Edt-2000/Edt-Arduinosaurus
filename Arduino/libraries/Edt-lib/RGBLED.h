#pragma once

#include <OSCArduino.h>
#include <OSCMessageConsumer.h>
#include <OSCMessageDefinitions.h>
#include <FastLED.h>
#include <FastLEDColorScheduler.h>
#include <FadeMode.h>

class EdtRGBLED : public OSC::StructMessageConsumer<OSC::ColorCommands>
{
  private:
	const char *_pattern;
	CRGB *_leds;
	int _nrOfLeds;

	FastLEDColorScheduler _colorScheduler;

  public:
	// OSC::SingleColorCommand singleColor;
	// OSC::DualColorCommand dualColor;
	// OSC::RainbowCommand rainbow;
	// OSC::VuMeterCommand vuMeter;
	// OSC::TwinkleCommand twinkle;
	// OSC::StroboCommand strobo;
	// OSC::KittCommand kitt;

	EdtRGBLED(const char *pattern, uint8_t const nrOfLeds) : StructMessageConsumer(12)
	{
		_pattern = pattern;

		_nrOfLeds = nrOfLeds;
		_leds = new CRGB[_nrOfLeds];

		_colorScheduler = FastLEDColorScheduler(_leds, _nrOfLeds);

		//addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::SinglePulse, &EdtLEDMessageBuffer::buffer);
		// addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::SingleSolid, &EdtLEDMessageBuffer::buffer);
		// addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::SingleSpark, &EdtLEDMessageBuffer::buffer);
		// addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::RainbowPulse, &EdtLEDMessageBuffer::buffer);
		// addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::RainbowSolid, &EdtLEDMessageBuffer::buffer);
		// addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::RainbowSpark, &EdtLEDMessageBuffer::buffer);
		// addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::VuMeter, &EdtLEDMessageBuffer::buffer);
		// addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::Twinkle, &EdtLEDMessageBuffer::buffer);
		// addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::Strobo, &EdtLEDMessageBuffer::buffer);
		// addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::DualPulse, &EdtLEDMessageBuffer::buffer);
		// addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::DualSolid, &EdtLEDMessageBuffer::buffer);
		// addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::DualSparkle, &EdtLEDMessageBuffer::buffer);
		// addEnumToStructMapping<OSC::CommandBuffer>(OSC::ColorCommands::Kitt, &EdtLEDMessageBuffer::buffer);
	}

	const char *pattern()
	{
		return _pattern;
	}

	template <uint8_t dataPin, uint8_t clockPin>
	void configurePins()
	{
		FastLED.addLeds<APA102, dataPin, clockPin, BRG>(_leds, _nrOfLeds);

		fill_solid(_leds, _nrOfLeds, CHSV(32, 240, 32));
	}

	void test()
	{
		fill_solid(_leds, _nrOfLeds, CHSV(0, 240, 255));

		_colorScheduler.fade(0, 127, 2);
	}

	void callbackEnum(OSC::ColorCommands command)
	{

		switch (command)
		{

		case OSC::ColorCommands::SinglePulse:
		case OSC::ColorCommands::SingleSolid:
		case OSC::ColorCommands::SingleSpark:

			if (EdtLEDMessageBuffer::buffer.singleColor.value > 0)
			{
				_colorScheduler.solid(EdtLEDMessageBuffer::buffer.singleColor.start, 
					EdtLEDMessageBuffer::buffer.singleColor.end, 
					EdtLEDMessageBuffer::buffer.singleColor.hue, 
					EdtLEDMessageBuffer::buffer.singleColor.saturation, 
					EdtLEDMessageBuffer::buffer.singleColor.value);
			}

			if (command == OSC::ColorCommands::SinglePulse || EdtLEDMessageBuffer::buffer.singleColor.value == 0)
			{
				_colorScheduler.fade(EdtLEDMessageBuffer::buffer.singleColor.start, 
					EdtLEDMessageBuffer::buffer.singleColor.end, 
					EdtLEDMessageBuffer::buffer.singleColor.duration, 
					FadeMode::FadeToBlack);
			}
			else if (command == OSC::ColorCommands::SingleSpark)
			{
				_colorScheduler.fade(EdtLEDMessageBuffer::buffer.singleColor.start, 
					EdtLEDMessageBuffer::buffer.singleColor.end, 
					EdtLEDMessageBuffer::buffer.singleColor.duration, FadeMode::FadeOneByOne);
			}
			else
			{
				_colorScheduler.disableFade(EdtLEDMessageBuffer::buffer.singleColor.start, 
					EdtLEDMessageBuffer::buffer.singleColor.end);
			}

			break;

		case OSC::ColorCommands::DualPulse:
		case OSC::ColorCommands::DualSolid:
		case OSC::ColorCommands::DualSparkle:

			if (EdtLEDMessageBuffer::buffer.dualColor.hue1 > 0)
			{
				_colorScheduler.solid(EdtLEDMessageBuffer::buffer.dualColor.start, EdtLEDMessageBuffer::buffer.dualColor.end, EdtLEDMessageBuffer::buffer.dualColor.hue1, 240, 255);

				if (EdtLEDMessageBuffer::buffer.dualColor.hue2 > 0 && EdtLEDMessageBuffer::buffer.dualColor.percentage > 0)
				{
					_colorScheduler.twinkle(EdtLEDMessageBuffer::buffer.dualColor.start, 
						EdtLEDMessageBuffer::buffer.dualColor.end, EdtLEDMessageBuffer::buffer.dualColor.hue2, 240, 255, EdtLEDMessageBuffer::buffer.dualColor.percentage, false);
				}
			}

			if (command == OSC::ColorCommands::DualPulse || EdtLEDMessageBuffer::buffer.dualColor.hue1 == 0)
			{
				_colorScheduler.fade(EdtLEDMessageBuffer::buffer.dualColor.start, EdtLEDMessageBuffer::buffer.dualColor.end, EdtLEDMessageBuffer::buffer.dualColor.duration, FadeMode::FadeToBlack);
			}
			else if (command == OSC::ColorCommands::DualSparkle)
			{
				_colorScheduler.fade(EdtLEDMessageBuffer::buffer.dualColor.start, EdtLEDMessageBuffer::buffer.dualColor.end, EdtLEDMessageBuffer::buffer.dualColor.duration, FadeMode::FadeOneByOne);
			}
			else
			{
				_colorScheduler.disableFade(EdtLEDMessageBuffer::buffer.dualColor.start, EdtLEDMessageBuffer::buffer.dualColor.end);
			}

			break;

		case OSC::ColorCommands::RainbowPulse:
		case OSC::ColorCommands::RainbowSolid:
		case OSC::ColorCommands::RainbowSpark:

			if (EdtLEDMessageBuffer::buffer.rainbow.deltaHue > 0)
			{
				_colorScheduler.rainbow(EdtLEDMessageBuffer::buffer.rainbow.start, EdtLEDMessageBuffer::buffer.rainbow.end, EdtLEDMessageBuffer::buffer.rainbow.hue, EdtLEDMessageBuffer::buffer.rainbow.deltaHue);
			}

			if (command == OSC::ColorCommands::RainbowPulse || EdtLEDMessageBuffer::buffer.rainbow.deltaHue == 0)
			{
				_colorScheduler.fade(EdtLEDMessageBuffer::buffer.rainbow.start, EdtLEDMessageBuffer::buffer.rainbow.end, EdtLEDMessageBuffer::buffer.rainbow.duration, FadeMode::FadeToBlack);
			}
			else if (command == OSC::ColorCommands::RainbowSpark)
			{
				_colorScheduler.fade(EdtLEDMessageBuffer::buffer.rainbow.start, EdtLEDMessageBuffer::buffer.rainbow.end, EdtLEDMessageBuffer::buffer.rainbow.duration, FadeMode::FadeOneByOne);
			}
			else
			{
				_colorScheduler.disableFade(EdtLEDMessageBuffer::buffer.rainbow.start, EdtLEDMessageBuffer::buffer.rainbow.end);
			}

			break;

		case OSC::ColorCommands::VuMeter:

			_colorScheduler.rainbow(EdtLEDMessageBuffer::buffer.vuMeter.start, 
				EdtLEDMessageBuffer::buffer.vuMeter.center, 
				EdtLEDMessageBuffer::buffer.vuMeter.end, 
				EdtLEDMessageBuffer::buffer.vuMeter.hue, 
				EdtLEDMessageBuffer::buffer.vuMeter.deltaHue, 
				EdtLEDMessageBuffer::buffer.vuMeter.intensity);

			break;

		case OSC::ColorCommands::Twinkle:

			_colorScheduler.disableFade(EdtLEDMessageBuffer::buffer.twinkle.start, 
				EdtLEDMessageBuffer::buffer.twinkle.end);

			if (EdtLEDMessageBuffer::buffer.twinkle.intensity > 0)
			{
				_colorScheduler.twinkle(EdtLEDMessageBuffer::buffer.twinkle.start, EdtLEDMessageBuffer::buffer.twinkle.end, EdtLEDMessageBuffer::buffer.twinkle.hue, 240, 255, EdtLEDMessageBuffer::buffer.twinkle.intensity);
			}
			else
			{
				_colorScheduler.fade(EdtLEDMessageBuffer::buffer.twinkle.start, EdtLEDMessageBuffer::buffer.twinkle.end, 127);
			}

			break;

		case OSC::ColorCommands::Strobo:

			_colorScheduler.strobo(EdtLEDMessageBuffer::buffer.strobo.hue, EdtLEDMessageBuffer::buffer.strobo.intensity);

			break;

		case OSC::ColorCommands::Kitt:
			
			if(EdtLEDMessageBuffer::buffer.kitt.position > 0) {
				_colorScheduler.kitt(EdtLEDMessageBuffer::buffer.kitt.position, EdtLEDMessageBuffer::buffer.kitt.length, EdtLEDMessageBuffer::buffer.kitt.hue);
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
