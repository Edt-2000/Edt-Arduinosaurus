#pragma once

#define ENVEDT
//#define ENVTEST
//#define ENVDEV

#include "Arduino.h"

/* Definition file */

#ifdef ENVEDT
#define IP_BROADCAST IPAddress(10, 0, 0, 10)
#endif

#ifdef ENVTEST
#define IP_BROADCAST IPAddress(192, 168, 0, 255)
#endif

#ifdef ENVDEV
#define IP_BROADCAST IPAddress(192, 168, 137, 255)
#endif

#define IP_TRAK IPAddress(10, 0, 0, 12)
#define IP_SLEDT IPAddress(10, 0, 0, 200)

uint8_t MAC_TRAK[] = { 0x00, 0x00, 0x12, 0x34, 0x56, 0xA0 };
uint8_t MAC_LED[] = { 0x00, 0x00, 0x12, 0x34, 0x56, 0xB0 };
uint8_t MAC_PONG[] = { 0x00, 0x00, 0x12, 0x34, 0x56, 0xC0 };
uint8_t MAC_MOSCIDI[] = { 0x00, 0x00, 0x12, 0x34, 0x56, 0xA1 };
uint8_t MAC_DOSMCX[] = { 0x00, 0x00, 0x12, 0x34, 0x56, 0xA2 };

/*
Thing 1: 18-FE-34-CC-15-CE
Thing 2: 5C-CF-7F-8C-4A-E2
Thing 3: 18-FE-34-CC-15-D7
Thing 4: 18-FE-34-CC-15-AC
Thing 5: 18-FE-34-CC-15-C7
Thing 6: 5C-CF-7F-8A-22-DD
Thing Dev: 5C-CF-7F-8C-8F-CC
*/

#define PORT_BROADCAST uint16_t(12345)
#define PORT_SEND uint16_t(8888)

// instrument / input OSC names
#define OSC_GREETER "/HELLO"

#define OSC_TRAK "/TK"
#define OSC_PEDAL "/PD/1"
#define OSC_MIDICONTROL "/MC"
#define OSC_MIDINOTE "/MN"
#define OSC_DMX "/DX"
#define OSC_SUIT "/ST"
#define OSC_SUIT_CHUK "/ST/CK"
#define OSC_GUITAR "/GT"

#define OSC_FL1 "/L/1"
#define OSC_FL2 "/L/2"
#define OSC_FL3 "/L/3"
#define OSC_FL4 "/L/4"
#define OSC_FL5 "/L/5"

#define FL1_NR_OF_LEDS 57
#define FL2_NR_OF_LEDS 57
#define FL3_NR_OF_LEDS 57
#define FL4_NR_OF_LEDS 57

#define OSC_RGB1 "/L/1"
#define OSC_RGB2 "/L/2"

#define RGB1_NR_OF_LEDS 5
#define RGB2_NR_OF_LEDS 5