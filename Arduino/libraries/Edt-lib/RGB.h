#pragma once

#include <OSCArduino.h>
#include <OSCMessageConsumer.h>
#include <OSCMessageDefinitions.h>
#include <FastLED.h>
//#include <FastLEDColorScheduler.h>
#include <OnOffLEDColorScheduler.h>
#include <FadeMode.h>

class EdtRGB : public OSC::StructMessageConsumer<OSC::ColorCommands, uint8_t>
{
  private:
	const char *_pattern;
	int _pinRed;
	int _pinGreen;
	int _pinBlue;

	OnOffLEDColorScheduler _colorSchedulerRed;
	OnOffLEDColorScheduler _colorSchedulerGreen;
	OnOffLEDColorScheduler _colorSchedulerBlue;

    union OSCBuffer {
		OSC::SingleColorCommand singleColor;
		OSC::DualColorCommand dualColor;
		OSC::RainbowCommand rainbow;
		OSC::VuMeterCommand vuMeter;
		OSC::TwinkleCommand twinkle;
		OSC::StroboCommand strobo;
		OSC::KittCommand kitt;
	} buffer;

	//FastLEDColorScheduler _colorScheduler;

  public:

	EdtRGB(const char *pattern, uint8_t const pinRed, uint8_t const pinGreen, uint8_t const pinBlue) : StructMessageConsumer(13)
	{
		_pattern = pattern;

		_pinRed = pinRed;
		_pinGreen = pinGreen;
		_pinBlue = pinBlue;
		pinMode(_pinRed, OUTPUT);
		pinMode(_pinGreen, OUTPUT);
		pinMode(_pinBlue, OUTPUT);

		_colorSchedulerRed = OnOffLEDColorScheduler(_pinRed, false);
		_colorSchedulerGreen = OnOffLEDColorScheduler(_pinGreen, false);
		_colorSchedulerBlue = OnOffLEDColorScheduler(_pinBlue, false);

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
		digitalWrite(_pinRed, 255);
		digitalWrite(_pinGreen, 220);
		digitalWrite(_pinBlue, 245);

		//_colorSchedulerRed.output(255);
		//_colorSchedulerRed.fade(4);
		//_colorSchedulerGreen.output(255);
		//_colorSchedulerGreen.fade(4);
		//_colorSchedulerBlue.output(255);
		//_colorSchedulerBlue.fade(4);
	}

	void callbackEnum(OSC::ColorCommands command)
	{

		switch(command) {

		case OSC::ColorCommands::SinglePulse:
		case OSC::ColorCommands::SingleSolid:

			if(buffer.singleColor.value > 0) {
				_colorSchedulerRed.output(buffer.singleColor.value);
				_colorSchedulerGreen.output(buffer.singleColor.value);
				_colorSchedulerBlue.output(buffer.singleColor.value);
			}
			
			if (command == OSC::ColorCommands::SinglePulse || buffer.singleColor.value == 0) {
				_colorSchedulerRed.fade(buffer.singleColor.duration);
				_colorSchedulerGreen.fade(buffer.singleColor.duration);
				_colorSchedulerBlue.fade(buffer.singleColor.duration);
			}
			else {
				_colorSchedulerRed.disableFade();
				_colorSchedulerGreen.disableFade();
				_colorSchedulerBlue.disableFade();
			}

		 	break;

		case OSC::ColorCommands::RainbowPulse:
		case OSC::ColorCommands::RainbowSolid:

			_colorSchedulerRed.output(127);
			_colorSchedulerGreen.output(127);
			_colorSchedulerBlue.output(127);
			
			if(command == OSC::ColorCommands::RainbowPulse) {
				_colorSchedulerRed.fade(buffer.rainbow.duration);				
				_colorSchedulerGreen.fade(buffer.rainbow.duration);				
				_colorSchedulerBlue.fade(buffer.rainbow.duration);				
			}
			else {
				_colorSchedulerRed.disableFade();
				_colorSchedulerGreen.disableFade();
				_colorSchedulerBlue.disableFade();
			}

			break;

		case OSC::ColorCommands::VuMeter:

			if (buffer.vuMeter.intensity > 0) {
				_colorSchedulerRed.output(buffer.vuMeter.intensity / 2);
				_colorSchedulerGreen.output(buffer.vuMeter.intensity / 2);
				_colorSchedulerBlue.output(buffer.vuMeter.intensity / 2);
				_colorSchedulerRed.disableFade();
				_colorSchedulerGreen.disableFade();
				_colorSchedulerBlue.disableFade();
			}

			break;

		case OSC::ColorCommands::Twinkle:
		
			_colorSchedulerRed.disableFade();
			_colorSchedulerGreen.disableFade();
			_colorSchedulerBlue.disableFade();

			if (buffer.twinkle.intensity > 0) {

				if (buffer.twinkle.intensity > random8()) {
					_colorSchedulerRed.output(255);
				}
				else if (buffer.twinkle.intensity > random8()) {
					_colorSchedulerGreen.output(255);
				}
				else if (buffer.twinkle.intensity > random8()) {
					_colorSchedulerBlue.output(255);
				}
				else {
					_colorSchedulerRed.output(0);
					_colorSchedulerGreen.output(0);
					_colorSchedulerBlue.output(0);
				}
			}
			else {
				_colorSchedulerRed.fade(127);
				_colorSchedulerGreen.fade(127);
				_colorSchedulerBlue.fade(127);
			}

			break;

		case OSC::ColorCommands::Strobo:

			_colorSchedulerRed.strobo(buffer.strobo.intensity);
			_colorSchedulerGreen.strobo(buffer.strobo.intensity);
			_colorSchedulerBlue.strobo(buffer.strobo.intensity);

			break;
		}
	}

	void animationLoop()
	{
		_colorSchedulerRed.loop();
		_colorSchedulerGreen.loop();
		_colorSchedulerBlue.loop();
	}
};
