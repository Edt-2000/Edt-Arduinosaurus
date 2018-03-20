/*
Edt-Trak

Using PlatformIO
*/
#include "TrakApplication.h"

TrakApplication application = TrakApplication();

// Define various ADC prescaler
// http://www.microsmart.co.za/technical/2014/03/01/advanced-arduino-adc/
//const unsigned char PS_16 = (1 << ADPS2);
//const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
//const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
//const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

void setup() {
	// set up the ADC
	// ADCSRA &= ~PS_128;
	// ADCSRA |= PS_16;
	
	application.setup();
}

void loop() {
	application.loop();
}