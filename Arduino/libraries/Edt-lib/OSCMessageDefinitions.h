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

union CommandBuffer {
	struct SingleColorCommand
	{
		// 0 - 127
		uint32_t start;
		uint32_t end;

		// 0 - 255
		uint32_t hue;
		uint32_t saturation;
		uint32_t value;
		uint32_t duration;
	} singleColor;

	struct DualColorCommand
	{
		// 0 - 127
		uint32_t start;
		uint32_t end;

		// 0 - 255
		uint32_t hue1;
		uint32_t hue2;
		uint32_t percentage;
		uint32_t duration;
	} dualColor;

	struct RainbowCommand
	{
		// 0 - 127
		uint32_t start;
		uint32_t end;

		// 0 - 255
		uint32_t hue;
		uint32_t deltaHue;
		uint32_t duration;
	} rainbow;

	struct VuMeterCommand
	{
		// 0 - 127
		uint32_t start;
		uint32_t end;
		uint32_t center;

		// 0 - 255
		uint32_t hue;
		uint32_t deltaHue;
		uint32_t intensity;
	} vuMeter;

	struct TwinkleCommand
	{
		// 0 - 127
		uint32_t start;
		uint32_t end;

		// 0 - 255
		uint32_t hue;
		uint32_t intensity;
	} twinkle;

	struct KittCommand
	{
		// 0 - 127
		uint32_t position;
		uint32_t length;

		// 0 - 255
		uint32_t hue;
	} kitt;

	struct StroboCommand
	{
		// 0 - 255
		uint32_t hue;
		uint32_t intensity;
	} strobo;
};
}

class EdtLEDMessageBuffer {
public:	
	static OSC::CommandBuffer buffer;
};

OSC::CommandBuffer EdtLEDMessageBuffer::buffer;