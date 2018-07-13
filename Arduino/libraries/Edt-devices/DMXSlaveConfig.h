#pragma once

#include "FadeMode.h"
#include "DMXSlave.h"

#ifdef _MSC_VER
#include "../Edt-devicesTest/stdafx.h"
#else

#endif

namespace OSC
{
namespace DMX
{
enum class SlaveType : uint8_t
{
	Unknown = 0,
	LedSpot = 1,
	ThreeChannelLed = 2,
	ShowTecCompactPar = 3,
	FixedSingleChannel = 4
};

struct SlaveConfig
{
  public:
	SlaveType type;
	uint16_t address;
	uint8_t maximumBrightness;
	uint8_t minimumBrightness;
};
} // namespace DMX
} // namespace OSC