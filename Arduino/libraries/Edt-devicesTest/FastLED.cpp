#include "stdafx.h"
#include "CppUnitTest.h"

#include "../../../../OSC-light/OSC-light/OSCArduino.h"
#include "../../../../OSC-light/OSC-light/IMessage.h"
#include "../Edt-devices/EdtFastLED.h"
#include "../Edt-devices/FastLEDColorScheduler.h"
#include "../Edt-devices/FastLEDColorScheduler.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EdtdevicesTest
{
	TEST_CLASS(FastLED)
	{
	public:

		TEST_METHOD(FastLED_SinglePulse)
		{
			auto message = OSC::StructMessage<OSC::EdtMessage, uint8_t>();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.messageStruct.command = OSC::ColorCommands::SinglePulse;
			message.messageStruct.commands.singleColor.start = 0;
			message.messageStruct.commands.singleColor.end = 127;
			message.messageStruct.commands.singleColor.hue = 1;
			message.messageStruct.commands.singleColor.saturation = 2;
			message.messageStruct.commands.singleColor.value = 3;
			message.messageStruct.commands.singleColor.duration = 4;

			/*message.addInt((int)OSC::ColorCommands::SinglePulse);
			message.addInt(0);
			message.addInt(127);
			message.addInt(1);
			message.addInt(2);
			message.addInt(3);
			message.addInt(4);
*/
			auto fastLed = OSC::Device::EdtFastLED("/Test", 1);

			arduino.addConsumer(&fastLed);

			auto stream = Stream();

			arduino.bindStream(&stream);
			message.send(&stream);

			arduino.loop();

			auto color = fastLed._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::solid, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.s == 2, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.v == 3, L"Command param incorrect", LINE_INFO());

			auto fade = fastLed._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::fade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.duration == 4, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.mode == FadeMode::FadeToBlack, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(FastLED_SingleSolid)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::SingleSolid);
			message.addInt(0);
			message.addInt(127);
			message.addInt(1);
			message.addInt(2);
			message.addInt(3);

			auto fastLed = OSC::Device::EdtFastLED("/Test", 1);

			arduino.addConsumer(&fastLed);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = fastLed._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::solid, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.s == 2, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.v == 3, L"Command param incorrect", LINE_INFO());

			auto fade = fastLed._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::disableFade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.end == 127, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(FastLED_SingleSpark)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::SingleSpark);
			message.addInt(0);
			message.addInt(127);
			message.addInt(1);
			message.addInt(2);
			message.addInt(3);
			message.addInt(4);

			auto fastLed = OSC::Device::EdtFastLED("/Test", 1);

			arduino.addConsumer(&fastLed);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = fastLed._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::solid, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.s == 2, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.v == 3, L"Command param incorrect", LINE_INFO());

			auto fade = fastLed._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::fade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.duration == 4, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.mode == FadeMode::FadeOneByOne, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(FastLED_DualPulse)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::DualPulse);
			message.addInt(0);
			message.addInt(127);
			message.addInt(1);
			message.addInt(2);
			message.addInt(3);
			message.addInt(4);

			auto fastLed = OSC::Device::EdtFastLED("/Test", 1);

			arduino.addConsumer(&fastLed);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = fastLed._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::solid, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.s == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.v == 255, L"Command param incorrect", LINE_INFO());

			auto twinkle = fastLed._colorScheduler.getCommandTwinkle();

			Assert::IsTrue(twinkle.type == Type::twinkle, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.h == 2, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.s == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.v == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.intensity == 3, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.blackOut == false, L"Command param incorrect", LINE_INFO());

			auto fade = fastLed._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::fade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.duration == 4, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.mode == FadeMode::FadeToBlack, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(FastLED_DualSolid)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::DualSolid);
			message.addInt(0);
			message.addInt(127);
			message.addInt(1);
			message.addInt(2);
			message.addInt(3);
			message.addInt(4);

			auto fastLed = OSC::Device::EdtFastLED("/Test", 1);

			arduino.addConsumer(&fastLed);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = fastLed._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::solid, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.s == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.v == 255, L"Command param incorrect", LINE_INFO());

			auto twinkle = fastLed._colorScheduler.getCommandTwinkle();

			Assert::IsTrue(twinkle.type == Type::twinkle, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.h == 2, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.s == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.v == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.intensity == 3, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.blackOut == false, L"Command param incorrect", LINE_INFO());

			auto fade = fastLed._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::disableFade, L"Command type incorrect", LINE_INFO());
		}

		TEST_METHOD(FastLED_DualSparkle)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::DualSparkle);
			message.addInt(0);
			message.addInt(127);
			message.addInt(1);
			message.addInt(2);
			message.addInt(3);
			message.addInt(4);

			auto fastLed = OSC::Device::EdtFastLED("/Test", 1);

			arduino.addConsumer(&fastLed);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = fastLed._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::solid, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.s == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.v == 255, L"Command param incorrect", LINE_INFO());

			auto twinkle = fastLed._colorScheduler.getCommandTwinkle();

			Assert::IsTrue(twinkle.type == Type::twinkle, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.h == 2, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.s == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.v == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.intensity == 3, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(twinkle.blackOut == false, L"Command param incorrect", LINE_INFO());

			auto fade = fastLed._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::fade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.duration == 4, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.mode == FadeMode::FadeOneByOne, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(FastLED_RainbowPulse)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::RainbowPulse);
			message.addInt(0);
			message.addInt(127);
			message.addInt(1);
			message.addInt(2);
			message.addInt(3);

			auto fastLed = OSC::Device::EdtFastLED("/Test", 1);

			arduino.addConsumer(&fastLed);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = fastLed._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::rainbow, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.dh == 2, L"Command param incorrect", LINE_INFO());

			auto fade = fastLed._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::fade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.duration == 3, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.mode == FadeMode::FadeToBlack, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(FastLED_RainbowSolid)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::RainbowSolid);
			message.addInt(0);
			message.addInt(127);
			message.addInt(1);
			message.addInt(2);
			message.addInt(3);

			auto fastLed = OSC::Device::EdtFastLED("/Test", 1);

			arduino.addConsumer(&fastLed);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = fastLed._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::rainbow, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.dh == 2, L"Command param incorrect", LINE_INFO());

			auto fade = fastLed._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::disableFade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.end == 127, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(FastLED_RainbowSpark)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::RainbowSpark);
			message.addInt(0);
			message.addInt(127);
			message.addInt(1);
			message.addInt(2);
			message.addInt(3);

			auto fastLed = OSC::Device::EdtFastLED("/Test", 1);

			arduino.addConsumer(&fastLed);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = fastLed._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::rainbow, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.dh == 2, L"Command param incorrect", LINE_INFO());

			auto fade = fastLed._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::fade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.duration == 3, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.mode == FadeMode::FadeOneByOne, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(FastLED_VuMeter)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::VuMeter);
			message.addInt(0);
			message.addInt(127);
			message.addInt(63);
			message.addInt(1);
			message.addInt(2);
			message.addInt(3);

			auto fastLed = OSC::Device::EdtFastLED("/Test", 1);

			arduino.addConsumer(&fastLed);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = fastLed._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::rainbow, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.center == 63, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.dh == 2, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.intensity == 3, L"Command param incorrect", LINE_INFO());

			auto fade = fastLed._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::none, L"Command type incorrect", LINE_INFO());
		}

		TEST_METHOD(FastLED_Twinkle)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::Twinkle);
			message.addInt(0);
			message.addInt(127);
			message.addInt(1);
			message.addInt(2);

			auto fastLed = OSC::Device::EdtFastLED("/Test", 1);

			arduino.addConsumer(&fastLed);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = fastLed._colorScheduler.getCommandTwinkle();

			Assert::IsTrue(color.type == Type::twinkle, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.s == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.v == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.intensity == 2, L"Command param incorrect", LINE_INFO());

			auto fade = fastLed._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::disableFade, L"Command type incorrect", LINE_INFO());
		}

		TEST_METHOD(FastLED_TwinkleFade)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::Twinkle);
			message.addInt(0);
			message.addInt(127);
			message.addInt(1);
			message.addInt(0);

			auto fastLed = OSC::Device::EdtFastLED("/Test", 1);

			arduino.addConsumer(&fastLed);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = fastLed._colorScheduler.getCommandTwinkle();

			Assert::IsTrue(color.type == Type::none, L"Command type incorrect", LINE_INFO());

			auto fade = fastLed._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::fade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.start == 0, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.end == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.duration == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.mode == FadeMode::FadeToBlack, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(FastLED_Strobo)
		{
			int i = 0;
			do {
				auto message = OSC::Message();
				auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

				message.setAddress("/Test");

				message.addInt((int)OSC::ColorCommands::Strobo);
				message.addInt(1);
				message.addInt(2);

				auto fastLed = OSC::Device::EdtFastLED("/Test", 1);

				arduino.addConsumer(&fastLed);

				auto stream = Stream();

				arduino.bindStream(&stream);

				message.send(&stream);

				arduino.loop();

				auto color = fastLed._colorScheduler.getCommandColor();

				Assert::IsTrue(color.type == Type::strobo, L"Command type incorrect", LINE_INFO());
				Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
				Assert::IsTrue(color.intensity == 2, L"Command param incorrect", LINE_INFO());

				auto fade = fastLed._colorScheduler.getCommandFade();

				Assert::IsTrue(fade.type == Type::none, L"Command type incorrect", LINE_INFO());
			} while (++i < 1000);
		}

		TEST_METHOD(FastLED_Chase)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::Chase);
			message.addInt(1);
			message.addInt(2);

			auto fastLed = OSC::Device::EdtFastLED("/Test", 1);

			arduino.addConsumer(&fastLed);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = fastLed._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::chase, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.speed == 2, L"Command param incorrect", LINE_INFO());

			auto fade = fastLed._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::none, L"Command type incorrect", LINE_INFO());
		}

	};
}