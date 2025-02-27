#ifndef AUXPORT_WAVESHAPE_H
#define AUXPORT_WAVESHAPE_H
/*
  ==============================================================================

    AuxWaveShape.h
    Created: 29 Mar 2023 12:31:24pm
    Author:  satya

  ==============================================================================
*/
#include "AuxBezier.h"
#include "AuxWaveShape.h"
#include <juce_audio_basics/juce_audio_basics.h>
namespace AuxPort
{
    class Auxshape
    {
    public:
        Auxshape() = default;
        ~Auxshape() = default;
        Auxshape(const Auxshape& shape) = default;
        void process(juce::AudioBuffer<float>& buffer, AuxPort::Bezier& bezier);
    private:
        float output = 0;
    };

    
}

#endif