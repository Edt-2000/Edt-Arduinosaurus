#pragma once

#include "AnimationType.h"
#include "Animation.h"

class Animations {
private:
	uint8_t _maxAnimations = 16;
public:
	Animation animations[16];
	uint8_t animationsActive = 0;

	void addAnimation(Animation animation) {
		if (animationsActive >= _maxAnimations) {
			return;
		}

		animations[animationsActive++] = animation;
	}

	void insertAnimation(Animation animation) {
		animations[0] = animation;
		animationsActive = 1;
	}

	void removeAnimation(uint8_t animationNr) {
		if (animationsActive >= 2 && animationNr < animationsActive - 1) {
			animations[animationNr] = animations[--animationsActive];
		}
		else {
			--animationsActive;
		}
	}

	void resetAnimations() {
		animationsActive = 0;
	}
};