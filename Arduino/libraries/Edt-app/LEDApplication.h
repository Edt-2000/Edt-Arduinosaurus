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

#ifdef EDTLED1
	OSC::Device::EdtFastLED fls[4] = { 
		OSC::Device::EdtFastLED(OSC_FL1, FL1_NR_OF_LEDS),
		OSC::Device::EdtFastLED(OSC_FL2, FL2_NR_OF_LEDS),
		OSC::Device::EdtFastLED(OSC_FL3, FL3_NR_OF_LEDS),
		OSC::Device::EdtFastLED(OSC_FL4, FL4_NR_OF_LEDS)
	};
#endif

#ifdef EDTLED2
	OSC::Device::EdtFastLED fls[4] = { 
		OSC::Device::EdtFastLED(OSC_FL5, FL5_NR_OF_LEDS),
		OSC::Device::EdtFastLED(OSC_FL6, FL6_NR_OF_LEDS),
		OSC::Device::EdtFastLED(OSC_FL7, FL7_NR_OF_LEDS),
		OSC::Device::EdtFastLED(OSC_FL8, FL8_NR_OF_LEDS)
	};
#endif

#ifdef EDTLED1
	OSC::Device::EdtRGB rgb = OSC::Device::EdtRGB(OSC_RGB1, RGB1_NR_OF_LEDS, &tlc);
#endif

#ifdef EDTLED2
	OSC::Device::EdtRGB rgb = OSC::Device::EdtRGB(OSC_RGB2, RGB2_NR_OF_LEDS, &tlc);
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

		Ethernet.begin(MAC_LED);

		udp.begin(PORT_BROADCAST);

#endif
		tlc.init(4095);
	}

	void setupOsc()
	{
		// pinout
		//
		// FL 1 | FL 2 | FL 3 | FL 4 |
		// 0    | 2    | 3    | 1    |
 

		fls[0].configurePins<A0, 3>();
		fls[1].configurePins<A2, 3>();
		fls[2].configurePins<A3, 3>();
		fls[3].configurePins<A1, 3>();

		osc = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(5, 0);
#ifdef USB
		osc.bindStream(&Serial);
#else
		osc.bindUDP(&udp, IP_BROADCAST, PORT_BROADCAST);
#endif
		osc.addConsumer(&fls[0]);
		osc.addConsumer(&fls[1]);
		osc.addConsumer(&fls[2]);
		osc.addConsumer(&fls[3]);

		osc.addConsumer(&rgb);

		// make a test blink
		fls[0].test();
		fls[1].test();
		fls[2].test();
		fls[3].test();

		rgb.test();

		tlc.clear();
	}

	void applicationLoop()
	{
		osc.loop(time.tOSC);

		if (time.tVISUAL)
		{
			fls[0].animationLoop();
			fls[1].animationLoop();
			fls[2].animationLoop();
			fls[3].animationLoop();

			rgb.animationLoop();

			FastLED.show();

			tlc.update();
		}
	}
};