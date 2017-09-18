#pragma once

#include "AbstractApplication.h"

#include "Definitions.h"

#include "Arduino.h"
#include "Ethernet.h"
#include "EthernetUdp.h"
#include "OSCArduino.h"
#include "Statemachine.h"
#include "Time.h"

unsigned long startTime;
unsigned long endTime;

class OSCProducerConsumer : public OSC::MessageConsumer, public OSC::MessageProducer
{
public:
	OSC::Message message = OSC::Message();

	OSCProducerConsumer() {
		message.empty();
		message.setAddress("/T");
		message.reserveAtLeast(20);
		
		for(int i = 0; i < 10; ++i) {
			message.add(i);
		}

		for(int f = 10; f < 20; ++f) {
			message.add((float)f);
		}

		message.setValidData(true);
	}

	void loop() {

	}

	OSC::Message * generateMessage() {
		return &message;
	}

	const char * pattern() {
		return message.address;
	}

	void callbackMessage(OSC::Message * msg) {
		for(int i = 0; i < 10; ++i) {
			message.set<int>(i, msg->getInt(i));
		}

		for(int f = 10; f < 20; ++f) {
			message.set<float>(f, msg->getInt(f));
		}

		message.setValidData(true);
	}
};

class PerformanceApplication : public AbstractApplication
{
public:
	OSCProducerConsumer prodCons = OSCProducerConsumer();
	
	EthernetUDP udp;

	void setupStatus() {
		status.setup(13, HIGH);
	}

	void setupNetwork() {
		Serial.begin(9600);

		Ethernet.begin(MAC_PING, IPAddress(10,0,0,2));

		udp.begin(PORT_BROADCAST);
	}

	void setupOsc() {
		osc = OSC::Arduino(1, 1);
		osc.bindUDP(&udp, IPAddress(10,0,0,1), 12345);
		osc.addConsumer(&prodCons);
		osc.addProducer(&prodCons);
	}

	void applicationLoop() {
		
	}
};