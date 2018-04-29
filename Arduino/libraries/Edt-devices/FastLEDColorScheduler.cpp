#include "FastLEDColorScheduler.h"

#ifndef _MSC_VER

#include "FastLED.h"
#include "FadeMode.h"

OSC::Device::FastLEDColorScheduler::FastLEDColorScheduler()
{
}

OSC::Device::FastLEDColorScheduler::FastLEDColorScheduler(CRGB *leds, uint8_t nrOfLeds)
{
	_leds = leds;
	_ledState = new LedState[nrOfLeds];
	_nrOfLeds = nrOfLeds;

	for (int i = 0; i < nrOfLeds; i++)
	{
		_ledState[i].fade = 255;
	}
}

void OSC::Device::FastLEDColorScheduler::fade(uint8_t start, uint8_t end, uint8_t speed, FadeMode fadeMode)
{
	_start = normalizeLedNrDown(start);
	_end = normalizeLedNrUp(end);

	_fadeMode = fadeMode;

	for (int i = _start; i < _end; i++)
	{
		_ledState[i].fade = speed;
	}
}

void OSC::Device::FastLEDColorScheduler::disableFade(uint8_t start, uint8_t end)
{
	_start = normalizeLedNrDown(start);
	_end = normalizeLedNrUp(end);

	for (int i = _start; i < _end; i++)
	{
		_ledState[i].fade = 255;
	}
}

void OSC::Device::FastLEDColorScheduler::solid(uint8_t start, uint8_t end, uint8_t hue, uint8_t saturation, uint8_t value)
{
	solid(start, end, CHSV(hue, saturation, value));

	// _start = normalizeLedNrDown(start);
	// _end = normalizeLedNrUp(end);
	// _length = _end - _start;

	// fill_solid(_leds + _start, _length, CHSV(hue, saturation, value));
}

void OSC::Device::FastLEDColorScheduler::solid(uint8_t start, uint8_t end, CHSV color)
{
	_start = normalizeLedNrDown(start);
	_end = normalizeLedNrUp(end);
	_length = _end - _start;

	fill_solid(_leds + _start, _length, color);
}

void OSC::Device::FastLEDColorScheduler::rainbow(uint8_t start, uint8_t end, uint8_t hue, uint8_t deltaHue)
{
	_start = normalizeLedNrDown(start);
	_end = normalizeLedNrUp(end);
	_length = _end - _start;

	fill_rainbow(_leds + _start, _length, hue, (deltaHue / 127.0) * (255.0 / _length));
}

void OSC::Device::FastLEDColorScheduler::rainbow(uint8_t start, uint8_t center, uint8_t end, uint8_t hue, uint8_t deltaHue, uint8_t intensity)
{
	_start = normalizeLedNrDown(start);
	_center = normalizeLedNrDown(center);
	_end = normalizeLedNrUp(end);

	if (_start != _center)
	{
		int leds = (_center - _start) * (intensity / 255.0);

		for (int i = _start; i < _center - leds; i++)
		{
			_ledState[i].fade = 127;
		}
		for (int i = _center - leds; i < _center; i++)
		{
			_ledState[i].fade = 255;
		}

		fill_rainbow_reverse(&_leds[_center - leds - 1], leds, hue, 255 - (deltaHue / (_center - _start)));
	}
	if (_center != _end)
	{
		int leds = (_end - _center) * (intensity / 255.0);

		for (int i = _center + leds; i < _end; i++)
		{
			_ledState[i].fade = 127;
		}
		for (int i = _center; i < _center + leds; i++)
		{
			_ledState[i].fade = 255;
		}

		fill_rainbow(&_leds[_center], leds, hue, 255 - (deltaHue / (_end - _center)));
	}
}

void OSC::Device::FastLEDColorScheduler::twinkle(uint8_t start, uint8_t end, uint8_t hue, uint8_t saturation, uint8_t value, uint8_t intensity, bool blackOut)
{
	_start = normalizeLedNrDown(start);
	_end = normalizeLedNrDown(end);

	for (int i = _start; i < _end; i++)
	{
		if (intensity > random8())
		{
			_leds[i] = CHSV(hue, saturation, value);
		}
		else if (blackOut)
		{
			_leds[i] = CHSV(0, 0, 0);
		}
	}
}

void OSC::Device::FastLEDColorScheduler::chase(uint8_t hue, uint8_t speed)
{
	if (speed == 0)
	{
		_animations.resetAnimations();
	}
	else
	{
		_animations.addAnimation(Animation(AnimationType::Chase, CHSV(hue, 255, 255), speed, 0));
	}
}

void OSC::Device::FastLEDColorScheduler::strobo(uint8_t hue, uint8_t fps)
{
	disableFade(0, 127);

	fill_solid(_leds, _nrOfLeds, CRGB::HTMLColorCode::Black);

	if (fps == 0)
	{
		_animations.resetAnimations();
	}
	else
	{
		if (hue < 255)
		{
			_animations.insertAnimation(Animation(AnimationType::Strobo, CHSV(hue, 255, 255), 255.0 / fps, 0));
		}
		else
		{
			_animations.insertAnimation(Animation(AnimationType::Strobo, CHSV(0, 0, 255), 255.0 / fps, 0));
		}
	}
}

