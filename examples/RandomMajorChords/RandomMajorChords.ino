/*
  RandomMajorChords.ino
  ---------------------
  Play random major chords using VS1053 in MIDI mode.
*/

#include <Arduino.h>
#include <MIDI_VS1053.h>

// Create the VS1053 MIDI controller object
VS1053_MIDI midi;

// Time of the last chord played (used to space chords)
unsigned long lastTime = 0;

// There are 3 channels used: 0, 1, 2
// We track what instrument is currently set on each channel so we
// don't call setInstrument() repeatedly and cause duplicated notes.
//
// Initialize to 255 which is an impossible instrument number (valid are 0..127).
uint8_t currentInstrument[3] = {255, 255, 255};


// ------------------------------------------------------------
// playChord()
// Plays a major triad (root, root + 4, root + 7) using three channels.
// - rootNote: MIDI note number for the chord root (0..127)
// - inst: instrument number (General MIDI 0..127)
// - durationMs: how long each note should sound in milliseconds
//
// This function:
//  1. Ensures each channel has the correct instrument set, but only
//     calls setInstrument() if the instrument changed since last time.
//  2. Sets stereo pan for each channel so the chord is spread across
//     left -> center -> right.
//  3. Calls playNoteAsync() to play notes in a non-blocking way.
// ------------------------------------------------------------
void playChord(uint8_t rootNote, Instrument inst, uint32_t durationMs) {
  // We will use three channels for the chord: left, center, right
  uint8_t channels[3] = {0, 1, 2};

  // Pan values in MIDI range 0..127. 32 = left, 64 = center, 96 = right.
  uint8_t pans[3] = {32, 64, 96};

  // Intervals for a major triad (in semitones): root, major third, perfect fifth
  int intervals[3] = {0, 4, 7};

  for (int i = 0; i < 3; i++) {
    uint8_t ch = channels[i];

    // Only call setInstrument() if the requested instrument is different
    // from what we last set on this channel. This avoids duplicate calls.
    if (currentInstrument[i] != (uint8_t)inst) {
      // Debug print so the user can see when we actually change instrument
      Serial.printf("[DEBUG] setInstrument called for channel %d -> instrument %d\n", ch, (int)inst);

      midi.setInstrument(ch, inst);       // Set instrument on this channel
      currentInstrument[i] = (uint8_t)inst; // Remember instrument for this channel
    }

    // Set stereo pan for this channel (left/center/right)
    midi.setPan(ch, pans[i]);

    // Play the note asynchronously (non-blocking).
    // Note: we still pass the instrument to playNoteAsync() because
    // some library versions require it. If your library has an overload
    // without instrument, you could use that instead.
    uint8_t note = (uint8_t)(rootNote + intervals[i]);
    midi.playNoteAsync(ch, inst, (Note)note, durationMs);
  }
}


// ------------------------------------------------------------
// setup()
// Initialization: serial + MIDI begin + some simple audio settings
// ------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for Serial to be ready if board requires it (harmless if not)
  }

  // Initialize the MIDI interface for VS1053
  midi.begin();

  // Enable debug output from the library (useful while developing)
  midi.setDebug(true);

  // Global sound settings (tweak as you like)
  // Master volume typically 0..127 (smaller number = softer)
  midi.setMasterVolume(100);

  // Simple effects - reverb and bass boost
  midi.setReverb(80);    // Adds ambient reverb effect
  midi.setBassBoost(10); // Adds a bit of low-frequency punch
}


// ------------------------------------------------------------
// loop()
// Main loop that periodically chooses a random chord and plays it.
// A new chord is played every 5000 ms (5 seconds).
// ------------------------------------------------------------
void loop() {
  // Always call update() to let the MIDI library handle async tasks
  midi.update();

  // Check if we should play a new chord
  if (millis() - lastTime >= 5000) {
    lastTime = millis();

    // -------------------------
    // Step 1: Choose a random octave base
    // These numbers are MIDI note numbers for C notes:
    // C1 = 24, C2 = 36, C3 = 48, C4 = 60, C5 = 72
    // Choose one randomly to vary pitch range.
    // -------------------------
    int octaves[] = {24, 36, 48, 60, 72};
    int octaveBase = octaves[random(0, 5)]; // random index 0..4

    // -------------------------
    // Step 2: Choose a random white-key offset within the octave
    // White keys in semitone offsets: 0 (C), 2 (D), 4 (E), 5 (F), 7 (G), 9 (A), 11 (B)
    // This keeps melodies on "white keys" (no sharps/flats), which sounds musical.
    // -------------------------
    int offsets[] = {0, 2, 4, 5, 7, 9, 11};
    int root = octaveBase + offsets[random(0, 7)];

    // -------------------------
    // Step 3: Choose a random General MIDI instrument (0..127)
    // -------------------------
    Instrument inst = (Instrument)random(0, 128);

    // -------------------------
    // Step 4: Play the chord for 1200 milliseconds (1.2 seconds)
    // -------------------------
    playChord((uint8_t)root, inst, 1200);

    // Print a simple message to the Serial Monitor so user can see what played
    Serial.println("========================================");
    Serial.printf(" New chord:\n   Instrument: %d\n   Root note: %d\n", (int)inst, root);
    Serial.println("========================================");
  }
}
