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

struct MessageData { 
	uint32_t int0; uint32_t int1; uint32_t int2; uint32_t int3; uint32_t int4; uint32_t int5; uint32_t int6; uint32_t int7; uint32_t int8; uint32_t int9;
	float float10; float float11; float float12; float float13; float float14; float float15; float float16; float float17; float float18; float float19;
};

struct Simple {
	uint32_t int0;
};

enum DummyEnum {
	def = 0
};

class OSCProducerConsumer : public OSC::StructMessageConsumer<DummyEnum>, public OSC::MessageProducer
{
public:
	MessageData messageData = MessageData();
	Simple simple = Simple();

	OSC::Message message = OSC::Message();

	OSCProducerConsumer() {
		message.empty();
		message.setAddress("/T");

		messageData.int0 = 0;
		messageData.int1 = 1;
		messageData.int2 = 2;
		messageData.int3 = 3;
		messageData.int4 = 4;
		messageData.int5 = 5;
		messageData.int6 = 6;
		messageData.int7 = 7;
		messageData.int8 = 8;
		messageData.int9 = 9;
		messageData.float10 = 10.10f;
		messageData.float11 = 11.11f;
		messageData.float12 = 12.12f;
		messageData.float13 = 13.13f;
		messageData.float14 = 14.14f;
		messageData.float15 = 15.15f;
		messageData.float16 = 16.16f;
		messageData.float17 = 17.17f;
		messageData.float18 = 18.18f;
		messageData.float19 = 19.19f;

		message.reserveAtLeast(22);
		message.addInt(0);
		message.add<MessageData>(&messageData);
		message.addInt(12345);

		message.setValidData(true);

		reserveAtLeast(1);
		stageStruct<MessageData>(DummyEnum::def, &messageData);
	}

	void loop() {
	}

	OSC::Message * generateMessage() {
		return &message;
	}

	const char * pattern() {
		return message.address;
	}

	void callbackEnum(DummyEnum dummyEum) {
		int data = messageData.int0;
		message.setInt(1, data);
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