#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class HelloJuceAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit HelloJuceAudioProcessorEditor(HelloJuceAudioProcessor&);
    void paint(juce::Graphics&) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HelloJuceAudioProcessorEditor)
};
