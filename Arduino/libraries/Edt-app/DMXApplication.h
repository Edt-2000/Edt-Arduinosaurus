#pragma once

//#define USB

#include "AbstractApplication.h"

#include "Definitions.h"

#include "Arduino.h"
#include "Ethernet.h"
#include "EthernetUDP.h"
#include "HardwareSerial.h"
#include "OSCArduino.h"
#include "Statemachine.h"
#include "Time.h"

#include "EdtDMX.h"
#include "DMXSlave.h"
#include "DMXLedSpot.h"

#include "FadeMode.h"
#include "SparkFun_Tlc5940.h"

auto slave = new DMXLedSpot();

class DMXApplication : public AbstractApplication
{
  public:
	OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>> osc;

	OSC::Device::EdtDMX dmx = 
		OSC::Device::EdtDMX(OSC_FL1, slave)
	;

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

		Serial.begin(57600);

#else

		Ethernet.begin(MAC_LED, IPAddress(192, 168, 1, 122));

		udp.begin(PORT_BROADCAST);

#endif
	}

	void setupOsc()
	{
		osc = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);
#ifdef USB
		osc.bindStream(&Serial);
#else
		osc.bindUDP(&udp, IP_BROADCAST, PORT_BROADCAST);
#endif
		osc.addConsumer(&dmx);

		// make a test blink
		dmx.test();
	}

	void applicationLoop()
	{
		osc.loop(time.tOSC);

		if (time.tVISUAL)
		{
			dmx.animationLoop();
		}
	}
};