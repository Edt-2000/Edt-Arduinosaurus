/*
Edt-DOSMCX

Using PlatformIO
*/
#include "DMXApplication.h"

DMXApplication application = DMXApplication();

void setup()
{
	application.setup();
}

void loop()
{
	application.loop();
}

// #include <Ethernet.h>
// #include <Udp.h>

// #include "AbstractApplication.h"

// #include "Definitions.h"

// #include "Arduino.h"
// #include "HardwareSerial.h"
// #include "OSCArduino.h"
// #include "Statemachine.h"
// #include "Time.h"

// #include "EdtFastLED.h"
// #include "EdtRGB.h"

// #include "DMXSerial.h"
// #include "DMXSlave.h"
// #include "DMXLedSpot.h"

// EthernetUDP udp;

// DMXSlave * led1;
// DMXSlave * led2;

// void setup()
// {
//     udp = EthernetUDP();

//     Ethernet.begin(MAC_LED, IPAddress(192, 168, 1, 122));

//     udp.begin(12345);

//     DMXSerial.init(DMXController);

//     led1 = new DMXLedSpot();
//     led2 = new DMXLedSpot();

//     led1->initialize(4);
//     led2->initialize(10);
// }

// void loop()
// {
//     // int brightness;

//     // for (brightness = 0; brightness <= 255; brightness++) {
        
//     //     led1->color(0, 255, brightness);
//     //     led2->color(0, 255, brightness);
        
//     //     delay(10);
//     // }
    
//     // for (brightness = 0; brightness <= 255; brightness++) {
        
//     //     led1->color(85, 255, brightness);
//     //     led2->color(85, 255, brightness);
        
//     //     delay(10);
//     // }
    
//     // for (brightness = 0; brightness <= 255; brightness++) {
        
//     //     led1->color(170, 255, brightness);
//     //     led2->color(170, 255, brightness);
        
//     //     delay(10);
//     // }
    
//     // for (brightness = 0; brightness <= 255; brightness++) {
        
//     //     led1->color(0, 0, brightness);
//     //     led2->color(0, 0, brightness);
        
//     //     delay(10);
//     // }
    
//     // delay(1000);
//     // for (brightness = 0; brightness <= 255; brightness++) {
//     //     led1->strobo(brightness);
//     //     led2->strobo(brightness);

//     //     delay(100);
//     // }

//     // DMXSerial.write(5, 0);
    
//     auto prev = millis();
//     auto loops = 0;
//     do {
//         if(millis() - prev > 40UL) {
//             loops++;

//             prev = millis();

// 			led1->intensity(loops);
// 			led2->intensity(loops);

//             led1->loop();
//             led2->loop();

//         }

//     } while(true);
// }
