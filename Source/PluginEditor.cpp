#include "PluginProcessor.h"
#include "PluginEditor.h"

LowCutAudioProcessorEditor::LowCutAudioProcessorEditor (LowCutAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    cutOffFrequencySlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    addAndMakeVisible(cutOffFrequencySlider);

    cutOffFrequencySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CUT_OFF_FREQUENCY", cutOffFrequencySlider);

    cutOffFrequencyLabel.setText("Cut Off Frequency", juce::dontSendNotification);
    addAndMakeVisible(cutOffFrequencyLabel);

    setSize (352, 104);
}

LowCutAudioProcessorEditor::~LowCutAudioProcessorEditor()
{
}

void LowCutAudioProcessorEditor::paint (juce::Graphics& g)
{
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::black);
}

void LowCutAudioProcessorEditor::resized()
{
    cutOffFrequencyLabel.setBounds(16, 16, 320, 28);
    cutOffFrequencySlider.setBounds(16, 52, 320, 36);
}