#include <Arduino.h>
#include "MIDI_VS1053.h"

VS1053_MIDI midi;
Song song(midi);

void setup() {
    Serial.begin(115200);
    midi.begin();
    midi.setDebug(true);

    // --- Effects and stereo setup ---
    midi.setReverb(90);        // strong but smooth reverb
    midi.setBassBoost(15);     // bass enhancement
    midi.setMasterVolume(100); // maximum volume without distortion

    // Panorama: left–right–center for enhanced stereo imaging
    midi.setPan(0, 32);   // Celesta melody — slightly left
    midi.setPan(1, 96);   // Pad chords — slightly right
    midi.setPan(2, 64);   // Bass — center
    midi.setPan(3, 110);  // Tubular Bells — right
    midi.setPan(4, 40);   // Shakuhachi — left

    // --- Main melody (Celesta + Glockenspiel) ---
    auto mainMelody = song.track(0)
        .instrument(Instrument::Celesta)
        .note("C5", 400).note("D5", 400).note("E5", 400).note("F5", 800)
        .note("G5", 400).note("A5", 400).note("G5", 800)
        .note("F5", 400).note("E5", 400).note("D5", 800)
        .rest(400)
        .note("E5", 400).note("F5", 400).note("G5", 800)
        .note("A5", 400).note("G5", 400).note("F5", 800)
        .rest(400)
        .instrument(Instrument::Glockenspiel)
        .note("C6", 300).note("E6", 300).note("G6", 600)
        .rest(200)
        .note("C6", 300).note("E6", 300).note("G6", 600)
        .rest(200);

    // --- Atmospheric chords (Pad1NewAge) ---
    auto atmosphere = song.track(1)
        .instrument(Instrument::Pad1NewAge)
        .chord({"C4","E4","G4"},2000)
        .chord({"D4","F4","A4"},2000)
        .chord({"E4","G4","B4"},2000)
        .chord({"F4","A4","C5"},2000)
        .chord({"C4","E4","G4"},2000)
        .chord({"G4","B4","D5"},2000)
        .chord({"F4","A4","C5"},2000)
        .chord({"E4","G4","B4"},2000);

    // --- Bass (SynthBass1) ---
    auto bass = song.track(2)
        .instrument(Instrument::SynthBass1)
        .note("C3",2000).note("D3",2000).note("E3",2000).note("F3",2000)
        .note("C3",2000).note("G3",2000).note("F3",2000).note("E3",2000);

    // --- Bells (TubularBells) ---
    auto bells = song.track(3)
        .instrument(Instrument::TubularBells)
        .rest(4000)
        .note("C5",800).rest(400)
        .note("E5",800).rest(400)
        .note("G5",800).rest(400)
        .note("C6",800).rest(400)
        .note("G5",800).rest(400)
        .note("E5",800).rest(400)
        .note("C5",800).rest(400);

    // --- Ethnic motif (Shakuhachi) ---
    auto ethnic = song.track(4)
        .instrument(Instrument::Shakuhachi)
        .rest(6000)
        .note("A4",1200).note("G4",800).note("F4",800).note("E4",1200)
        .note("D4",800).note("C4",800)
        .rest(800);

    // --- Start playback ---
    song.play(true); // loop the track
}

void loop() {
    midi.update(); // continuous VS1053 engine update
}
