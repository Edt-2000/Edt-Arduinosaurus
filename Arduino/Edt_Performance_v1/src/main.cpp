/*
Edt-Performance

Using PlatformIO
*/
#include "PerformanceApplication.h"

PerformanceApplication application = PerformanceApplication();

void setup() {
	application.setup();
}

void loop() {
	application.loop();
}