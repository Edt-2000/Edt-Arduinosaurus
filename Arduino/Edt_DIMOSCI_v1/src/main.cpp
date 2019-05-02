/*
Edt-DIMOSCI

Using PlatformIO
*/
#include "MIDIApplication.h"

MIDIApplication application = MIDIApplication();

void setup()
{
	application.setup();
}

void loop()
{
	application.loop();
}