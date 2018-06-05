#pragma once

#include "AnimationType.h"

#ifdef _MSC_VER
#include "../Edt-devicesTest/stdafx.h"
#else
#include "FastLED.h"
#endif


struct Animation {
public:
	Animation() : color(CHSV(0,0,0)) {};
	Animation(AnimationType type, CHSV color, uint8_t data, uint8_t state) : type(type), color(color), data(data), state(state) {};

	AnimationType type;
	CHSV color;
	uint8_t data;
	uint8_t state;
};