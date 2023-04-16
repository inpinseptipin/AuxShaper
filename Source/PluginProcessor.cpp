/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AuxShaperAudioProcessor::AuxShaperAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),parameterMap(this),bezier(4096,AuxPort::Bezier::FourthOrder)
#endif
{
    xParam1 = new juce::AudioParameterFloat("x1", "x1", 0, 1, 0.5);
    yParam1 = new juce::AudioParameterFloat("y1", "y1", 0, 1, 0.5);
    slopeParam = new juce::AudioParameterFloat("slope", "slope", 0, 1, 0);
    parameterMap.addParameter(xParam1);
    parameterMap.addParameter(yParam1);
    parameterMap.addParameter(slopeParam);
    parameterMap.save();
}

AuxShaperAudioProcessor::~AuxShaperAudioProcessor()
{
}

//==============================================================================
const juce::String AuxShaperAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AuxShaperAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AuxShaperAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AuxShaperAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AuxShaperAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AuxShaperAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AuxShaperAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AuxShaperAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AuxShaperAudioProcessor::getProgramName (int index)
{
    return {};
}

void AuxShaperAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AuxShaperAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    if ((*xParam1) != midPoint.x)
        midPoint.setX(*xParam1);
    if ((*yParam1) != midPoint.y)
        midPoint.setY(juce::jmap<float>(*yParam1, -10, 10));
    if (slopeFactor != *slopeParam)
        slopeFactor = *slopeParam;



    startPoint.setXY(0, -0);
    endPoint.setXY(1, 1);
    midNegPoint.setXY((1 - slopeFactor) * midPoint.getX(), (1 - slopeFactor) * midPoint.getY());
    midPosPoint.setXY((1 + slopeFactor) * midPoint.getX(), (1 + slopeFactor) * midPoint.getY());
}

void AuxShaperAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AuxShaperAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AuxShaperAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    if ((*xParam1) != midPoint.x)
        midPoint.setX(*xParam1);
    if ((*yParam1) != midPoint.y)
        midPoint.setY(*yParam1);
    if (slopeFactor != *slopeParam)
        slopeFactor = *slopeParam;


    midNegPoint.setXY((1 - slopeFactor) * midPoint.getX(), (1 - slopeFactor) * midPoint.getY());
    midPosPoint.setXY((1 + slopeFactor) * midPoint.getX(), (1 + slopeFactor) * midPoint.getY());

    bezier.setPoint(startPoint, 0);
    bezier.setPoint(midNegPoint, 1);
    bezier.setPoint(midPoint, 2);
    bezier.setPoint(midPosPoint, 3);
    bezier.setPoint(endPoint, 4);

    bezier.calcPoints();
    bezier.drawWaveshaper();


    //engine.process(buffer);

    waveshaper.process(buffer, bezier);
}

//==============================================================================
bool AuxShaperAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AuxShaperAudioProcessor::createEditor()
{
    return new AuxShaperAudioProcessorEditor (*this);
}

//==============================================================================
void AuxShaperAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AuxShaperAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AuxShaperAudioProcessor();
}
