#include "RGBColorScheduler.h"

#define COLOR_INVERSE 255
#define COLOR_CORRECTION 0 //98

#define INTENSITY_BOOST 0.0

#define RAINBOW_POSITIONS 6

#define HUE(h) h
#define VALUE(v) v + ((255 - v) * INTENSITY_BOOST)

#ifndef _MSC_VER

#include "FastLED.h"
#include "FadeMode.h"
#include "SparkFun_Tlc5940.h"


OSC::Device::RGBColorScheduler::RGBColorScheduler()
{
}

OSC::Device::RGBColorScheduler::RGBColorScheduler(CRGB * leds, uint8_t nrOfLeds, Tlc5940 * tlc)
{
	_leds = leds;
	_nrOfLeds = nrOfLeds;

	_fadeBackup = new CRGB[_nrOfLeds];

	fill_solid(_leds, _nrOfLeds, CRGB::HTMLColorCode::Black);

	_ledState.fade = 255;

	_tlc = tlc;
}

void OSC::Device::RGBColorScheduler::fade(uint8_t speed, FadeMode fadeMode)
{
	_ledState.fade = speed;
	_fadeMode = fadeMode;

	if (_fadeMode == FadeMode::FadeOneByOne)
	{
		for (int i = 0; i < _nrOfLeds; i++) {
			_fadeBackup[i] = _leds[i];
		}
	}
}

void OSC::Device::RGBColorScheduler::disableFade()
{
	_ledState.fade = 255;
}

void OSC::Device::RGBColorScheduler::solid(uint8_t hue, uint8_t saturation, uint8_t value)
{
	fill_solid(_leds, _nrOfLeds, CHSV(HUE(hue), saturation, VALUE(value)));
}

void OSC::Device::RGBColorScheduler::solid(CHSV color)
{
	fill_solid(_leds, _nrOfLeds, color);
}

void OSC::Device::RGBColorScheduler::solid(uint8_t hue1, uint8_t hue2, uint8_t saturation, uint8_t value, uint8_t percentage)
{
	for (int i = 0; i < _nrOfLeds; i++) {
		if (percentage > random8()) {
			_leds[i].setHSV(HUE(hue2), saturation, VALUE(value));
		}
		else {
			_leds[i].setHSV(HUE(hue1), saturation, VALUE(value));
		}
	}
}

void OSC::Device::RGBColorScheduler::rainbow(uint8_t hue, uint8_t deltaHue)
{
	fill_rainbow(_leds, _nrOfLeds, HUE(hue), (deltaHue / 127.0) * (255.0 / _nrOfLeds));
}

void OSC::Device::RGBColorScheduler::intensity(uint8_t intensity) {
	if (intensity == 0) {
		fill_solid(_leds, _nrOfLeds, CRGB::HTMLColorCode::Black);
	}
	else {
		fill_solid(_leds, _nrOfLeds, CHSV(HUE(0) + (85 - (intensity / 2.5)), 255, VALUE(intensity)));
	}
}

void OSC::Device::RGBColorScheduler::strobo(uint8_t hue, uint8_t fps)
{
	disableFade();

	fill_solid(_leds, _nrOfLeds, CRGB::HTMLColorCode::Black);

	if (fps == 0)
	{
		_animations.resetAnimations();
	}
	else
	{
		if (hue < 255)
		{
			_animations.insertAnimation(Animation(AnimationType::Strobo, CHSV(HUE(hue), 255, 255), 255.0 / fps, 0));
		}
		else
		{
			_animations.insertAnimation(Animation(AnimationType::Strobo, CHSV(0, 0, 255), 255.0 / fps, 0));
		}
	}
}

