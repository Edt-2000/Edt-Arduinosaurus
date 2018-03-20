#pragma once

#include <OSCArduino.h>
#include <OSCMessageProducer.h>

union EdtTrakData
{
	int buffer[6];
	struct {
		int leftX;
		int leftY;
		int leftZ;
		int rightX;
		int rightY;
		int rightZ;
	};

	EdtTrakData() {
		leftX = 0;
		leftY = 0;
		leftZ = 0;
		rightX = 0;
		rightY = 0;
		rightZ = 0;
	};

	bool lowLeft() {
		return (leftZ < 5);
	};

	bool lowRight() {
		return (rightZ < 5);
	};
};

union EdtAITrakConfig
{
	int buffer[6];
	struct {
		int leftX;
		int leftY;
		int leftZ;
		int rightX;
		int rightY;
		int rightZ;
	};
};

class EdtAITrak : public OSC::MessageProducer {
public:
	EdtTrakData data = EdtTrakData();

	EdtAITrak(int leftX, int leftY, int leftZ, int rightX, int rightY, int rightZ, const char * oscAddress) {
		_config.leftX = leftX;
		_config.leftY = leftY;
		_config.leftZ = leftZ;
		_config.rightX = rightX;
		_config.rightY = rightY;
		_config.rightZ = rightZ;

		_message.setAddress(oscAddress);
		_message.reserve(6);
	};

	void loop() {}

	OSC::Message * generateMessage() {
		for (int i = 0; i < 6; i++) {
			data.buffer[i] = analogRead(_config.buffer[i]) / 8;
			_message.setInt(i, data.buffer[i]);
		}

		_currentEmpty = data.lowLeft() && data.lowRight();

		if (data.lowLeft()) {
			_message.setInt(0, 0);
			_message.setInt(1, 0);
			_message.setInt(2, 0);
		}
		if(data.lowRight()) {
			_message.setInt(3, 0);
			_message.setInt(4, 0);
			_message.setInt(5, 0);
		}

		_message.setValidData(!(_currentEmpty && _previousEmpty));

		_previousEmpty = _currentEmpty;

		return &_message;
	};
private:
	OSC::Message _message = OSC::Message();
	EdtAITrakConfig _config = EdtAITrakConfig();

	bool _previousEmpty = false;
	bool _currentEmpty = false;
};