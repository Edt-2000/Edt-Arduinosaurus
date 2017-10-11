#pragma once

#include <Arduino.h>
#include <OSCArduino.h>
#include <OSCMessageProducer.h>

class EdtDIPedal : public OSC::MessageProducer {
public:
	EdtDIPedal(int ground, int ring, int tip, const char * oscAddress) {
		_ring = ring;
		_tip = tip;
		_ground = ground;

		pinMode(ring, INPUT_PULLUP);
		pinMode(tip, INPUT_PULLUP);
		pinMode(ground, OUTPUT);

		digitalWrite(ground, LOW);

		_message.setAddress(oscAddress);
		_message.reserve(1);
	}

	void loop() {
		if (millis() > _disableUntil) {
			_value = 0;

			if(digitalRead(_tip) == LOW) {
				_value += 1;
			} 
			if(digitalRead(_ring) == LOW) {
				_value += 2;
			}

			if (_value > 0 && _previousValue == _value) {
				_disableUntil = millis() + 300UL;
			}

			_previousValue = _value;
		}
	}

	OSC::Message * generateMessage() {
		if (_value > 0) {
			_message.setValidData(true);
			_message.setInt(0, _value);

			_value = 0;
			_previousValue = 0;
		}
		else {
			_message.setValidData(false);
		}
		return &_message;
	}
private:
	OSC::Message _message = OSC::Message();
	int _ring;
	int _tip;
	int _ground;

	int _value = 0;
	int _previousValue = 0;
	unsigned long _disableUntil = 0;
};