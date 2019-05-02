#pragma once

#ifdef _MSC_VER
#include "../Edt-devicesTest/stdafx.h"
#include "../../../Arduino/libraries/Edt-lib/OSCMessageDefinitions.h"
#include "../../../../OSC-light/OSC-light/OSCArduino.h"
#else
#include <OSCArduino.h>
#include <OSCMessageProducer.h>
#include <OSCMessageDefinitions.h>
#endif

#include "MIDI.h"

namespace OSC
{
namespace Device
{
class EdtMIDI : public OSC::MessageProducer<OSC::StructMessage<OSC::EdtMessage, uint8_t>>
{
  private:
	const char *_pattern;
	
  public:
	EdtMIDI(const char *pattern) : MessageProducer()
	{
		_pattern = pattern;
	}

	void loop() {

    }

    MessageType * generateMessage() {
        
    }
};

} // namespace Device
} // namespace OSC