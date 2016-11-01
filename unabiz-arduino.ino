//  radiocrafts-test
//  Send a sample message to SIGFOX base station from RC1692HP-SIG.
//  Based on https://github.com/Snootlab/Akeru

#include "Radiocrafts.h"

// TD1208 Sigfox module IO definition
/*   Snootlab device | TX | RX
               Radiocrafts | D4 | D5
               Akene | D5 | D4
            Breakout | your pick */
//#define AKENE_TX 5  //  For UnaBiz / Akene
//#define AKENE_RX 4  //  For UnaBiz / Akene

#define RADIOCRAFTS_TX 4  //  For UnaBiz / Radiocrafts Dev Kit
#define RADIOCRAFTS_RX 5  //  For UnaBiz / Radiocrafts Dev Kit

// Sigfox instance management 
Radiocrafts Radiocrafts(RADIOCRAFTS_RX, RADIOCRAFTS_TX);

void setup()
{
  // initialize console serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Demo sketch for Radiocrafts library :)");
  
  // Check TD1208 communication
  if (!Radiocrafts.begin())
  {
    Serial.println("TD1208 KO");
    while(1);
  }
  Radiocrafts.echoOn(); // comment this line to hide AT commands
}

void loop()
{
  String result = "";
  //  Enter command mode.  TODO: Confirm response = '>'
  Serial.println(F("\nEntering command mode (expecting '>')"));
  Radiocrafts.enterCommandMode();

  //  Disable emulation mode.
  Serial.println(F("\nDisable emulation mode"));
  Radiocrafts.disableEmulator(result);

  //  Check whether emulator is used for transmission.
  Serial.println(F("\nEmulator Enabled (expecting 0) = ")); int emulator = 0;
  Radiocrafts.getEmulator(emulator);

  //  Get network mode for transmission.  TODO: Confirm network mode = 0 for uplink only, no downlink.
  Serial.println(F("\nNetwork Mode (expecting 0) = "));
  Radiocrafts.getParameter(0x3b, result);
  
  //  Get baud rate.  TODO: Confirm baud rate = 5 for 19200 bps.
  Serial.println(F("\nBaud Rate (expecting 5) = "));
  Radiocrafts.getParameter(0x30, result);
  
  //  Set the frequency of SIGFOX module to SG/TW.
  Serial.println(F("\nSetting frequency "));
  result = "";
  Radiocrafts.setFrequencySG(result);
  Serial.print(F("Set frequency result = "));
  Serial.println(result);

  //  Get and display the frequency used by the SIGFOX module.  TODO: Confirm that it returns 3 for RCZ4 (SG/TW).
  Serial.println(F("\nGetting frequency "));
  String frequency = "";
  Radiocrafts.getFrequency(frequency);
  Serial.print(F("Frequency (expecting 3) = "));
  Serial.println(frequency);

  // Read module temperature
  Serial.println(F("\nGetting temperature "));
  int temperature = 0;
  if (Radiocrafts.getTemperature(temperature))
  {
    Serial.print(F("Temperature = "));
    Serial.print(temperature);
    Serial.println(F(" C"));
  }
  else
  {
    Serial.println(F("Temperature KO"));
  }

  // Read module supply voltage
  Serial.println(F("\nGetting voltage "));
  float voltage = 0.0;
  if (Radiocrafts.getVoltage(voltage))
  {
    Serial.print(F("Supply voltage = "));
    Serial.print(voltage);
    Serial.println(F(" V"));
  }
  else
  {
    Serial.println(F("Supply voltage KO"));
  }

  // Read module identification
  // Returns with 12 bytes: 4 bytes ID (LSB first) and 8 bytes PAC (MSB first).
  String id = "";
  Serial.println(F("\nGetting ID "));
  if (Radiocrafts.getID(id))
  {
    Serial.print(F("\n4 bytes ID (LSB first) and 8 bytes PAC (MSB first) = "));
    Serial.println(id);
  }
  else
  {
    Serial.println(F("ID KO"));
  }

  // Read module hardware version
  Serial.println(F("\nGetting hardware "));
  String hardware = "";
  if (Radiocrafts.getHardware(hardware))
  {
    Serial.print(F("Hardware version = "));
    Serial.println(hardware);
  }
  else
  {
    Serial.println(F("Hardware version KO"));
  }

  // Read module firmware version
  Serial.println(F("\nGetting firmware "));
  String firmware = "";
  if (Radiocrafts.getFirmware(firmware))
  {
    Serial.print(F("Firmware version = "));
    Serial.println(firmware);
  }
  else
  {
    Serial.println(F("Firmware version KO"));
  }

  // Read power 
  Serial.println(F("\nGetting power "));
  int power = 0;
  if (Radiocrafts.getPower(power))
  {
    Serial.print(F("Power level = "));
    Serial.print(power);
    Serial.println(F(" dB"));
  }
  else
  {
    Serial.println(F("Power level KO"));
  }

  //  Exit command mode.
  Radiocrafts.exitCommandMode();

  // Convert to hexadecimal before sending
  String temp = Radiocrafts.toHex(temperature);
  String volt = Radiocrafts.toHex(voltage);
  
  String msg = temp + volt; // Put everything together

  for (int i = 0; i < 10; i++) {
    Serial.println(F("\nSending payload "));
    if (Radiocrafts.sendPayload(msg))
    {
      Serial.println(F("Message sent !"));
    }
    else
    {
      Serial.println(F("Message not sent !"));
    }
    delay(6000);
  }

  // End of tests
  while (1);
}

