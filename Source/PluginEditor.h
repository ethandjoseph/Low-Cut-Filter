#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
//#include "HorizontalSliderComponent.h"

class LowCutAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    LowCutAudioProcessorEditor (LowCutAudioProcessor&);
    ~LowCutAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider cutOffFrequencySlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutOffFrequencySliderAttachment;
    
    juce::Label cutOffFrequencyLabel;

    LowCutAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LowCutAudioProcessorEditor)
};