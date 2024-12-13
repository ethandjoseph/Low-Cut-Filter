#include "PluginProcessor.h"
#include "PluginEditor.h"

LowCutAudioProcessor::LowCutAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    apvts (*this, nullptr, "Parameters", createParameterLayout())
#endif
{
}

LowCutAudioProcessor::~LowCutAudioProcessor()
{
}

const juce::String LowCutAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LowCutAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LowCutAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LowCutAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LowCutAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LowCutAudioProcessor::getNumPrograms()
{
    return 1;
}

int LowCutAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LowCutAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LowCutAudioProcessor::getProgramName (int index)
{
    return {};
}

void LowCutAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void LowCutAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lowCutFilter.setSamplingRate(static_cast<float>(sampleRate));
}

void LowCutAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LowCutAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo()
        || layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}
#endif

// Process Block ===========================================================================================//
void LowCutAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    float cutOffFrequency = apvts.getRawParameterValue("CUT_OFF_FREQUENCY")->load(); // in C++, load() => std::atomic<T> to T conversion
    //std::cout << cutOffFrequency << std::endl;
    lowCutFilter.setCutOffFrequency(cutOffFrequency);
    
    lowCutFilter.process(buffer);
}
//==========================================================================================================//

bool LowCutAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* LowCutAudioProcessor::createEditor()
{
    return new LowCutAudioProcessorEditor (*this);
}

void LowCutAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void LowCutAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LowCutAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout LowCutAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "CUT_OFF_FREQUENCY",
        "Cut Off Frequency",
        juce::NormalisableRange{20.f,       // rangeStart
                                20000.f,    // rangeEnd
                                0.1f,       // intervalValue
                                0.2f,       // skewFactor
                                false},     // useSymmetricSkew
        500.f                               // default
        ));

    return { params.begin(), params.end() };
}