#include <Arduino.h>

#include "AbstractApplication.h"

#include "Definitions.h"

#include "Arduino.h"
#include "HardwareSerial.h"
#include "OSCArduino.h"
#include "Statemachine.h"
#include "Time.h"

#define ProMicro1 1
#define ProMicro2 2

#if ProMicro == ProMicro1
const char address[] = "/N1";
const char otherAddress[] = "/N1";
#elif ProMicro == ProMicro2
const char address[] = "/N1";
const char otherAddress[] = "/N1";
#else
const char address[] = "/Nx";
const char otherAddress[] = "/Nx";
#endif

struct Message
{
    uint8_t int1;
    uint8_t int2;
    uint8_t int3;
    uint8_t int4;
};

struct InterMessage
{
    uint32_t int1;
};

int cheat1 = 0;

class Node : public OSC::MessageProducer<OSC::StructMessage<Message, uint8_t>>
{
  public:
    OSC::StructMessage<Message, uint8_t> message;
    uint8_t i = 0;

    Node(const char *address)
    {
        message.setAddress(address);
    }

    void loop()
    {
    }

    OSC::StructMessage<Message, uint8_t> *generateMessage()
    {
        ++i;

        message.messageStruct.int1 = i;
        message.messageStruct.int2 = cheat1;
        message.messageStruct.int3 = Serial1.available();
        message.messageStruct.int4 = Serial1.peek();

        message.setValidData(true);

        return &message;
    }
};

class NodeServer : public OSC::MessageProducer<OSC::StructMessage<InterMessage, uint32_t>>
{
  public:
    OSC::StructMessage<InterMessage, uint32_t> message;
    uint32_t i = 0;

    NodeServer(const char *address)
    {
        message.setAddress(address);
    }

    void loop()
    {
    }

    OSC::StructMessage<InterMessage, uint32_t> *generateMessage()
    {
        ++i;

        message.messageStruct.int1 = i;

        message.setValidData(true);

        return &message;
    }
};

class NodeClient : public OSC::MessageConsumer<OSC::StructMessage<InterMessage, uint32_t>>
{
  public:
    const char *_address;
    bool _state = false;

    NodeClient(const char *address)
    {
        address = address;
    }

    const char *pattern()
    {
        return address;
    }

    void callbackMessage(OSC::StructMessage<InterMessage, uint32_t> *message)
    {
        _state = !_state;
        digitalWrite(6, _state ? HIGH : LOW);

        cheat1 = message->messageStruct.int1;
    }
};

class GuitarApplication : public AbstractApplication
{
  public:
    OSC::Arduino<OSC::StructMessage<Message, uint8_t>> osc;
    OSC::Arduino<OSC::StructMessage<InterMessage, uint32_t>> interArduinoOsc;

    Node node = Node(address);
    NodeServer interNodeServer = NodeServer(address);
    NodeClient interNodeClient = NodeClient(otherAddress);

    void setupStatus()
    {
        status.setup(5, HIGH);
    }

    void setupNetwork()
    {
        Serial.begin(57600);

        while (!Serial)
        {
            delay(1000);
        }

        pinMode(0, INPUT_PULLUP);
        pinMode(1, INPUT_PULLUP);
        
        Serial1.begin(9600);
    }

    void setupOsc()
    {
        osc = OSC::Arduino<OSC::StructMessage<Message, uint8_t>>(0, 1);

        osc.bindStream(&Serial);

        osc.addProducer(&node);

        interArduinoOsc = OSC::Arduino<OSC::StructMessage<InterMessage, uint32_t>>(1, 1);

        interArduinoOsc.bindStream(&Serial1);

        interArduinoOsc.addProducer(&interNodeServer);
        interArduinoOsc.addConsumer(&interNodeClient);
    }

    void applicationLoop()
    {
    //if(time.t1000ms) {
        osc.loop(time.t100ms);
        interArduinoOsc.loop(time.t100ms);
        //}
    }
} application;

bool state = false;

void setup()
{
    pinMode(6, OUTPUT);

    application.setup();
}

void loop()
{
    application.loop();

    // state = !state;
    // digitalWrite(5, state);

    // //hs.write(message, 3);

    // hs.loop();

    // // if (hs.available() > 0)
    // // {
    // //     hs.flush();
    // // }

    // //Serial.println(message);

    // //auto string = Serial1.readStringUntil('\n');

    // //Serial.print("Received: ");
    // //Serial.println(string);

    // digitalWrite(6, hs.healthy() ? HIGH : LOW);

    // delay(50);
}
