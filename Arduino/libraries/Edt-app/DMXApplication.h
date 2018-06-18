#pragma once

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
#include "DMXSlaveConfig.h"

#include "FadeMode.h"
#include "SparkFun_Tlc5940.h"

class DMXApplication : public AbstractApplication
{
  public:
	OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>> osc;

#ifdef ETHERNET
	OSC::Device::EdtDMX dmx = OSC::Device::EdtDMX(OSC_FL1);
#endif
#ifdef USB
	OSC::Device::EdtDMX dmx = OSC::Device::EdtDMX(OSC_FL2);
#endif

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
		Ethernet.begin(MAC_DOSMCX, IP_DOSMCX);

		udp.begin(PORT_BROADCAST);

#endif
	}

	void setupOsc()
	{
		DMXSerial.init(DMXController);

		osc = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);
#ifdef USB
		osc.bindStream(&Serial);
#else
		osc.bindUDP(&udp, IP_BROADCAST, PORT_BROADCAST);
#endif
		osc.addConsumer(&dmx);

		// make a test blink
		dmx.initialize();
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