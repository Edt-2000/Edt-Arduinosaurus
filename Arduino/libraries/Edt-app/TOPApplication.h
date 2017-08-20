#pragma once

//#include <functional>

#include "AbstractApplication.h"

#include "Definitions.h"

#include "Arduino.h"
#include "Ethernet.h"
#include "EthernetUdp.h"
#include "OSCArduino.h"
#include "Statemachine.h"
#include "Time.h"
#include "Top.h"

#include "FastLED.h"

class TOPApplication : public AbstractApplication
{
public:
	EdtTOP top1 = EdtTOP(OSC_TOP1, 29);
	EdtTOP top2 = EdtTOP(OSC_TOP2, 59);
	EthernetUDP udp;
	int dot = 0;

	void setupStatus() {
		status.setup(13, HIGH);
	}

	void setupNetwork() {
		Ethernet.begin(MAC_PING);

		udp.begin(PORT_BROADCAST);
	}

	void setupOsc() {
		top1.configurePins<2,3>();
		top2.configurePins<4,3>();

		osc = OSC::Arduino(1, 0);
		osc.bindUDP(&udp, IP_BROADCAST, PORT_BROADCAST);
		osc.addConsumer(&top1);
		osc.addConsumer(&top2);
	}

	void applicationLoop() {
		if (time.tVISUAL) {
			FastLED.show();
			
			top1.animationLoop();
			top2.animationLoop();
		}
	}
};