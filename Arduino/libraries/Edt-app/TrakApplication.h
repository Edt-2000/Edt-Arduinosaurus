#pragma once

#include "AbstractApplication.h"

#include "Definitions.h"

#include "Arduino.h"
#include "Ethernet.h"
#include "EthernetUdp.h"
#include "OSCArduino.h"
#include "Statemachine.h"
#include "Time.h"
#include "Trak.h"
#include "Pedal.h"

class TrakApplication : public AbstractApplication
{
public:
	EdtAITrak Trak = EdtAITrak(2, 1, 0, 5, 4, 3, OSC_TRAK);
	EdtDIPedal Pedal = EdtDIPedal(5, 6, 7, OSC_PEDAL);

	EthernetUDP udp;
	
	void setupStatus() {
		status.setup(13, HIGH);
	}

	void setupNetwork() {
		Ethernet.begin(MAC_TRAK);

		udp.begin(PORT_BROADCAST);
	}

	void setupOsc() {
		osc = OSC::Arduino(0, 2);
		// trak / pedal only submits to sledt
		osc.bindUDP(&udp, IP_BROADCAST, PORT_BROADCAST);
		//osc.bindUDP(&udp, IP_SLEDT, PORT_BROADCAST);
		osc.addProducer(&Trak);
		osc.addProducer(&Pedal);
	}

	void applicationLoop() {
	}
};