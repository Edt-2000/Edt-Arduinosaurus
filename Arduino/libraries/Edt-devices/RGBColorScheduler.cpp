#include "RGBColorScheduler.h"

#define HUE(h) h
#define VALUE(v) v // + ((255 - v) * INTENSITY_BOOST)

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
	this->nrOfLeds = nrOfLeds;

	fill_solid(_leds, nrOfLeds, CRGB::HTMLColorCode::Black);

	_ledState.fade = 255;

	_tlc = tlc;
}

void OSC::Device::RGBColorScheduler::fade(uint8_t speed, FadeMode fadeMode)
{
	_ledState.fade = speed;
}

void OSC::Device::RGBColorScheduler::disableFade()
{
	_ledState.fade = 255;
}

void OSC::Device::RGBColorScheduler::solid(uint8_t hue, uint8_t saturation, uint8_t value)
{
	fill_solid(_leds, nrOfLeds, CHSV(HUE(hue), saturation, VALUE(value)));
}

void OSC::Device::RGBColorScheduler::solid(CHSV color)
{
	fill_solid(_leds, nrOfLeds, color);
}

void OSC::Device::RGBColorScheduler::solid(uint8_t hue1, uint8_t hue2, uint8_t saturation, uint8_t value, uint8_t percentage)
{
	for (int i = 0; i < nrOfLeds; i++) {
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
	fill_rainbow(_leds, nrOfLeds, HUE(hue), (deltaHue / 127.0) * (255.0 / nrOfLeds));
}

void OSC::Device::RGBColorScheduler::intensity(uint8_t intensity) {
	if (intensity == 0) {
		fill_solid(_leds, nrOfLeds, CRGB::HTMLColorCode::Black);
	}
	else {
		fill_solid(_leds, nrOfLeds, CHSV(HUE(0) + (85 - (intensity / 2.5)), 255, VALUE(intensity)));
	}
}

void OSC::Device::RGBColorScheduler::strobo(uint8_t hue, uint8_t fps)
{
	disableFade();

	fill_solid(_leds, nrOfLeds, CRGB::HTMLColorCode::Black);

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

			fill_solid(_leds, nrOfLeds, CHSV(0, 0, 0));

			if ((_animations.animations[i].state++) > _animations.animations[i].data)
			{
				_animations.animations[i].state = 0;

				fill_solid(_leds, nrOfLeds, _animations.animations[i].color);
			}

			// there is nothing else to animate besides flashing of the strobo
			output();
			return;
		}

		i++;
	}

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

		fadeToBlackBy(_leds, nrOfLeds, _ledState.fade);
	}
	
	output();
}

inline void OSC::Device::RGBColorScheduler::output() {
	// moronic pinout:
	// 
	// | AUX | LED 1 | LED 2 | LED 3  | LED 4    | LED 5   |
	// | 1   | 2 3 4 | 5 6 7 | 8 9 10 | 11 12 13 | 14 15 0 |
	// |     | G B R | G B R | G B R  | G  B  R  | G  B  R |
	//
	// (who designed the board? :P)

	// tclAddress is a struct with a 4 bit integer
	// so 15 + 1 = 0 automatically
	tlcAddress tlc;

	tlc.address = 1;

	for (int i = 0; i < nrOfLeds; i++) {
		uint8_t red = _leds[i].red;
		uint8_t blue = qsub8(_leds[i].blue, red / 8);
		uint8_t green = qsub8(qsub8(_leds[i].green, red / 6), blue / 8);

		_tlc->set(++tlc.address, 4095 - (int)((((double)green) / 255.0) * 4095));
		_tlc->set(++tlc.address, 4095 - (int)((((double)blue) / 255.0) * 4095));
		_tlc->set(++tlc.address, 4095 - (int)((((double)red) / 255.0) * 4095));
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