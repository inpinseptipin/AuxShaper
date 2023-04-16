/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "AuxShaper.h"

//==============================================================================
/**
*/
class AuxShaperAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AuxShaperAudioProcessorEditor (AuxShaperAudioProcessor&);
    ~AuxShaperAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AuxShaperAudioProcessor& audioProcessor;
    AuxShaper auxShaper;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AuxShaperAudioProcessorEditor)
};