void OSC::Device::RGBColorScheduler::loop()
{
	int i = 0;

	while (i < _animations.animationsActive)
	{
		switch (_animations.animations[i].type)
		{
		case AnimationType::Strobo:

			fill_solid(_leds, _nrOfLeds, CHSV(0, 0, 0));

			if ((_animations.animations[i].state++) > _animations.animations[i].data)
			{
				_animations.animations[i].state = 0;

				fill_solid(_leds, _nrOfLeds, _animations.animations[i].color);
			}

			// there is nothing else to animate besides flashing of the strobo
			output();
			return;
		}

		i++;
	}

	switch (_fadeMode)
	{
	case FadeMode::FadeToBlack:
		if (_ledState.fade < 255)
		{
			if (_ledState.fade > 255 - 62)
			{
				_ledState.fade = 255;
			}
			else
			{
				_ledState.fade += ((_ledState.fade) / 4) + 1;
			}

			fadeToBlackBy(_leds, _nrOfLeds, _ledState.fade);
		}
		break;
	case FadeMode::FadeOneByOne:
		if (_ledState.fade < 255)
		{
			if (_ledState.fade > random8())
			{
				for (int i = 0; i < _nrOfLeds; i++) {
					_fadeBackup[i] = _leds[i];
				}
				fill_solid(_leds, _nrOfLeds, CRGB::HTMLColorCode::Black);
			}
			else
			{
				if (_ledState.fade > 255 - 17)
				{
					_ledState.fade = 255;

					fill_solid(_leds, _nrOfLeds, CRGB::HTMLColorCode::Black);
				}
				else
				{
					_ledState.fade += ((_ledState.fade) / 16) + 1;

					for (int i = 0; i < _nrOfLeds; i++) {
						_leds[i] = _fadeBackup[i];
					}
					fadeToBlackBy(_leds, _nrOfLeds, _ledState.fade);
				}
			}
		}

		break;

	}

	output();
}

inline void OSC::Device::RGBColorScheduler::output() {
	int j = 0;
	for (int i = 0; i < _nrOfLeds; i++) {
		_tlc->set(++j, 4095 - (int)((((double)_leds[i].red) / 255.0) * 4095));
		_tlc->set(++j, 4095 - (int)((((double)_leds[i].green) / 255.0) * 4095));
		_tlc->set(++j, 4095 - (int)((((double)_leds[i].blue) / 255.0) * 4095));
	}
}

#else

OSC::Device::RGBColorScheduler::RGBColorScheduler() {
}

OSC::Device::RGBColorScheduler::RGBColorScheduler(CRGB * leds, uint8_t nrOfLeds, Tlc5940 * tlc) {
}

void OSC::Device::RGBColorScheduler::solid(uint8_t h, uint8_t s, uint8_t v) {
	commandColor = Command();
	commandColor.type = Type::solid;
	commandColor.h = h;
	commandColor.s = s;
	commandColor.v = v;
}

void OSC::Device::RGBColorScheduler::solid(uint8_t h1, uint8_t h2, uint8_t s, uint8_t v, uint8_t percentage) {
	commandColor = Command();
	commandColor.type = Type::solid;
	commandColor.h = h1;
	commandColor.dh = h2;
	commandColor.s = s;
	commandColor.v = v;
	commandColor.percentage = percentage;
}

void OSC::Device::RGBColorScheduler::fade(uint8_t duration, FadeMode mode) {
	commandFade = Command();
	commandFade.type = Type::fade;
	commandFade.duration = duration;
	commandFade.mode = mode;
}

void OSC::Device::RGBColorScheduler::disableFade() {
	commandFade = Command();
	commandFade.type = Type::disableFade;
}

void OSC::Device::RGBColorScheduler::rainbow(uint8_t h, uint8_t dh) {
	commandColor = Command();
	commandColor.type = Type::rainbow;
	commandColor.h = h;
	commandColor.dh = dh;
}

void OSC::Device::RGBColorScheduler::intensity(uint8_t intensity) {
	commandColor = Command();
	commandColor.type = Type::intensity;
	commandColor.intensity = intensity;
}

void OSC::Device::RGBColorScheduler::strobo(uint8_t h, uint8_t intensity) {
	commandColor = Command();
	commandColor.type = Type::strobo;
	commandColor.h = h;
	commandColor.intensity = intensity;
}

void OSC::Device::RGBColorScheduler::loop() {

}

Command OSC::Device::RGBColorScheduler::getCommandColor() {
	return commandColor;
}

Command OSC::Device::RGBColorScheduler::getCommandTwinkle() {
	return commandTwinkle;
}

Command OSC::Device::RGBColorScheduler::getCommandFade() {
	return commandFade;
}

#endif