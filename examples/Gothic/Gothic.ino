#include <Arduino.h>
#include "MIDI_VS1053.h"

// VS1053 MIDI wrapper instance and song container
VS1053_MIDI midi;
Song song(midi);

void setup() {
    Serial.begin(115200);
    midi.begin();
    midi.setDebug(true);

    // --- Effects and stereo setup ---
    midi.setReverb(90);        // strong but smooth reverb
    midi.setBassBoost(15);     // slightly more bass for gothic warmth
    midi.setMasterVolume(100); // full volume without clipping

    // Super-stereo panorama: left-right-center spacing for lush stereo field
    // track 0 — lead / vocal-like
    midi.setPan(0, 36);   // slightly left
    // track 1 — pads / choir
    midi.setPan(1, 100);  // slightly right
    // track 2 — bass
    midi.setPan(2, 64);   // center
    // track 3 — bells / accent
    midi.setPan(3, 112);  // right
    // track 4 — ethnic / flute / atmosphere
    midi.setPan(4, 48);   // left
    // optional additional ambient FX track
    midi.setPan(5, 80);   // right-of-center

    // --- Composition: Gothic motif (original) ---
    // Tempo note: the durations below are milliseconds (used directly by this simple API).
    // Main melodic/timbral choices:
    //  - lead: ChoirAahs / VoiceOohs / Celesta (vocal-ish, haunting)
    //  - pads: Pad4Choir / Pad1NewAge (wide, sustaining)
    //  - bass: SynthBass1 (warm, deep)
    //  - bells: TubularBells (cold accents)
    //  - ethnic: Shakuhachi (wind-like, distant)
    //  - extra strings: StringEnsemble1 (sustained cinematic support)

    // --- Main melody (vocal-style lead) ---
    auto mainMelody = song.track(0)
        .instrument(Instrument::ChoirAahs) // haunting vocal pad used as a lead
        // Phrase 1
        .note("D5", 600).note("F5", 300).note("E5", 300).note("D5", 600)
        .note("A4", 400).note("C5", 400).note("D5", 800)
        // Phrase 2
        .note("F5", 600).note("G5", 300).note("F5", 300).note("E5", 600)
        .note("D5", 400).note("C5", 400).note("A4", 800)
        // Phrase 3
        .note("A#4", 600).note("D5", 300).note("C5", 300).note("A4", 600)
        .note("F4", 400).note("G4", 400).note("A4", 800)
        // Long emotional sustain to close the phrase
        .note("D5", 1000);

    // --- Atmospheric pads / strings (wide choir + strings) ---
    auto atmosphere = song.track(1)
        .instrument(Instrument::Pad4Choir)
        .chord({"D3","A3","D4","F4"}, 4000) // Dm
        .chord({"A2","E3","A3","C#4"}, 4000) // A major (as V)
        .chord({"A#2","F3","A#3","D4"}, 4000) // Bb
        .chord({"F2","C3","F3","A3"}, 4000)  // F
        .chord({"D3","A3","D4","F4"}, 4000)  // Dm repeat
        .chord({"G3","B3","D4","G4"}, 4000)  // G (color)
        .chord({"F2","A2","C3","F3"}, 4000)  // F
        .chord({"A#2","F3","A#3","D4"}, 4000); // Bb

    // Add a soft string bed on a separate track to enrich the middle
    auto strings = song.track(5)
        .instrument(Instrument::StringEnsemble1)
        .rest(0)
        .chord({"D3","F3","A3","D4"}, 4000)
        .chord({"A2","C#3","E3","A3"}, 4000)
        .chord({"A#2","D3","F3","A#3"}, 4000)
        .chord({"F2","A2","C3","F3"}, 4000)
        .chord({"D3","F3","A3","D4"}, 4000)
        .chord({"G2","B2","D3","G3"}, 4000)
        .chord({"F2","A2","C3","F3"}, 4000)
        .chord({"A#2","D3","F3","A#3"}, 4000);

    // --- Bass (warm synth) ---
    auto bass = song.track(2)
        .instrument(Instrument::SynthBass1)
        .note("D2", 2000).note("A1", 2000)
        .note("A#1", 2000).note("F1", 2000)
        .note("D2", 2000).note("C2", 2000)
        .note("A#1",2000).note("A1",2000);

    // --- Bells / accent hits (tubular bells) ---
    auto bells = song.track(3)
        .instrument(Instrument::TubularBells)
        .rest(8000) // let the intro breathe
        .note("D4", 1000).rest(600)
        .note("A3", 800).rest(800)
        .note("F4", 1000).rest(1200)
        // Add repeating bell motif as a counter-accent
        .note("D4", 800).rest(800)
        .note("A3", 800).rest(800);

    // --- Ethnic / wind motif (Shakuhachi) ---
    auto ethnic = song.track(4)
        .instrument(Instrument::Shakuhachi)
        .rest(12000)
        .note("A4", 1000).note("G4", 600).note("F4", 600)
        .note("E4", 1200).note("D4", 600).note("C4", 600)
        .rest(1600);

    // --- Optional small lead arpeggio (Glockenspiel/Celesta) for shimmer ---
    auto shimmer = song.track(6)
        .instrument(Instrument::Celesta)
        .rest(2000)
        .note("F5", 300).note("E5", 300).note("D5", 600)
        .rest(400)
        .note("C6", 300).note("E6", 300).note("G6", 600)
        .rest(600)
        .note("F5",300).note("E5",300).note("D5",600);

    // --- Start playback in loop mode (true -> loop) ---
    song.play(true);
}

void loop() {
    // Continuous update of the VS1053 MIDI engine
    midi.update();
}
