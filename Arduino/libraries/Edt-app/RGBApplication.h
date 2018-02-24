#pragma once

#define USB

#include "AbstractApplication.h"

#include "Definitions.h"

#include "Arduino.h"
#include "HardwareSerial.h"
#include "Ethernet.h"
#include "EthernetUdp.h"
#include "OSCArduino.h"
#include "Statemachine.h"
#include "Time.h"
#include "RGB.h"
#include "FadeMode.h"

class RGBApplication : public AbstractApplication
{
  public:
	EdtRGB rgb1 = EdtRGB(OSC_RGB1, 3, 5, 6);

#ifndef USB
	EthernetUDP udp;
#endif

	void setupStatus()
	{
		status.setup(13, HIGH);
	}

	void setupNetwork()
	{
#ifdef USB

		Serial.begin(9600);

		while(!Serial) {
			;
		}

		Serial.println("Hi");

#else

		Ethernet.begin(MAC_LED);

		udp.begin(PORT_BROADCAST);

#endif
	}

	void setupOsc()
	{
		osc = OSC::Arduino(3, 0);
#ifdef USB
		osc.bindStream(&Serial);
#else
		osc.bindUDP(&udp, IP_BROADCAST, PORT_BROADCAST);
#endif
		osc.addConsumer(&rgb1);
		
		// make a test blink
		rgb1.test();
	}

	void applicationLoop()
	{
		if (time.tVISUAL)
		{
			rgb1.animationLoop();
		}
	}
};