/*
test
Demo sketch for Radiocrafts library :)
Radiocrafts.echoOn

Entering command mode (expecting '>')
Radiocrafts.sendCommand: 00
Radiocrafts.sendCommand response:
Radiocrafts.enterCommandMode: OK

Disable emulation mode
Radiocrafts.sendCommand: 4d2800
Radiocrafts.sendCommand response:
Radiocrafts.sendCommand: ff
Radiocrafts.sendCommand response:

Emulator Enabled (expecting 0) =
Radiocrafts.getParameter: address=40
Radiocrafts.sendCommand: 5928
Radiocrafts.sendCommand response:
Radiocrafts.getParameter: address=40 returned

Network Mode (expecting 0) =
Radiocrafts.getParameter: address=59
Radiocrafts.sendCommand: 593B
Radiocrafts.sendCommand response:
Radiocrafts.getParameter: address=59 returned

Baud Rate (expecting 5) =
Radiocrafts.getParameter: address=48
Radiocrafts.sendCommand: 5930
Radiocrafts.sendCommand response:
Radiocrafts.getParameter: address=48 returned

Setting frequency
Radiocrafts.setFrequencySG
Radiocrafts.sendCommand: 4d000300
Radiocrafts.sendCommand response:
Radiocrafts.sendCommand: ff
Radiocrafts.sendCommand response:
Set frequency result =

Getting frequency
Radiocrafts.sendCommand: 5900
Radiocrafts.sendCommand response:
Frequency (expecting 3) =

Getting temperature
Radiocrafts.sendCommand: 55
Radiocrafts.sendCommand response:
Radiocrafts.getTemperature: returned 0
Temperature = 0 C

Getting voltage
Radiocrafts.sendCommand: 56
Radiocrafts.sendCommand response:
Radiocrafts.getVoltage: returned 0.000000
Supply voltage = 0.000000 V

Getting ID
Radiocrafts.sendCommand: 39
Radiocrafts.sendCommand response:
Radiocrafts.getID: returned

4 bytes ID (LSB first) and 8 bytes PAC (MSB first) =

Getting hardware
Radiocrafts.getHardware: ERROR - Not implemented
Hardware version = TODO

Getting firmware
Radiocrafts.getFirmware: ERROR - Not implemented
Firmware version = TODO

Getting power
Radiocrafts.getParameter: address=1
Radiocrafts.sendCommand: 5901
Radiocrafts.sendCommand response:
Radiocrafts.getParameter: address=1 returned
Radiocrafts.getPower: returned 0
Power level = 0 dB
Radiocrafts.sendCommand: 58
Radiocrafts.sendCommand response:
Radiocrafts.exitCommandMode: OK

Sending payload
Radiocrafts.sendPayload: 000000000000
Radiocrafts.sendCommand: 0C000000000000
Radiocrafts.sendCommand response:

Message sent !

Sending payload
Radiocrafts.sendPayload: 000000000000
Warning: Should wait 10 mins before sending the next message
Radiocrafts.sendCommand: 0C000000000000
Radiocrafts.sendCommand response:

Message sent !

Sending payload
Radiocrafts.sendPayload: 000000000000
Warning: Should wait 10 mins before sending the next message
Radiocrafts.sendCommand: 0C000000000000
Radiocrafts.sendCommand response:

Message sent !

Sending payload
Radiocrafts.sendPayload: 000000000000
Warning: Should wait 10 mins before sending the next message
Radiocrafts.sendCommand: 0C000000000000
Radiocrafts.sendCommand response:

Message sent !

Sending payload
Radiocrafts.sendPayload: 000000000000
Warning: Should wait 10 mins before sending the next message
Radiocrafts.sendCommand: 0C000000000000
Radiocrafts.sendCommand response:

Message sent !

Sending payload
Radiocrafts.sendPayload: 000000000000
Warning: Should wait 10 mins before sending the next message
Radiocrafts.sendCommand: 0C000000000000
Radiocrafts.sendCommand response:

Message sent !

Sending payload
Radiocrafts.sendPayload: 000000000000
Warning: Should wait 10 mins before sending the next message
Radiocrafts.sendCommand: 0C000000000000
Radiocrafts.sendCommand response:

Message sent !
*/
