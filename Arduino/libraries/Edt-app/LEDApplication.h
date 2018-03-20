#pragma once

#define USB

#include "AbstractApplication.h"

#include "Definitions.h"

#include "Arduino.h"
#include "HardwareSerial.h"
//#include "Ethernet.h"
//#include "EthernetUdp.h"
#include "OSCArduino.h"
#include "Statemachine.h"
#include "Time.h"
//#include "Greeter.h"
#include "RGBLED.h"
#include "RGB.h"
//#include "LED.h"
#include "FadeMode.h"

class LEDApplication : public AbstractApplication
{
  public:
	Tlc5940 tlc = Tlc5940();
	EdtRGBLED rgbLed1 = EdtRGBLED(OSC_LED1, LED1_NR_OF_LEDS);
	EdtRGBLED rgbLed2 = EdtRGBLED(OSC_LED2, LED2_NR_OF_LEDS);
	EdtRGBLED rgbLed3 = EdtRGBLED(OSC_LED3, LED3_NR_OF_LEDS);

	// EdtLED led1 = EdtLED(OSC_ONOFFLED1, 3);
	// EdtLED led2 = EdtLED(OSC_ONOFFLED2, 5);
	// EdtLED led3 = EdtLED(OSC_ONOFFLED3, 6);
	// EdtLED led4 = EdtLED(OSC_ONOFFLED4, 9);
	// EdtLED led5 = EdtLED(OSC_ONOFFLED5, 11);
	// EdtLED led6 = EdtLED(OSC_ONOFFLED6, 13);
	
	EdtRGB rgb1 = EdtRGB(OSC_RGB1, 1, 2, 3, &tlc);

	//EdtGreeter greeter = EdtGreeter(OSC_GREETER);

#ifndef USB
	EthernetUDP udp;
#endif

	void setupStatus()
	{
		tlc.init();

		status.setup(13, HIGH);
		
#ifdef USB

		Serial.begin(9600);

		bool b = false;

		while(!Serial) {
			status.setup(13, b);
			b = !b;
		}

		Serial.println("Hi");

#endif

	}

	void setupNetwork()
	{
#ifdef USB

		Serial.begin(9600);

		while(!Serial) {
			;
		}

		Serial.println("Hi");

#else

		Ethernet.begin(MAC_LED);

		udp.begin(PORT_BROADCAST);

#endif
	}

	void setupOsc()
	{
    	rgbLed1.configurePins<A0, A3>(); 
    	rgbLed2.configurePins<A1, A3>(); 
    	rgbLed3.configurePins<A2, A3>(); 

		osc = OSC::Arduino(4, 0);
#ifdef USB
		osc.bindStream(&Serial);
#else
		osc.bindUDP(&udp, IP_BROADCAST, PORT_BROADCAST);
#endif
		osc.addConsumer(&rgbLed1);
		osc.addConsumer(&rgbLed2);
		osc.addConsumer(&rgbLed3);
		// osc.addConsumer(&led1);
		// osc.addConsumer(&led2);
		// osc.addConsumer(&led3);
		// osc.addConsumer(&led4);
		// osc.addConsumer(&led5);
		// osc.addConsumer(&led6);
		osc.addConsumer(&rgb1);
		//osc.addConsumer(&greeter);
		//osc.addProducer(&greeter);

		// make a test blink
		rgbLed1.test();
		rgbLed2.test();
		rgbLed3.test();
		// led1.test();
		// led2.test();
		// led3.test();
		// led4.test();
		// led5.test();
		// led6.test();
		rgb1.test();

		Tlc.clear();
	}


	void applicationLoop()
	{
		if (time.tVISUAL)
		{
			
			//tlc.set(1, ++i);
			//tlc.set(2, i * 2);
			//tlc.set(3, i * 3);


			rgbLed1.animationLoop();
			rgbLed2.animationLoop();
			rgbLed3.animationLoop();
			// led1.animationLoop();
			// led2.animationLoop();
			// led3.animationLoop();
			// led4.animationLoop();
			// led5.animationLoop();
			// led6.animationLoop();

			rgb1.animationLoop();

			FastLED.show();

			tlc.update();
		}
	}
};