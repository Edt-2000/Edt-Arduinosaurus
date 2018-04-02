#include "stdafx.h"
#include "CppUnitTest.h"

#include "../../../../OSC-light/OSC-light/OSCArduino.h"
#include "../Edt-devices/EdtRGB.h"
#include "../Edt-devices/RGBColorScheduler.h"
#include "../Edt-devices/RGBColorScheduler.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EdtdevicesTest
{		
	TEST_CLASS(RGB)
	{
	public:

		TEST_METHOD(RGB_SinglePulse)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::SinglePulse);
			message.addInt(0);
			message.addInt(127);
			message.addInt(1);
			message.addInt(2);
			message.addInt(3);
			message.addInt(4);

			auto rgb = OSC::Device::EdtRGB("/Test", 5, nullptr);
			
			arduino.addConsumer(&rgb);

			auto stream = Stream();

			arduino.bindStream(&stream);
			message.send(&stream);

			arduino.loop();

			auto color = rgb._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::solid, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.s == 2, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.v == 3, L"Command param incorrect", LINE_INFO());

			auto fade = rgb._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::fade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.duration == 4, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.mode == FadeMode::FadeToBlack, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(RGB_SingleSolid)
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

			auto rgb = OSC::Device::EdtRGB("/Test", 5, nullptr);

			arduino.addConsumer(&rgb);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = rgb._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::solid, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.s == 2, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.v == 3, L"Command param incorrect", LINE_INFO());

			auto fade = rgb._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::disableFade, L"Command type incorrect", LINE_INFO());
		}

		TEST_METHOD(RGB_SingleSpark)
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

			auto rgb = OSC::Device::EdtRGB("/Test", 5, nullptr);

			arduino.addConsumer(&rgb);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = rgb._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::solid, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.s == 2, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.v == 3, L"Command param incorrect", LINE_INFO());

			auto fade = rgb._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::fade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.duration == 4, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.mode == FadeMode::FadeOneByOne, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(RGB_DualPulse)
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

			auto rgb = OSC::Device::EdtRGB("/Test", 5, nullptr);

			arduino.addConsumer(&rgb);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = rgb._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::solid, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.dh == 2, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.s == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.v == 254, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.percentage == 3, L"Command param incorrect", LINE_INFO());

			auto fade = rgb._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::fade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.duration == 4, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.mode == FadeMode::FadeToBlack, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(RGB_DualSolid)
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

			auto rgb = OSC::Device::EdtRGB("/Test", 5, nullptr);

			arduino.addConsumer(&rgb);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = rgb._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::solid, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.dh == 2, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.s == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.v == 254, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.percentage == 3, L"Command param incorrect", LINE_INFO());

			auto fade = rgb._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::disableFade, L"Command type incorrect", LINE_INFO());
		}

		TEST_METHOD(RGB_DualSparkle)
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

			auto rgb = OSC::Device::EdtRGB("/Test", 5, nullptr);

			arduino.addConsumer(&rgb);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = rgb._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::solid, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.dh == 2, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.s == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.v == 254, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.percentage == 3, L"Command param incorrect", LINE_INFO());

			auto fade = rgb._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::fade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.duration == 4, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.mode == FadeMode::FadeOneByOne, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(RGB_RainbowPulse)
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

			auto rgb = OSC::Device::EdtRGB("/Test", 5, nullptr);

			arduino.addConsumer(&rgb);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = rgb._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::rainbow, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.dh == 2, L"Command param incorrect", LINE_INFO());

			auto fade = rgb._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::fade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.duration == 3, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.mode == FadeMode::FadeToBlack, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(RGB_RainbowSolid)
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

			auto rgb = OSC::Device::EdtRGB("/Test", 5, nullptr);

			arduino.addConsumer(&rgb);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = rgb._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::rainbow, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.dh == 2, L"Command param incorrect", LINE_INFO());

			auto fade = rgb._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::disableFade, L"Command type incorrect", LINE_INFO());
		}

		TEST_METHOD(RGB_RainbowSpark)
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

			auto rgb = OSC::Device::EdtRGB("/Test", 5, nullptr);

			arduino.addConsumer(&rgb);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = rgb._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::rainbow, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.dh == 2, L"Command param incorrect", LINE_INFO());

			auto fade = rgb._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::fade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.duration == 3, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.mode == FadeMode::FadeOneByOne, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(RGB_VuMeter)
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

			auto rgb = OSC::Device::EdtRGB("/Test", 5, nullptr);

			arduino.addConsumer(&rgb);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = rgb._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::intensity, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.intensity == 3, L"Command param incorrect", LINE_INFO());

			auto fade = rgb._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::none, L"Command type incorrect", LINE_INFO());
		}

		TEST_METHOD(RGB_Twinkle)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::Twinkle);
			message.addInt(0);
			message.addInt(127);
			message.addInt(1);
			message.addInt(255);
			
			auto rgb = OSC::Device::EdtRGB("/Test", 5, nullptr);

			arduino.addConsumer(&rgb);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = rgb._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::solid, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.s == 255, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.v == 254, L"Command param incorrect", LINE_INFO());

			auto fade = rgb._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::disableFade, L"Command type incorrect", LINE_INFO());
		}

		TEST_METHOD(RGB_TwinkleFade)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::Twinkle);
			message.addInt(0);
			message.addInt(127);
			message.addInt(1);
			message.addInt(0);

			auto rgb = OSC::Device::EdtRGB("/Test", 5, nullptr);

			arduino.addConsumer(&rgb);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = rgb._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::none, L"Command type incorrect", LINE_INFO());
			
			auto fade = rgb._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::fade, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(fade.duration == 127, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(fade.mode == FadeMode::FadeToBlack, L"Command param incorrect", LINE_INFO());
		}

		TEST_METHOD(RGB_Strobo)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::Strobo);
			message.addInt(1);
			message.addInt(2);

			auto rgb = OSC::Device::EdtRGB("/Test", 5, nullptr);

			arduino.addConsumer(&rgb);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = rgb._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::strobo, L"Command type incorrect", LINE_INFO());
			Assert::IsTrue(color.h == 1, L"Command param incorrect", LINE_INFO());
			Assert::IsTrue(color.intensity == 2, L"Command param incorrect", LINE_INFO());

			auto fade = rgb._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::none, L"Command type incorrect", LINE_INFO());
		}

		TEST_METHOD(RGB_Kitt)
		{
			auto message = OSC::Message();
			auto arduino = OSC::Arduino<OSC::StructMessage<OSC::EdtMessage, uint8_t>>(1, 0);

			message.setAddress("/Test");

			message.addInt((int)OSC::ColorCommands::Kitt);
			message.addInt(1);
			message.addInt(2);
			message.addInt(3);

			auto rgb = OSC::Device::EdtRGB("/Test", 5, nullptr);

			arduino.addConsumer(&rgb);

			auto stream = Stream();

			arduino.bindStream(&stream);

			message.send(&stream);

			arduino.loop();

			auto color = rgb._colorScheduler.getCommandColor();

			Assert::IsTrue(color.type == Type::none, L"Command type incorrect", LINE_INFO());
			
			auto fade = rgb._colorScheduler.getCommandFade();

			Assert::IsTrue(fade.type == Type::none, L"Command type incorrect", LINE_INFO());
		}

	};
}