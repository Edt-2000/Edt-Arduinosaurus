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
#include "RGBLED.h"
#include "LED.h"

//#include "FastLED.h"

class LEDApplication : public AbstractApplication
{
public:
	EdtRGBLED rgbLed1 = EdtRGBLED(OSC_LED1, LED1_NR_OF_LEDS);
	EdtRGBLED rgbLed2 = EdtRGBLED(OSC_LED2, LED2_NR_OF_LEDS);
	EdtRGBLED rgbLed3 = EdtRGBLED(OSC_LED3, LED3_NR_OF_LEDS);

	EdtLED led1 = EdtLED(OSC_ONOFFLED1, 3);
	EdtLED led2 = EdtLED(OSC_ONOFFLED2, 5);
	EdtLED led3 = EdtLED(OSC_ONOFFLED3, 6);
	EdtLED led4 = EdtLED(OSC_ONOFFLED4, 9);
	EdtLED led5 = EdtLED(OSC_ONOFFLED5, 11);
	EdtLED led6 = EdtLED(OSC_ONOFFLED6, 13);

	EthernetUDP udp;
	
	void setupStatus() {
		status.setup(13, HIGH);
	}

	void setupNetwork() {
		Ethernet.begin(MAC_PING);

		udp.begin(PORT_BROADCAST);
	}

	void setupOsc() {
		rgbLed1.configurePins<A0,A5>();
		rgbLed2.configurePins<A1,A5>();
		rgbLed3.configurePins<A2,A5>();

		osc = OSC::Arduino(9, 0);
		osc.bindUDP(&udp, IP_BROADCAST, PORT_BROADCAST);
		osc.addConsumer(&rgbLed1);
		osc.addConsumer(&rgbLed2);
		osc.addConsumer(&rgbLed3);
		osc.addConsumer(&led1);
		osc.addConsumer(&led2);
		osc.addConsumer(&led3);
		osc.addConsumer(&led4);
		osc.addConsumer(&led5);
		osc.addConsumer(&led6);

		// make a test blink
		rgbLed1.test();
		rgbLed2.test();
		rgbLed3.test();
		led1.test();
		led2.test();
		led3.test();
		led4.test();
		led5.test();
		led6.test();
	}

	void applicationLoop() {
		if (time.tVISUAL) {
			FastLED.show();
			
			rgbLed1.animationLoop();
			rgbLed2.animationLoop();
			rgbLed3.animationLoop();
			led1.animationLoop();
			led2.animationLoop();
			led3.animationLoop();
			led4.animationLoop();
			led5.animationLoop();
			led6.animationLoop();
		}
	}
};