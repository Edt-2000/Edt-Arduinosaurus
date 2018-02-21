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
	uint8_t start;
	uint8_t end;

	// 0 - 255
	uint8_t hue;
	uint8_t saturation;
	uint8_t value;
	uint8_t duration;
};

struct DualColorCommand
{
	// 0 - 127
	uint8_t start;
	uint8_t end;

	// 0 - 255
	uint8_t hue1;
	uint8_t hue2;
	uint8_t percentage;
	uint8_t duration;
};

struct RainbowCommand
{
	// 0 - 127
	uint8_t start;
	uint8_t end;

	// 0 - 255
	uint8_t hue;
	uint8_t deltaHue;
	uint8_t duration;
};

struct VuMeterCommand
{
	// 0 - 127
	uint8_t start;
	uint8_t end;
	uint8_t center;

	// 0 - 255
	uint8_t hue;
	uint8_t deltaHue;
	uint8_t intensity;
};

struct TwinkleCommand
{
	// 0 - 127
	uint8_t start;
	uint8_t end;

	// 0 - 255
	uint8_t hue;
	uint8_t intensity;
};

struct KittCommand
{
	// 0 - 127
	uint8_t position;
	uint8_t length;

	// 0 - 255
	uint8_t hue;
};

struct StroboCommand
{
	// 0 - 255
	uint8_t hue;
	uint8_t intensity;
};
}