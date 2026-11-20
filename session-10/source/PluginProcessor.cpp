#include "PluginProcessor.h"
#include "PluginEditor.h"

HelloJuceAudioProcessor::HelloJuceAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
}

bool HelloJuceAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto output = layouts.getMainOutputChannelSet();
    return (output == juce::AudioChannelSet::mono()
            || output == juce::AudioChannelSet::stereo())
        && output == layouts.getMainInputChannelSet();
}

void HelloJuceAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                          juce::MidiBuffer& midi)
{
    juce::ignoreUnused(buffer);
    // No processing: input audio passes through unchanged.
    midi.clear();
}

juce::AudioProcessorEditor* HelloJuceAudioProcessor::createEditor()
{
    return new HelloJuceAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HelloJuceAudioProcessor();
}
