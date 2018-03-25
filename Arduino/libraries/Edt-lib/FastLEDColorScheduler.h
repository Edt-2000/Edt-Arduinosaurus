// #include "FastLED.h"
// #include "FadeMode.h"

// class FastLEDColorScheduler
// {
//   private:
// 	struct Color
// 	{
// 		int h;
// 		int s;
// 		int l;

// 		CHSV chsv()
// 		{
// 			return CHSV(h, s, l);
// 		}
// 	};

// 	struct LedState
// 	{
// 	  public:
// 		uint8_t fade;
// 	};

// 	struct Strobo
// 	{
// 		bool active;
// 		int loop;
// 		float fpl;
// 		Color color;
// 	};

// 	CRGB *_leds;
// 	LedState *_ledState;
// 	FadeMode _fadeMode;

// 	int _start;
// 	int _center;
// 	int _end;
// 	int _length;
// 	int _nrOfLeds;

// 	int normalizeLedNrDown(int percentage)
// 	{
// 		return floorf((percentage / 127.0) * _nrOfLeds);
// 	}
// 	int normalizeLedNrUp(int percentage)
// 	{
// 		return ceilf((percentage / 127.0) * _nrOfLeds);
// 	}

// 	Strobo _strobo;

//   public:
// 	FastLEDColorScheduler()
// 	{
// 	}

// 	FastLEDColorScheduler(CRGB *leds, int nrOfLeds)
// 	{
// 		_leds = leds;
// 		_ledState = new LedState[nrOfLeds];
// 		_nrOfLeds = nrOfLeds;

// 		for (int i = 0; i < nrOfLeds; i++)
// 		{
// 			_ledState[i].fade = 255;
// 		}
// 	}

// 	void fade(int start, int end, int speed, FadeMode fadeMode = FadeMode::FadeToBlack)
// 	{
// 		_start = normalizeLedNrDown(start);
// 		_end = normalizeLedNrUp(end);

// 		_fadeMode = fadeMode;

// 		for (int i = _start; i < _end; i++)
// 		{
// 			_ledState[i].fade = speed;
// 		}
// 	}

// 	void disableFade(int start, int end)
// 	{
// 		_start = normalizeLedNrDown(start);
// 		_end = normalizeLedNrUp(end);

// 		for (int i = _start; i < _end; i++)
// 		{
// 			_ledState[i].fade = 255;
// 		}
// 	}

// 	void solid(int start, int end, int hue, int saturation, int value)
// 	{
// 		_start = normalizeLedNrDown(start);
// 		_end = normalizeLedNrUp(end);
// 		_length = _end - _start;

// 		fill_solid(_leds + _start, _length, CHSV(hue, saturation, value));
// 	}

// 	void rainbow(int start, int end, int hue, int deltaHue)
// 	{
// 		_start = normalizeLedNrDown(start);
// 		_end = normalizeLedNrUp(end);
// 		_length = _end - _start;

// 		fill_rainbow(_leds + _start, _length, hue, (deltaHue / 127.0) * (255.0 / _length));
// 	}

// 	void rainbow(int start, int center, int end, int hue, int deltaHue, int intensity)
// 	{
// 		_start = normalizeLedNrDown(start);
// 		_center = normalizeLedNrDown(center);
// 		_end = normalizeLedNrUp(end);

// 		if (_start != _center)
// 		{

// 			int leds = (_center - _start) * (intensity / 255.0);

// 			for (int i = _start; i < _center - leds; i++)
// 			{
// 				_ledState[i].fade = 127;
// 			}
// 			for (int i = _center - leds; i < _center; i++)
// 			{
// 				_ledState[i].fade = 255;
// 			}

// 			fill_rainbow_reverse(&_leds[_center - leds - 1], leds, hue, deltaHue / (_center - _start));
// 		}
// 		if (_center != _end)
// 		{

// 			int leds = (_end - _center) * (intensity / 255.0);

