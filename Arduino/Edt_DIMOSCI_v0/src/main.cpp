#include <Arduino.h>
#include <Midi.h>

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {

  Serial.begin(57600);
  MIDI.begin(0);

}

void loop() {

  if (MIDI.read()) {

    Serial.print(MIDI.getChannel(), HEX);
    Serial.print(" ");
    Serial.print(MIDI.getType(), HEX);
    Serial.print(" ");
    Serial.print(MIDI.getData1(), HEX);
    Serial.print(" ");
    Serial.println(MIDI.getData2(), HEX);

  }

}


/**********************************************************************************/
/*    Demo program for:								  */
/*	  Board: Shield-MIDI + Olimexino-328    				  */
/*    Manufacture: OLIMEX                                                   	  */
/*	  COPYRIGHT (C) 2013							  */
/*    Designed by: Engineer Penko T. Bozhkov                                      */
/*    Module Name    :  MIDI_Demo_Example                                         */
/*    File   Name    :  MIDI_Demo_Example                                         */
/*    Revision       :  1.0.0 (initial)                                           */
/*    Date           :  17.05.2013                                                */
/*    Built with Arduino C/C++ Compiler, version: 1.0.4                           */
/**********************************************************************************/
/*
  Description:
  ============
  When Olimexino-328's BUT is pressed, then its LED2(Yellow) is switched over and stream of messages is sent over the MIDI-OUT connector.
  This stream is part of a song. During this process, receiving is not possible in the current demo example.
  Once the sending has been finished, receiving is enabled.
  Whenever message is received, Olimexino-328's LED1(Green) is switched over and the received data is displayed on the software serial terminal named: mySerial.
  The physical pins that are selected to refer to mySerial are: D12 for RxD, D11 for TxD and GND must not be forgotten!
  At Olimexino-328 board, this pins are connected to the UEXT connector. This gives us an opportunity to connect
  there USB-Serial-Cable-F, see https://www.olimex.com/Products/Components/Cables/USB-Serial-Cable/USB-Serial-Cable-F/ 
  and use it as a terminal for observing all received MIDI messages. The required physical connections are as follows:
  USB-Serial-Cable-F(Blue)  -> connected to UEXT pin 2 (GND)
  USB-Serial-Cable-F(Red)   -> connected to UEXT pin 7 (D12, i.e. RxD)
  USB-Serial-Cable-F(Green) -> connected to UEXT pin 8 (D11, i.e. TxD)
  Once you have done this connections, plug your USB-Serial-Cable-F in any USB and open serial port with settings: 57600 8N1
  and enjoy while you are improvising your ideas! ;-)
  
*/
// In this source is used the library: http://arduino.cc/en/Reference/SoftwareSerial
// #include <SoftwareSerial.h>

// // SoftwareSerial mySerial(12, 11);   // RX, TX
// const int buttonPin = 2;           // the number of the pushbutton pin
// const int ledPin1 =  13;           // the number of the LED1 pin
// const int ledPin2 =  9;            // the number of the LED2 pin
// const int NumberDetections = 50;   // Number of the detections before accepting Button as pressed

// // variables will change:
// int buttonState = 0;         // variable for reading the pushbutton status
// int buttonCounter = NumberDetections;
// int MIDIRecBytes = 0;
// unsigned char temp = 0;

// void setup()  
// {
//   noInterrupts();
  
//   // initialize the LED pins as outputs:
//   pinMode(ledPin1, OUTPUT);
//   pinMode(ledPin2, OUTPUT);      
//   // initialize the pushbutton pin as an input:
//   pinMode(buttonPin, INPUT); 
//   // Open serial communications and wait for port to open:
//   Serial1.begin(31250);  // Select the MIDI standard baud rate
//   while (!Serial1) {
//     ; // wait for serial port to connect. Needed for Leonardo only
//   }

//   // set the data rate for the SoftwareSerial port(must be higher than the baudrate of the MIDI communication)
//   Serial.begin(57600);
  

//   Serial.println("\f"\
//                     "=====================\n\r"\
//                     " MIDI Demo Program!!!\n\r"\
//                     "=====================\n\r"\
//                     "Received Data in HEX format: \n\r");
                    
//   interrupts();
// }

// void loop() // run over and over
// {
//   // If any MIDI messages are incomming, display them to the mySerial terminal
//   while(Serial1.available()){
//     temp = Serial1.read();
//     // HEX to ASCII converter
//     if((temp>>4)>9){ Serial.write(((temp>>4)+0x37)); }      // 0x37+0x0A = 0x41 (ASCII of "A")
//     else{ Serial.write(((temp>>4)+0x30)); }                 // 0x30 (ASCII of "0")
//     // Send ASCII code of the first digit
//     if((temp&0x0F)>9){ Serial.write(((temp&0x0F)+0x37)); }  // 0x37+0x0A = 0x41 (ASCII of "A")
//     else{ Serial.write(((temp&0x0F)+0x30)); }               // 0x30 (ASCII of "0")
//     // Send ASCII code of the second digit
//     ///*
//     // Print only 6 HEX digits at one row
//     MIDIRecBytes++;
//     //if(!(MIDIRecBytes&1)){ mySerial.write(0x20); } // Space if even number
//     Serial.write(0x20); // Space
//     if(MIDIRecBytes==6){
//       MIDIRecBytes = 0;
//       Serial.write("\r\n");  // New line
//     }
//     // Switch over LED1 at every received byte
//     if(digitalRead(ledPin1)==HIGH){  digitalWrite(ledPin1, LOW);  }         // turn LED off
//     else{                            digitalWrite(ledPin1, HIGH); }         // turn LED on
//     //*/
//   }
  
//   // Echo - only for debug purpose
//   //if (mySerial.available()){ Serial.write(mySerial.read()); }
//   //if (Serial.available()){ mySerial.write(Serial.read()); }
    
// }
