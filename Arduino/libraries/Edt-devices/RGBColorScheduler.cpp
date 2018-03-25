#include "RGBColorScheduler.h"

#define COLOR_INVERSE 255
#define COLOR_CORRECTION 0 //98

#define INTENSITY_BOOST 0.3

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

OSC::Device::RGBColorScheduler::RGBColorScheduler(uint8_t redChannel, uint8_t greenChannel, uint8_t blueChannel, uint8_t rainbowPos, Tlc5940 * tlc)
{
	_channels[0] = redChannel;
	_channels[1] = greenChannel;
	_channels[2] = blueChannel;

	_rainbowPos = rainbowPos * (255 / RAINBOW_POSITIONS);

	_color.setColorCode(CRGB::HTMLColorCode::Black);

	_fade = 255;

	_strobo.active = false;

	_tlc = tlc;
}

void OSC::Device::RGBColorScheduler::fade(uint8_t speed, FadeMode fadeMode)
{
	_fade = speed;
	_fadeMode = fadeMode;

	if (_fadeMode == FadeMode::FadeOneByOne)
	{
		_fadeBackup = _color;
	}
}

void OSC::Device::RGBColorScheduler::disableFade()
{
	_fade = 255;
}

void OSC::Device::RGBColorScheduler::solid(uint8_t hue, uint8_t saturation, uint8_t value)
{
	fill_solid(&_color, 1, CHSV(HUE(hue), saturation, VALUE(value)));
}

void OSC::Device::RGBColorScheduler::solid(uint8_t hue1, uint8_t hue2, uint8_t saturation, uint8_t value, uint8_t percentage)
{
	if (percentage > random8()) {
		fill_solid(&_color, 1, CHSV(HUE(hue2), saturation, value + ((255 - value) * INTENSITY_BOOST)));
	}
	else {
		fill_solid(&_color, 1, CHSV(HUE(hue1), saturation, value + ((255 - value) * INTENSITY_BOOST)));
	}
}

void OSC::Device::RGBColorScheduler::rainbow(uint8_t hue, uint8_t deltaHue)
{
	fill_solid(&_color, 1, CHSV(HUE(hue) + ((deltaHue / 127.0) * _rainbowPos), 255, 255));
}

void OSC::Device::RGBColorScheduler::intensity(uint8_t intensity) {
	if (intensity == 0) {
		_color.setColorCode(CRGB::HTMLColorCode::Black);
	}
	else {
		fill_solid(&_color, 1, CHSV(HUE(0) + (85 - (intensity / 2.5)), 255, VALUE(intensity)));
	}
}

void OSC::Device::RGBColorScheduler::strobo(uint8_t hue, uint8_t fps)
{
	disableFade();

	_color.setColorCode(CRGB::HTMLColorCode::Black);

	_strobo.active = fps > 0;
	_strobo.loop = 0;
	_strobo.fpl = (255.0 / fps);
	if (hue == 0) {
		_strobo.color.h = 0;
		_strobo.color.s = 0;
		_strobo.color.l = 255;
	}
	else {
		_strobo.color.h = hue;
		_strobo.color.s = 255;
		_strobo.color.l = 255;
	}
}

void OSC::Device::RGBColorScheduler::loop()
{
	if (_strobo.active)
	{
		_color.setColorCode(CRGB::HTMLColorCode::Black);

		if ((_strobo.loop++) > _strobo.fpl)
		{
			_strobo.loop = 0;

			_color = _strobo.color.chsv();

			//_color.setColorCode(CRGB::HTMLColorCode::White);
		}
	}
	else
	{
		switch (_fadeMode)
		{
		case FadeMode::FadeToBlack:
			if (_fade < 255)
			{
				if (_fade > 255 - 62)
				{
					_fade = 255;
				}
				else
				{
					_fade += ((_fade) / 4) + 1;
				}

				fadeToBlackBy(&_color, 1, _fade);
			}
			break;
		case FadeMode::FadeOneByOne:
			if (_fade < 255)
			{
				if (_fade > random8())
				{
					_fadeBackup = _color;
					_color.setColorCode(CRGB::HTMLColorCode::Black);
				}
				else
				{
					if (_fade > 255 - 17)
					{
						_fade = 255;

						_color.setColorCode(CRGB::HTMLColorCode::Black);
					}
					else
					{
						_fade += ((_fade) / 16) + 1;

						_color = _fadeBackup;
						fadeToBlackBy(&_color, 1, _fade);
					}
				}
			}

			break;
		}
	}

	_tlc->set(_channels[0], 4095 - (int)((((double)_color.red) / 255.0) * 4095));
	_tlc->set(_channels[1], 4095 - (int)((((double)_color.green) / 255.0) * 4095));
	_tlc->set(_channels[2], 4095 - (int)((((double)_color.blue) / 255.0) * 4095));
}

#else

OSC::Device::RGBColorScheduler::RGBColorScheduler() {
}

OSC::Device::RGBColorScheduler::RGBColorScheduler(uint8_t redChannel, uint8_t greenChannel, uint8_t blueChannel, uint8_t rainbowPos, Tlc5940 * tlc) {
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