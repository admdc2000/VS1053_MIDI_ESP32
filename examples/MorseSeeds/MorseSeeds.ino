#include <Arduino.h>
#include "pins.h"
#include "MIDI_VS1053.h"
#include "bip39_words.h"  // Header file with BIP39 words

// Create MIDI object
VS1053_MIDI midi;

// Morse settings
int charsPerMinute = 15;  // Speed: 15 characters per minute (CPM)
int midiNote = 76;         // Note: E5
Instrument midiInstrument = Instrument::ElectricGuitarClean;

int dotDuration;    // Dot duration (1 time unit)
int dashDuration;   // Dash duration (3 time units)
int symbolSpace;    // Space between symbol elements (1 time unit)
int letterSpace;    // Space between letters (3 time units)
int wordSpace;      // Space between words (7 time units)

// Morse code encoding (A-Z)
const char* morseCodes[26] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
    ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
    "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."
};

void calculateDurations() {
    // Dot duration = 1200 ms / CPM
    dotDuration = 1200 / charsPerMinute;
    dashDuration = 3 * dotDuration;  // Dash is 3 times longer than dot
    symbolSpace = dotDuration;       // Space between symbol elements
    letterSpace = 3 * dotDuration;   // Space between letters
    wordSpace = 7 * dotDuration;     // Space between words
}

void playTone(int duration) {
    midi.noteOn(0, (Note)midiNote, 127);  // Turn on note
    delay(duration);                      // Hold tone
    midi.noteOff(0, (Note)midiNote, 127); // Turn off note
}

void playMorseSymbol(const char* symbol) {
    for (int i = 0; symbol[i] != '\0'; i++) {
        if (symbol[i] == '.') {
            playTone(dotDuration);  // Dot - short signal
        } else if (symbol[i] == '-') {
            playTone(dashDuration); // Dash - long signal
        }

        // Space between symbol elements (if there is a next element)
        if (symbol[i + 1] != '\0') {
            delay(symbolSpace);
        }
    }
}

String generateRandomWords(int wordCount) {
    String result = "";

    for (int i = 0; i < wordCount; i++) {
        int randomIndex = random(BIP39_WORD_COUNT);
        result += BIP39_WORDS[randomIndex];

        if (i < wordCount - 1) {
            result += " ";
        }
    }

    return result;
}

void playText(String text) {
    text.toUpperCase();

    for (int i = 0; i < text.length(); i++) {
        char c = text[i];

        if (c == ' ') {
            delay(wordSpace); // Space between words
            continue;
        }

        if (c >= 'A' && c <= 'Z') {
            int index = c - 'A';
            playMorseSymbol(morseCodes[index]);
            delay(letterSpace); // Space between letters
        }
    }

    // Force turn off all notes
    delay(50);
    for (int note = 0; note < 128; note++) {
        midi.noteOff(0, (Note)note, 0);
    }
    delay(50);
    midi.update();
}

void playRandomMorse(int wordCount = 3) {
    String randomText = generateRandomWords(wordCount);
    Serial.print("Playing: ");
    Serial.println(randomText);
    playText(randomText);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting Morse Code Generator with VS1053");

    // Initialize MIDI
    midi.begin();

    // Set instrument
    midi.setInstrument(0, midiInstrument);

    // Initialize random seed
    randomSeed(analogRead(0));

    // Calculate durations
    calculateDurations();

    Serial.println("Morse Generator Ready!");
    Serial.println("=== Playing random BIP39 words ===");
    Serial.print("Fixed speed: ");
    Serial.print(charsPerMinute);
    Serial.println(" CPM");
    Serial.print("Fixed note: ");
    Serial.println(midiNote);
    Serial.print("Fixed instrument: ");
    Serial.println((int)midiInstrument);
}

void loop() {
    playRandomMorse(24); // Play 24 words at a time
    delay(3000); // Pause between sequences
}
