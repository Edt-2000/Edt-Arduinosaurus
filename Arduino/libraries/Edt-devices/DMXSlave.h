#pragma once

#include <EEPROM.h>
#include "FadeMode.h"
#include "DMXSlaveConfig.h"

#define COUNTLOCATION 0

#ifdef _MSC_VER
#include "../Edt-devicesTest/stdafx.h"
#else

#endif

namespace OSC
{
namespace DMX
{
class Slave
{
  protected:
	  int _address;
	  int _minimumBrightness;
	  int _maximumBrightness;

	  inline uint8_t clampValue(uint8_t value) {
		  return _minimumBrightness + ((value / 255.0) * ((double)(_maximumBrightness - _minimumBrightness)));
	  }

  public:
	void virtual initialize(int address, int maximumBrightness, int minimumBrightness) = 0;
	void virtual loop() = 0;

	void virtual solid(uint8_t h, uint8_t s, uint8_t v) = 0;
	void virtual solid(uint8_t h1, uint8_t h2, uint8_t s, uint8_t v, uint8_t percentage) = 0;
	void virtual intensity(uint8_t intensity) = 0;
	void virtual fade(uint8_t duration, FadeMode mode = FadeMode::FadeToBlack) = 0;
	void virtual disableFade() = 0;
	void virtual strobo(uint8_t h, uint8_t intesity) = 0;

	static void clearSlaveConfig() {
		setSlaveCount(COUNTLOCATION);
	}

	static uint8_t getSlaveCount()
	{
		uint8_t count;

		EEPROM.get(COUNTLOCATION, count);

		return count;
	}

	static void setSlaveCount(uint8_t count)
	{
		EEPROM.update(COUNTLOCATION, count);
	}

	static SlaveConfig getSlaveConfig(uint8_t nr)
	{
		SlaveConfig config;

		EEPROM.get(1 + (sizeof(SlaveConfig) * nr), config);

		return config;
	}

	static void setSlaveConfig(SlaveConfig config)
	{
		uint8_t nr = getSlaveCount();
		
		EEPROM.put(1 + (sizeof(SlaveConfig) * nr), config);

		setSlaveCount(nr + 1);
	}
};
} // namespace DMX
} // namespace OSC