// 			for (int i = _center + leds; i < _end; i++)
// 			{
// 				_ledState[i].fade = 127;
// 			}
// 			for (int i = _center; i < _center + leds; i++)
// 			{
// 				_ledState[i].fade = 255;
// 			}

// 			fill_rainbow(&_leds[_center], leds, hue, deltaHue / (_end - _center));
// 		}
// 	}

// 	void twinkle(int start, int end, int hue, int saturation, int value, int intensity, bool blackOut = true)
// 	{
// 		_start = normalizeLedNrDown(start);
// 		_end = normalizeLedNrDown(end);

// 		for (int i = _start; i < _end; i++)
// 		{
// 			if (intensity > random8())
// 			{
// 				_leds[i] = CHSV(hue, saturation, value);
// 			}
// 			else if (blackOut)
// 			{
// 				_leds[i] = CHSV(0, 0, 0);
// 			}
// 		}
// 	}

// 	void kitt(int position, int length, int hue)
// 	{
// 		_start = normalizeLedNrDown(min(position, 127 - length));
// 		_length = normalizeLedNrDown(length);

// 		for (int i = 0; i < _start; i++)
// 		{
// 			_ledState[i].fade = 63;
// 		}
// 		fill_solid(_leds + _start, _length, CHSV(hue, 240, 255));
// 		for (int i = _start + _length; i < _nrOfLeds; i++)
// 		{
// 			_ledState[i].fade = 63;
// 		}
// 	}

// 	void strobo(int hue, int fps)
// 	{
// 		disableFade(0, _nrOfLeds);

// 		fill_solid(_leds, _nrOfLeds, 0);

// 		_strobo.active = fps > 0;
// 		_strobo.loop = 0;
// 		_strobo.fpl = (255.0 / fps);
// 		if (hue == 0)
// 		{
// 			_strobo.color.h = 0;
// 			_strobo.color.s = 0;
// 			_strobo.color.l = 255;
// 		}
// 		else
// 		{
// 			_strobo.color.h = hue;
// 			_strobo.color.s = 255;
// 			_strobo.color.l = 255;
// 		}
// 	}

// 	void loop()
// 	{
// 		if (_strobo.active)
// 		{

// 			fill_solid(_leds, _nrOfLeds, 0);

// 			if ((_strobo.loop++) > _strobo.fpl)
// 			{
// 				_strobo.loop = 0;

// 				fill_solid(_leds, _nrOfLeds, _strobo.color.chsv());
// 			}
// 		}
// 		else
// 		{
// 			switch (_fadeMode)
// 			{
// 			case FadeMode::FadeToBlack:
// 				for (int i = 0; i < _nrOfLeds; i++)
// 				{
// 					if (_ledState[i].fade < 255)
// 					{
// 						if (_ledState[i].fade > 255 - 62)
// 						{
// 							_ledState[i].fade = 255;
// 						}
// 						else
// 						{
// 							_ledState[i].fade += ((_ledState[i].fade) / 4) + 1;
// 						}
// 						//long add = ((_ledState[i].fade) / 4) + 1;
// 						//if((long)_ledState[i].fade + add > 255) {
// 						//	_ledState[i].fade = 255;
// 						//}
// 						//else {
// 						//	_ledState[i].fade += add;
// 						//}

// 						fadeToBlackBy(_leds + i, 1, _ledState[i].fade);
// 					}
// 				}
// 				break;
// 			case FadeMode::FadeOneByOne:
// 				for (int i = 0; i < _nrOfLeds; i++)
// 				{
// 					if (_ledState[i].fade < 255)
// 					{
// 						if (_ledState[i].fade > random8())
// 						{
// 							_ledState[i].fade = 255;

// 							fadeToBlackBy(_leds + i, 1, 255);
// 						}
// 						else
// 						{
// 							fadeToBlackBy(_leds + i, 1, 8);
// 						}
// 					}
// 				}
// 				break;
// 			}
// 		}
// 	}
// };
