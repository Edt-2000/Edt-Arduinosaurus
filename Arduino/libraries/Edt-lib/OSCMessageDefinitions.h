#pragma once

namespace OSC {
    enum ColorCommands {
        SingleSolid = 0,
		SinglePulse = 1,
		RainbowSolid = 2,
		RainbowPulse = 3,
		VuMeter = 4,
		Twinkle = 5,
		Strobo = 6
	};
	
	struct SingleColorCommand {
		// 0 - 127
		int start;
		int end;

		// 0 - 255
		int hue;
		int saturation;
		int value;
		int duration;
	};

	struct RainbowCommand {
		// 0 - 127
		int start;
		int end;

		// 0 - 255
		int hue;
		int deltaHue;
		int duration;
	};

	struct VuMeterCommand {
		// 0 - 127
		int start;
		int end;
		int center;

		// 0 - 255
		int hue;
		int deltaHue;
		int intensity;
	};

	struct TwinkleCommand {
		// 0 - 127
		int start;
		int end;

		// 0 - 255
		int hue;
		int intensity;
	};

	struct StroboCommand {
		// 0 - 255
		int hue;
		int intensity;
	};
}