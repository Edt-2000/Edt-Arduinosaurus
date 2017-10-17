/*
Edt-LED

Using PlatformIO
*/
#include "LEDApplication.h"

LEDApplication application = LEDApplication();

void setup()
{
	application.setup();
}

void loop()
{
	application.loop();
}