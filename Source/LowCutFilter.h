#pragma once

// Credits to Jan Wilczek of WolfSound
// https://thewolfsound.com/lowpass-highpass-filter-plugin-with-juce/
// https://youtu.be/xJCt0MVZZ7Q

#include <vector>
#include "JuceHeader.h"
#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>

class LowCutFilter
{
public:
    LowCutFilter() = default;

    void setCutOffFrequency(float fc)
    {
        this->cutOffFrequency = fc;
    }

    void setSamplingRate(float sr)
    {
        this->samplingRate = sr;
    }

    void setQFactor(float qf)
    {
    }

    void process(juce::AudioBuffer<float>& buffer)
    {
        constexpr auto PI = 3.14159265359f;

        dnBuffer.resize(buffer.getNumChannels(), 0.f); // resize the allpass buffers to the number of channels and zero the new ones

        const auto sign = -1.f; // highPass

        const auto tan = std::tan(PI * cutOffFrequency / samplingRate); // allpass coefficient is constant while processing a block of samples
        const auto a1 = (tan - 1.f) / (tan + 1.f);

        for (auto channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto channelSamples = buffer.getWritePointer(channel);
            for (auto i = 0; i < buffer.getNumSamples(); ++i)
            {
                const auto inputSample = channelSamples[i];

                const auto allpassFilteredSample = a1 * inputSample + dnBuffer[channel];
                dnBuffer[channel] = inputSample - a1 * allpassFilteredSample;

                const auto filterOutput = 0.5f * (inputSample + sign * allpassFilteredSample);

                channelSamples[i] = filterOutput;
            }
        }
    }

private:
    float cutOffFrequency;
    float samplingRate;
    std::vector<float> dnBuffer;
};