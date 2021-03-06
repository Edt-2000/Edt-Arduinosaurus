#pragma once

#include "AbstractApplication.h"

#include "Definitions.h"

#include "Arduino.h"
#ifdef ETHERNET
#include "Ethernet.h"
#include "EthernetUDP.h"
#endif
#include "HardwareSerial.h"
#include "OSCArduino.h"
#include "Statemachine.h"
#include "Time.h"

#include "EdtDMX.h"
#include "DMXSlave.h"
#include "DMXSlaveConfig.h"
#include "DMXLedSpot.h"
#include "DMXThreeChannelLed.h"
#include "DMXShowTecCompactPar.h"
#include "DMXFixedSingleChannel.h"

#include "FadeMode.h"
#include "SparkFun_Tlc5940.h"

// TODO: 9 bit address support

class DMXApplication : public AbstractApplication
{
  public:
	OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>> osc;

#ifdef ETHERNET
	OSC::Device::EdtDMX dmx = OSC::Device::EdtDMX(OSC_DMX1);

	EthernetUDP udp;
#else
	OSC::Device::EdtDMX dmx = OSC::Device::EdtDMX(OSC_DMX2);
#endif

	void setupStatus()
	{
		status.setup(13, HIGH);
	}

	void setupNetwork()
	{
#ifndef ETHERNET

		Serial.begin(57600);

#else
		Ethernet.begin(MAC_DOSMCX, IP_DOSMCX);

 		udp.begin(PORT_BROADCAST);

 #endif
	}

	void setupOsc()
	{
		DMXSerial.init(DMXController);

		osc = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(0, 0);
 #ifndef ETHERNET
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