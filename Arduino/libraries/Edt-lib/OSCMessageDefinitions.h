#pragma once

namespace OSC
{
enum ColorCommands
{
	SingleSolid = 0,
	SinglePulse = 1,
	SingleSpark = 7,
	RainbowSolid = 2,
	RainbowPulse = 3,
	RainbowSpark = 8,
	VuMeter = 4,
	Twinkle = 5,
	Strobo = 6,
	DualSolid = 9,
	DualPulse = 10,
	DualSparkle = 11,
	Kitt = 12
};

struct SingleColorCommand
{
	// 0 - 127
	uint8_t start : 8;
	uint8_t end : 8;

	// 0 - 255
	uint8_t hue : 8;
	uint8_t saturation : 8;
	uint8_t value : 8;
	uint8_t duration : 8;
};

struct DualColorCommand
{
	// 0 - 127
	uint8_t start : 8;
	uint8_t end : 8;

	// 0 - 255
	uint8_t hue1 : 8;
	uint8_t hue2 : 8;
	uint8_t percentage : 8;
	uint8_t duration : 8;
};

struct RainbowCommand
{
	// 0 - 127
	uint8_t start : 8;
	uint8_t end : 8;

	// 0 - 255
	uint8_t hue : 8;
	uint8_t deltaHue : 8;
	uint8_t duration : 8;
};

struct VuMeterCommand
{
	// 0 - 127
	uint8_t start : 8;
	uint8_t end : 8;
	uint8_t center : 8;

	// 0 - 255
	uint8_t hue : 8;
	uint8_t deltaHue : 8;
	uint8_t intensity : 8;
};

struct TwinkleCommand
{
	// 0 - 127
	uint8_t start : 8;
	uint8_t end : 8;

	// 0 - 255
	uint8_t hue : 8;
	uint8_t intensity : 8;
};

struct KittCommand
{
	// 0 - 127
	uint8_t position : 8;
	uint8_t length : 8;

	// 0 - 255
	uint8_t hue : 8;
};
}

struct StroboCommand
{
	// 0 - 255
	uint8_t hue : 8;
	uint8_t intensity : 8;
};

OSC::CommandBuffer EdtLEDMessageBuffer::buffer;