/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AuxShaperAudioProcessorEditor::AuxShaperAudioProcessorEditor (AuxShaperAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),auxShaper(&audioProcessor.parameterMap)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (300, 400);
    addAndMakeVisible(auxShaper);

}

AuxShaperAudioProcessorEditor::~AuxShaperAudioProcessorEditor()
{
}

//==============================================================================
void AuxShaperAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    auto x = this->getLocalBounds().getWidth() / 10;
    auto y = this->getLocalBounds().getHeight() / 10;

    g.fillAll(juce::Colour::fromRGBA(11, 12, 13, 150));
    g.setFont(juce::Font("Papyrus", 36, 1));
    g.setColour(juce::Colours::silver);
    g.drawText("AuxShaper", 0, 9 * y, 5 * x, y, juce::Justification::horizontallyCentred);
}

void AuxShaperAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto x = this->getWidth() / 10;
    auto y = this->getHeight() / 10;
    auxShaper.setBounds(0, 0, 10 * x, 9 * y);
}
