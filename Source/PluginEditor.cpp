/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Week3AudioProcessorEditor::Week3AudioProcessorEditor(Week3AudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(350, 250);
    levelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    levelSlider.setValue(0.5);
    levelSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
                                true,
                                levelSlider.getTextBoxWidth(),
                                levelSlider.getTextBoxHeight());
    levelSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.tree, "level", levelSlider));
    
    comboBox.addItem("sine", 1);
    comboBox.addItem("square", 2);
    comboBox.addItem("triangle", 3);
    comboBox.addItem("sawtooth", 4);
    comboBox.setSelectedId(1);

    addAndMakeVisible(levelSlider);
    addAndMakeVisible(comboBox);
}

Week3AudioProcessorEditor::~Week3AudioProcessorEditor()
{
}

//==============================================================================
void Week3AudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    int x = 50;
    int y = 50;
    int width = 50;
    int height = levelSlider.getHeight();
    
    g.drawFittedText("Level", x, y, width, height, juce::Justification::centred, 1);
    g.drawFittedText("Mode", x, y + height, width, height, juce::Justification::centred, 1);
}

void Week3AudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    int sliderWidth = area.getWidth() / 2;
    int sliderHeight = area.getHeight() / 4;
    int x = 100;
    int y = 50;
    
    levelSlider.setBounds(x, y, sliderWidth, sliderHeight);
    comboBox.setBounds(x, y + levelSlider.getHeight() + sliderHeight / 4 , sliderWidth, sliderHeight / 2);
}
