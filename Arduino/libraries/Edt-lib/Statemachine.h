#pragma once

enum State {
	begin = 1,
	starting = 2,
	run = 4
};

class EdtStatemachine {
public:
	EdtStatemachine() : _state(State::begin) {};

	inline State current() {
		return _state;
	}

	inline void begin() {
		_state = State::begin;
	}

	inline void step() {
		State newState;

		switch (_state) {
		case State::begin:
			Serial.println("Starting");
			newState = State::starting;
			break;
		case State::starting:
			Serial.println("Run");
			newState = State::run;
			break;
		default:
			Serial.println("Begin");
			newState = State::begin;
			break;
		}

		_state = newState;
	};
private:
	State _state;
};