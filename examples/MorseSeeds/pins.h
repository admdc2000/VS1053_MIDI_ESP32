#pragma once


/*
pins.h


Pin assignments and wiring notes for connecting a VS1053/VS1053B breakout
module to an ESP32 (esp32dev). This header is intentionally small and
documented so users can change pin numbers to match their board.


Wiring / Notes:
- VS1053 has two chip-select like pins: SCI (control) and SDI/SDI (data). We
name them VS1053_CS (SCI / control) and VS1053_DCS (SDI / data). DREQ is a
data request (output from VS1053) and should be connected to an ESP32
input pin that supports digitalRead.
- MOSI/MISO/SCK must be connected to ESP32's SPI pins defined below or use
VSPI/HSPI pins (this library calls SPI.begin(SCK, MISO, MOSI)).
- Reset pin is optional on some breakouts; if not available, undefine
VS1053_RESET in the project to skip the hardware reset sequence.


Default mapping below follows a common wiring for ESP32 dev boards and
VS1053 breakout modules. You may change these values before including the
main library if your wiring differs.
*/


// Control (SCI) chip select (pull LOW to send control commands)
#ifndef VS1053_CS
#define VS1053_CS 2
#endif


// Data (SDI) chip select (pull LOW to send audio/MIDI bytes)
#ifndef VS1053_DCS
#define VS1053_DCS 4
#endif


// DREQ: Data request output from VS1053 (HIGH when ready to receive data)
#ifndef VS1053_DREQ
#define VS1053_DREQ 36
#endif


// SPI pins (MOSi, MISO, SCLK). The library calls SPI.begin(SCK, MISO, MOSI)
#ifndef VS1053_MOSI
#define VS1053_MOSI 23
#endif
#ifndef VS1053_MISO
#define VS1053_MISO 19
#endif
#ifndef VS1053_SCK
#define VS1053_SCK 18
#endif


// Optional hardware reset pin. If your breakout does not provide a reset pin
// or you prefer not to drive it from the ESP32, comment out the definition.
#ifndef VS1053_RESET
#define VS1053_RESET 5
#endif


// Recommendation summary for wiring (example):
// VS1053 -> ESP32
// CS (SCI) -> VS1053_CS (GPIO 2)
// DCS (SDI)-> VS1053_DCS (GPIO 4)
// DREQ -> VS1053_DREQ (GPIO 36)
// MOSI -> VS1053_MOSI (GPIO 23)
// MISO -> VS1053_MISO (GPIO 19)
// SCLK -> VS1053_SCK (GPIO 18)
// RESET -> VS1053_RESET (GPIO 5) (optional)
