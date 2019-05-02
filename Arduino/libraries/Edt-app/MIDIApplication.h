#pragma once

#include "AbstractApplication.h"

#include "Definitions.h"

#include "Arduino.h"

#include "MIDI.h"

class MIDIApplication : public AbstractApplication
{
  public:
	OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>> osc;

	OSC::Device::EdtDMX dmx = OSC::Device::EdtDMX(OSC_MIDI1);

	EthernetUDP udp;

	void setupStatus()
	{
		status.setup(13, HIGH);
	}

	void setupNetwork()
	{
		Ethernet.begin(MAC_DIMOSCI, IP_DIMOSCI);

 		udp.begin(PORT_BROADCAST);
	}

	void setupOsc()
	{
		DMXSerial.init(DMXController);

		osc = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(0, 0);
 		osc.bindUDP(&udp, IP_BROADCAST, PORT_BROADCAST);
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