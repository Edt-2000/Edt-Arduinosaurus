// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

#include "../Edt-devices/FadeMode.h"

// TODO: reference additional headers your program requires here

typedef unsigned char uint8_t;
struct CRGB {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};
struct CHSV {
	CHSV(uint8_t h, uint8_t s, uint8_t v) : h(h), s(s), v(v) {

	}
	uint8_t h;
	uint8_t s;
	uint8_t v;
};
enum Type {
	none,
	solid,
	fade,
	disableFade,
	rainbow,
	twinkle,
	strobo,
	kitt,
	intensity
};
struct Command {
public:
	Type type = Type::none;

	uint8_t start = 0;
	uint8_t center = 0;
	uint8_t end = 0;
	uint8_t h = 0;
	uint8_t s = 0;
	uint8_t v = 0;
	uint8_t duration = 0;
	FadeMode mode = FadeMode::FadeToBlack;
	uint8_t dh = 0;
	uint8_t intensity = 0;
	bool blackOut = false;
	uint8_t position = 0;
	uint8_t length = 0;
	uint8_t percentage = 0;
};
class Tlc5940 {
public:

};
