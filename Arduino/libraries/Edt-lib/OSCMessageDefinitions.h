#pragma once

namespace OSC
{
enum ColorCommands : uint8_t
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
	Chase = 12,
	Bash = 13,

	DMXConfig = 254
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

enum ChaseStyle
{
	Default = 0,
	LongTail = 1
};
struct ChaseCommand
{
	// 0 - 255
	uint8_t hue : 8;
	uint8_t speed : 8;
	ChaseStyle style: 8;
};

struct BashCommand
{
	// 0 - 255
	uint8_t hue : 8;
	uint8_t intensity : 8;
};

struct StroboCommand
{
	// 0 - 255
	uint8_t hue : 8;
	uint8_t intensity : 8;
};

struct DMXConfigCommand 
{
	// 0 - 255
	uint8_t clear : 8;
	uint8_t reset : 8;
	uint8_t slaveType : 8;
	uint8_t slaveAddress : 8;
	uint8_t maximumBrightness: 8;
	uint8_t minimumBrightness: 8;
};

struct EdtMessage
{
	ColorCommands command : 8;
	union Commands {
		SingleColorCommand singleColor;
		DualColorCommand dualColor;
		RainbowCommand rainbow;
		VuMeterCommand vuMeter;
		TwinkleCommand twinkle;
		ChaseCommand chase;
		BashCommand bash;
		StroboCommand strobo;

		DMXConfigCommand dmxConfig;
	};

	Commands commands;
};
}