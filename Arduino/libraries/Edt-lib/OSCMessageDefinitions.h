#pragma once

namespace OSC {
    enum class ColorCommands {
        SingleSolid = 0,
		SinglePulse = 1,
		RainbowSolid = 2,
		RainbowPulse = 3,
		VUMeter = 4,
		Twinkle = 5,
		Strobo = 6
	};
	
	struct Color {
		int hue;
		int saturation;
		int value;
	};

	struct SingleColorCommand {
		int start;
		int end;

		Color color;

		int duration;

		inline int getRealStart(int nr) {
			return floorf((start / 127.0) * nr);
		}
		inline int getRealEnd(int nr) {
			return ceilf((end / 127.0) * nr);
		}
	};

	struct RainbowCommand {
		int start;
		int end;

		int hue;
		int deltaHue;

		int duration;
	};

// 	_start = floorf((msg->getInt(1) / 127.0) * _nrOfLeds);
		// 	_end = ceilf((msg->getInt(2) / 127.0) * _nrOfLeds);
			
		// 	_hue = msg->getInt(3);
		// 	_deltahue = round((msg->getInt(4) / 127.0) * (255.0 / _nrOfLeds));

		// 	if (_deltahue > 0) {
		// 		fill_rainbow(&_leds[_start], _end - _start, _hue, _deltahue);
		// 	}

		// 	if (_mode == RainbowPulse || _deltahue == 0) {
		// 		_duration = msg->getInt(5);

		// 		colorScheduler.blackout(_start, _end, _duration);
		// 	}
		// 	else {
		// 		colorScheduler.disableBlackout(_start, _end);
		// 	}
}