void OSC::Device::FastLEDColorScheduler::loop()
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
			return;
		case AnimationType::Chase:

			if (_animations.animations[i].state > 255 - _animations.animations[i].data)
			{
				_animations.removeAnimation(i);

				// cycle to next animation type without incrementing i, since _animationsActive -= 1
				continue;
			}

			_animations.animations[i].state += _animations.animations[i].data;

			uint8_t from = _animations.animations[i].state / 2;
			uint8_t to = (_animations.animations[i].state / 2) + 1;
			if (to > 127)
			{
				to = 127;
			}

			solid(from, to, _animations.animations[i].color);
			fade(from, to, 63, _fadeMode);

			break;
		}

		i++;
	}

	switch (_fadeMode)
	{
	case FadeMode::FadeToBlack:
		for (int i = 0; i < _nrOfLeds; i++)
		{
			if (_ledState[i].fade < 255)
			{
				if (_ledState[i].fade > 255 - 62)
				{
					_ledState[i].fade = 255;
				}
				else
				{
					_ledState[i].fade += ((_ledState[i].fade) / 4) + 1;
				}

				fadeToBlackBy(_leds + i, 1, _ledState[i].fade);
			}
		}
		break;
	case FadeMode::FadeOneByOne:
		for (int i = 0; i < _nrOfLeds; i++)
		{
			if (_ledState[i].fade < 255)
			{
				if (_ledState[i].fade > random8())
				{
					_ledState[i].fade = 255;

					fadeToBlackBy(_leds + i, 1, 255);
				}
				else
				{
					fadeToBlackBy(_leds + i, 1, 8);
				}
			}
		}
		break;
	}
}

#else

OSC::Device::FastLEDColorScheduler::FastLEDColorScheduler()
{
}

OSC::Device::FastLEDColorScheduler::FastLEDColorScheduler(CRGB *leds, uint8_t nrOfLeds)
{
}

void OSC::Device::FastLEDColorScheduler::solid(uint8_t start, uint8_t end, uint8_t h, uint8_t s, uint8_t v)
{
	commandColor = Command();
	commandColor.type = Type::solid;
	commandColor.start = start;
	commandColor.end = end;
	commandColor.h = h;
	commandColor.s = s;
	commandColor.v = v;
}

void OSC::Device::FastLEDColorScheduler::fade(uint8_t start, uint8_t end, uint8_t duration, FadeMode mode)
{
	commandFade = Command();
	commandFade.type = Type::fade;
	commandFade.start = start;
	commandFade.end = end;
	commandFade.duration = duration;
	commandFade.mode = mode;
}

void OSC::Device::FastLEDColorScheduler::disableFade(uint8_t start, uint8_t end)
{
	commandFade = Command();
	commandFade.type = Type::disableFade;
	commandFade.start = start;
	commandFade.end = end;
}

void OSC::Device::FastLEDColorScheduler::rainbow(uint8_t start, uint8_t end, uint8_t h, uint8_t dh)
{
	commandColor = Command();
	commandColor.type = Type::rainbow;
	commandColor.start = start;
	commandColor.end = end;
	commandColor.h = h;
	commandColor.dh = dh;
}

void OSC::Device::FastLEDColorScheduler::rainbow(uint8_t start, uint8_t center, uint8_t end, uint8_t h, uint8_t dh, uint8_t intensity)
{
	commandColor = Command();
	commandColor.type = Type::rainbow;
	commandColor.start = start;
	commandColor.center = center;
	commandColor.end = end;
	commandColor.h = h;
	commandColor.dh = dh;
	commandColor.intensity = intensity;
}

void OSC::Device::FastLEDColorScheduler::twinkle(uint8_t start, uint8_t end, uint8_t h, uint8_t s, uint8_t v, uint8_t intensity, bool blackOut)
{
	commandTwinkle = Command();
	commandTwinkle.type = Type::twinkle;
	commandTwinkle.start = start;
	commandTwinkle.end = end;
	commandTwinkle.h = h;
	commandTwinkle.s = s;
	commandTwinkle.v = v;
	commandTwinkle.intensity = intensity;
	commandTwinkle.blackOut = blackOut;
}

void OSC::Device::FastLEDColorScheduler::strobo(uint8_t h, uint8_t intensity)
{
	commandColor = Command();
	commandColor.type = Type::strobo;
	commandColor.h = h;
	commandColor.intensity = intensity;
}

void OSC::Device::FastLEDColorScheduler::chase(uint8_t h, uint8_t speed)
{
	commandColor = Command();
	commandColor.type = Type::chase;
	commandColor.h = h;
	commandColor.speed = speed;
}

void OSC::Device::FastLEDColorScheduler::loop()
{
}

Command OSC::Device::FastLEDColorScheduler::getCommandColor()
{
	return commandColor;
}

Command OSC::Device::FastLEDColorScheduler::getCommandTwinkle()
{
	return commandTwinkle;
}

Command OSC::Device::FastLEDColorScheduler::getCommandFade()
{
	return commandFade;
}

#endif