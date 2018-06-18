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
enum SlaveType : uint8_t
{
	Unknown = 0,
	DefaultLedSpot = 1
};

struct SlaveConfig
{
  public:
	SlaveType type;
	uint8_t address;
};
} // namespace DMX
} // namespace OSC