#pragma once

#include "FadeMode.h"

#ifdef _MSC_VER
#include "../Edt-devicesTest/stdafx.h"
#else

#endif

class DMXSlave {
protected:


public:
	void virtual initialize(int address) = 0;
	void virtual loop() = 0;

	void virtual solid(uint8_t h, uint8_t s, uint8_t v) = 0;
	void virtual solid(uint8_t h1, uint8_t h2, uint8_t s, uint8_t v, uint8_t percentage) = 0;
	void virtual intensity(uint8_t intensity) = 0;
	void virtual fade(uint8_t duration, FadeMode mode = FadeMode::FadeToBlack) = 0;
	void virtual disableFade() = 0;
	void virtual strobo(uint8_t h, uint8_t intesity) = 0;
};