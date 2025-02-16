/*
  ==============================================================================

    AuxWaveShape.cpp
    Created: 29 Mar 2023 12:31:24pm
    Author:  satya

  ==============================================================================
*/

#include "AuxWaveShape.h"

void AuxPort::Auxshape::process(juce::AudioBuffer<float>& buffer, AuxPort::Bezier& bezier)
{

    for (uint32_t i = 0; i < buffer.getNumChannels(); i++)
    {
        for (uint32_t j = 0; j < buffer.getNumSamples(); j++)
        {
            float sample = buffer.getSample(i, j);
            uint32_t index = bezier.search(buffer.getSample(i,j));
            
            if (buffer.getSample(i,j) == bezier.getPoint(index, AuxPort::Bezier::PointType::WaveshapeVector)->getX())
                output = bezier.getPoint(index, AuxPort::Bezier::PointType::WaveshapeVector)->getY();
            else
                if (index >= 2 * bezier.getSize() - 1)
                    output =  bezier.getPoint(2 * bezier.getSize() - 1, AuxPort::Bezier::PointType::WaveshapeVector)->getY();
                else if (index < 0)
                    output = bezier.getPoint(0, AuxPort::Bezier::PointType::WaveshapeVector)->getY();
                else
                    output = (bezier.getPoint(index, AuxPort::Bezier::PointType::WaveshapeVector)->getY() + bezier.getPoint(index + 1, AuxPort::Bezier::PointType::WaveshapeVector)->getY()) / 2;

            buffer.setSample(i,j, output);
        }
    }
}


