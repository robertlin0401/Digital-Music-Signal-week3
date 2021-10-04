/*
  ==============================================================================

    SynthVoice.cpp
    Created: 4 Oct 2021 1:57:03am
    Author:  robert

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    noteMidiNumber = midiNoteNumber;
    frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    currentAngle = 0.f;
    angleIncrement = frequency / getSampleRate() * juce::MathConstants<float>::twoPi;
    tailOff = 0.0;
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    if (allowTailOff) {
        if (tailOff == 0.0)
            tailOff = 1.0;
    } else {
        clearCurrentNote();
        level = 0;
        currentAngle = 0.f;
    }
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
    // handle pitch wheel moved midi event
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
    // handle midi control change
}

void SynthVoice::renderNextBlock(juce::AudioBuffer <float> &outputBuffer, int startSample, int numSamples)
{
    if (tailOff > 0.0) {
        for (int i = startSample; i < (startSample + numSamples); i++) {
            float value = std::sin(currentAngle) * level * tailOff;
            outputBuffer.addSample(0, i, value);
            outputBuffer.addSample(1, i, value);
            
            currentAngle += angleIncrement;
            tailOff *= 0.99;
            
            if (tailOff <= 0.05) {
                clearCurrentNote();
                angleIncrement = 0.0;
                level = 0.0;
                break;
            }
        }
    } else {
        for (int i = startSample; i < (startSample + numSamples); i++) {
            float value = std::sin(currentAngle) * level;
            outputBuffer.addSample(0, i, value);
            outputBuffer.addSample(1, i, value);
            
            currentAngle += angleIncrement;
        }
    }
}

void SynthVoice::setLevel(float newLevel)
{
    level = newLevel;
}
