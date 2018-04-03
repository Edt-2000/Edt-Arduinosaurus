#pragma once

#define USB

#include "AbstractApplication.h"

#include "Definitions.h"

#include "Arduino.h"
#include "HardwareSerial.h"
#include "OSCArduino.h"
#include "Statemachine.h"
#include "Time.h"

#include "EdtFastLED.h"
#include "EdtRGB.h"

#include "FadeMode.h"
#include "SparkFun_Tlc5940.h"

class LEDApplication : public AbstractApplication
{
  public:
	OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>> osc;

	Tlc5940 tlc = Tlc5940();
	
	OSC::Device::EdtFastLED fl1 = OSC::Device::EdtFastLED(OSC_FL1, FL1_NR_OF_LEDS);
	OSC::Device::EdtFastLED fl2 = OSC::Device::EdtFastLED(OSC_FL2, FL2_NR_OF_LEDS);
	OSC::Device::EdtFastLED fl3 = OSC::Device::EdtFastLED(OSC_FL3, FL3_NR_OF_LEDS);
	OSC::Device::EdtFastLED fl4 = OSC::Device::EdtFastLED(OSC_FL4, FL4_NR_OF_LEDS);

	OSC::Device::EdtRGB rgb1 = OSC::Device::EdtRGB(OSC_RGB1, RGB1_NR_OF_LEDS, &tlc);
	
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

		Ethernet.begin(MAC_LED);

		udp.begin(PORT_BROADCAST);

#endif
		tlc.init(4095);
	}

	void setupOsc()
	{
		fl1.configurePins<A0, 3>();
		fl2.configurePins<A1, 3>();
		fl3.configurePins<A2, 3>();
		fl4.configurePins<A3, 3>();

		osc = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(5, 0);
#ifdef USB
		osc.bindStream(&Serial);
#else
		osc.bindUDP(&udp, IP_BROADCAST, PORT_BROADCAST);
#endif
		osc.addConsumer(&fl1);
		osc.addConsumer(&fl2);
		osc.addConsumer(&fl3);
		osc.addConsumer(&fl4);

		osc.addConsumer(&rgb1);

		// make a test blink
		fl1.test();
		fl2.test();
		fl3.test();
		fl4.test();

		rgb1.test();

		tlc.clear();
	}

	void applicationLoop()
	{
		osc.loop(time.tOSC);

		if (time.tVISUAL)
		{
			fl1.animationLoop();
			fl2.animationLoop();
			fl3.animationLoop();
			fl4.animationLoop();

			rgb1.animationLoop();

			FastLED.show();

			tlc.update();
		}
	}
};