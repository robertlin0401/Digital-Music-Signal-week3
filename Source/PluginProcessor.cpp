/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Week3AudioProcessor::Week3AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor(BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
     tree(*this, nullptr, "PARAM", {
          std::make_unique<juce::AudioParameterFloat>("level",
                                                      "Level",
                                                      juce::NormalisableRange<float>(0.0f, 1.0f, 0.1f), 0.5f,
                                                      juce::String(),
                                                      juce::AudioProcessorParameter::genericParameter,
                                                      [](float value, int){ return juce::String(value); },
                                                      [](juce::String text){ return text.getFloatValue(); }),
          std::make_unique<juce::AudioParameterChoice>("mode",
                                                       "Mode",
                                                       juce::StringArray({ "sine", "square", "triangle", "sawtooth" }), 0)
          })
#endif
{
    mySynth.clearSounds();
    mySynth.addSound(new SynthSound());
    
    mySynth.clearVoices();
    for (int i = 0; i < 5; i++)
        mySynth.addVoice(new SynthVoice());
}

Week3AudioProcessor::~Week3AudioProcessor()
{
}

//==============================================================================
const juce::String Week3AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Week3AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Week3AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Week3AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Week3AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Week3AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Week3AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Week3AudioProcessor::setCurrentProgram(int index)
{
}

const juce::String Week3AudioProcessor::getProgramName(int index)
{
    return {};
}

void Week3AudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void Week3AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    lastSampleRate = sampleRate;
    mySynth.setCurrentPlaybackSampleRate(sampleRate);
}

void Week3AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Week3AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Week3AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int i = 0; i < mySynth.getNumVoices(); i++) {
        auto* myVoice = dynamic_cast<SynthVoice*>(mySynth.getVoice(i));
        myVoice->setLevel(tree.getRawParameterValue("level")->load());
        myVoice->setMode(tree.getRawParameterValue("mode")->load());
    }
    mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool Week3AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Week3AudioProcessor::createEditor()
{
    return new Week3AudioProcessorEditor(*this);
}

//==============================================================================
void Week3AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Week3AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Week3AudioProcessor();
}
