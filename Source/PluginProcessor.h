/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AuxIIR.h"
#include "AuxParam.h"
#include "AuxBezier.h"
#include "AuxWaveShape.h"
//==============================================================================
/**
*/
class AuxShaperAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    AuxShaperAudioProcessor();
    ~AuxShaperAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    AuxPort::ParameterMap parameterMap;
private:
    //==============================================================================
    juce::AudioParameterFloat* yParam1;
    juce::AudioParameterFloat* xParam1;
    juce::AudioParameterFloat* slopeParam;
    float slopeFactor;

    float sample;

    juce::Point<float> startPoint;
    juce::Point<float> endPoint;
    juce::Point<float> midPoint;
    juce::Point<float> midNegPoint;
    juce::Point<float> midPosPoint;


    AuxPort::IIR::Engine engine;
    AuxPort::Bezier bezier;
    AuxPort::Auxshape waveshaper;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AuxShaperAudioProcessor)
};
