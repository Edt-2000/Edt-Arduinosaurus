#pragma once

#include "AbstractApplication.h"

#include "Definitions.h"

#include "ESP8266WiFi.h"
#include "Arduino.h"
#include "WiFiUdp.h"

#include "LightPWM.h"
#include "Guitar.h"

#include "WifiConfig.h" // defines WifiName and WifiPassword

class GuitarApplication : public AbstractApplication
{
public:
	//EdtLightPWM PWM = EdtLightPWM();
	EdtGuitar guitar = EdtGuitar(OSC_GUITAR);

	WiFiUDP udp;

	GuitarApplication() {}

	void setupStatus() {
		status.setup(5, LOW);

		//PWM.start(5);
	}

	void setupNetwork() {
		WiFi.disconnect();
		WiFi.mode(WIFI_STA);
		WiFi.begin(WifiName, WifiPassword);

		while(!(WiFi.status() == WL_CONNECTED))
		{
			delay(1);
		}

		udp.begin(PORT_BROADCAST);
	}

	void setupOsc() {
		osc = OSC::Arduino(0, 1);
		osc.bindUDP(&udp, IP_BROADCAST, PORT_BROADCAST);
		osc.addProducer(&guitar);
	}

	void applicationLoop() {
		//PWM.loop();

		
		
		yield();
	}
};