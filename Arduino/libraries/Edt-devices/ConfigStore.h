#pragma once

#ifdef _MSC_VER
#include "../Edt-devicesTest/stdafx.h"
class EepromDefinition {
	int length() { return 0; }
	void put() { return 0; }
	int length() { return 0; }
} EEPROM;
#else
#include <EEPROM.h>
#endif;

struct Config {
	uint8_t type;
	uint8_t data;
};

class ConfigStore {
private:


public:
	template <typename T>
	T get(uint8_t address) {

	};

	uint8_t length() {
		return EEPROM.length();
	}


};