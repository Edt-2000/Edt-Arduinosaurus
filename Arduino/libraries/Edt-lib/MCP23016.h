#pragma once

/*
IO Driver for MCP23016 
16 channel DIO converter

TODO:
	support Interrupt output
	support IO speed settings
	support multiple addresses
	support INPUT_PULLUP
	remove whiles
	improve reading of data, the datasheet indicates that it can be faster.

*/
#include <Arduino.h>
#include <Wire.h>

#define GP0 0x00
#define GP1 0x01
#define LAT0 0x02
#define LAT1 0x03
#define POL0 0x04
#define POL1 0x05
#define DIR0 0x06
#define DIR1 0x07
#define INT0 0x08
#define INT1 0x09
#define IOCON 0x0A

union MCP23016_dataWord {
	unsigned int data;

	struct b_t
	{
		unsigned char lo;
		unsigned char hi;
	} bytes;

	MCP23016_dataWord()
	{
		data = 0;
	}
};

class MCP23016
{
  private:
	unsigned char _address;

	MCP23016_dataWord _read;
	MCP23016_dataWord _write;
	MCP23016_dataWord _modes;
	MCP23016_dataWord _inputPolarity;

	union {
		unsigned char all;
		struct
		{
			// dimensions must be included, otherwise each bool will occupy a char and the union will be 6 chars wide. //
			bool modesLo : 1;
			bool modesHi : 1;
			bool inputPolarityLo : 1;
			bool inputPolarityHi : 1;
			bool writesLo : 1;
			bool writesHi : 1;
		};
	} _send;

  public:
	MCP23016(int address)
	{
		_address = 0x20 | ((unsigned char)address & 0xff);

		_modes.data = 0xFFFF;
		_inputPolarity.data = 0x0000;

		_send.modesLo = true;
		_send.modesHi = true;

		_send.inputPolarityLo = true;
		_send.inputPolarityHi = true;
	}

	void begin()
	{
		// cycle at the beginning as the configuration is required to be send to the chip //
		// otherwise the program starts at the first loop writing to input addresses, or vice versa //
		cycle();
	}
	void cycle()
	{
		while (_send.all != 0)
		{
			Wire.beginTransmission(_address);

			if (_send.modesLo)
			{
				Wire.write(DIR0);
				Wire.write(_modes.bytes.lo);

				_send.modesLo = false;
			}
			else if (_send.modesHi)
			{
				Wire.write(DIR1);
				Wire.write(_modes.bytes.hi);

				_send.modesHi = false;
			}
			else if (_send.inputPolarityLo)
			{
				Wire.write(POL0);
				Wire.write(_inputPolarity.bytes.lo);

				_send.inputPolarityLo = false;
			}
			else if (_send.inputPolarityHi)
			{
				Wire.write(POL1);
				Wire.write(_inputPolarity.bytes.hi);

				_send.inputPolarityHi = false;
			}
			else if (_send.writesLo)
			{
				Wire.write(GP0);
				Wire.write(_write.bytes.lo);

				_send.writesLo = false;
			}
			else if (_send.writesHi)
			{
				Wire.write(GP1);
				Wire.write(_write.bytes.hi);

				_send.writesHi = false;
			}
			Wire.endTransmission();
		}

		yield();

		Wire.beginTransmission(_address);
		Wire.write(GP0);
		Wire.endTransmission();

		yield();

		Wire.requestFrom(_address, (unsigned char)0x01);

		while (Wire.available() < 1)
			;

		if (Wire.available() >= 1)
		{
			_read.bytes.lo = Wire.read();
		}

		yield();

		Wire.beginTransmission(_address);
		Wire.write(GP1);
		Wire.endTransmission();

		yield();

		Wire.requestFrom(_address, (unsigned char)0x01);

		while (Wire.available() < 1)
			;

		if (Wire.available() >= 1)
		{
			_read.bytes.hi = Wire.read();
		}

		yield();
	}
	void mode(int address, int mode)
	{
		if (mode == INPUT)
		{
			_modes.data |= (0x0001 << address);
			_inputPolarity.data &= ~(0x0001 << address);
		}
		else if (mode == OUTPUT)
		{
			_modes.data &= ~(0x0001 << address);
		}
		else if (mode == INPUT_PULLUP)
		{
			_modes.data |= (0x0001 << address);
			_inputPolarity.data |= (0x0001 << address);
		}

		if (address > 7)
		{
			_send.modesHi = true;
			_send.inputPolarityHi = true;
		}
		else
		{
			_send.modesLo = true;
			_send.inputPolarityLo = true;
		}
	}

	bool digitalRead(int address)
	{
		return (_read.data & (0x0001 << address));
	}
	void digitalWrite(int address, bool data)
	{
		if (!data)
		{
			_write.data |= (0x0001 << address);
		}
		else
		{
			_write.data &= ~(0x0001 << address);
		}

		if (address > 7)
		{
			_send.writesHi = true;
		}
		else
		{
			_send.writesLo = true;
		}
	}
};