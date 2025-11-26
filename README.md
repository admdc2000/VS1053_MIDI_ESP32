# VS1053_MIDI_ESP32

**Lightweight, header-only library for realtime MIDI playback and non-blocking multi-track sequencing on ESP32 using the VS1053B chip.**

---

## 🎯 Overview

**VS1053_MIDI_ESP32** is a compact library for **ESP32 (Arduino framework)** that enables:
- **Realtime MIDI plugin loading** for the **VS1053B** chip.
- **Non-blocking multi-track sequencer** with per-event control over instrument, velocity, and duration.
- **Convenience APIs** for composing songs and patterns in code.
- **Optimized instrument handling** to avoid duplicate Program Change messages.
- **Helpers** for pan, reverb, and volume control.

---

## 🔌 Installation

### 1. Hardware Requirements
- **ESP32** (tested on `esp32dev`).
- **VS1053B** breakout board.
- **SPI connection** between ESP32 and VS1053B.

### 2. Software Setup
1. **Download the library**:
   ```bash
   git clone https://github.com/admdc2000/VS1053_MIDI_ESP32.git
   ```
2. Place the library in your Arduino `libraries` folder.
3. Include the header in your sketch:
   ```cpp
   #include "MIDI_VS1053.h"
   ```

### 3. Pin Configuration
The library uses `pins.h` for pin assignments. Default pins:
```cpp
#define VS1053_CS     2   // Control (SCI) chip select
#define VS1053_DCS    4   // Data (SDI) chip select
#define VS1053_DREQ  36   // Data request
#define VS1053_MOSI  23   // SPI MOSI
#define VS1053_MISO  19   // SPI MISO
#define VS1053_SCK   18   // SPI SCLK
#define VS1053_RESET 5    // Optional reset pin
```
**Note:** You can override these pins by defining them before including `MIDI_VS1053.h`.

---

## 🚀 Quick Start

### Example: Play a Random Major Chord
```cpp
#include <Arduino.h>
#include "MIDI_VS1053.h"

VS1053_MIDI midi;

void setup() {
  Serial.begin(115200);
  midi.begin();
  midi.setDebug(true);
  midi.setMasterVolume(100);
  midi.setReverb(80);
}

void loop() {
  midi.update();
  static unsigned long lastTime = 0;
  if (millis() - lastTime >= 5000) {
    lastTime = millis();
    int root = random(24, 72); // Random root note (C1 to C5)
    Instrument inst = (Instrument)random(0, 128);
    midi.playNoteAsync(0, inst, (Note)root, 1200);
    midi.playNoteAsync(1, inst, (Note)(root + 4), 1200);
    midi.playNoteAsync(2, inst, (Note)(root + 7), 1200);
  }
}
```

---

## 🎵 Composer API Example
```cpp
#include "MIDI_VS1053.h"

VS1053_MIDI midi;
Song song(midi);

void setup() {
  midi.begin();
  song.track(0)
    .instrument(Instrument::AcousticGrandPiano)
    .note("C4", 500)
    .note("E4", 500)
    .note("G4", 500)
    .note("C5", 1000);
  song.play();
}

void loop() {
  midi.update();
}
```

---

## 📖 API Reference

### Core Class: `VS1053_MIDI`
| Method | Description |
|--------|-------------|
| `begin()` | Initialize SPI and load the MIDI plugin. |
| `setInstrument(channel, inst)` | Set instrument for a MIDI channel. |
| `noteOn(channel, note, vel)` | Send a Note On message. |
| `noteOff(channel, note, vel)` | Send a Note Off message. |
| `playNoteAsync(channel, inst, note, durationMs, vel)` | Play a note and schedule its Note Off. |
| `addEvent(track, timeOffsetMs, channel, inst, note, vel, durationMs)` | Add an event to a sequencer track. |
| `startSequencer(loopMs)` | Start the sequencer. |
| `stopSequencer()` | Stop the sequencer. |
| `update()` | **Must be called in `loop()`** to handle scheduling. |

### Composer Classes
- **`TrackComposer`**: Build patterns with `note()`, `chord()`, and `arp()`.
- **`Song`**: Combine tracks and play them.

---

## 📄 License
**MIT License**
Copyright (c) 2024 AdmDC

