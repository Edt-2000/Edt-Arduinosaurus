/*
Edt-LED

Using PlatformIO
*/
#include "RGBApplication.h"

RGBApplication application = RGBApplication();

void setup()
{
	application.setup();
}

void loop()
{
	application.loop();
}