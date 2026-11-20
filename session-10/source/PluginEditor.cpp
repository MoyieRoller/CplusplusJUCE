#include "PluginEditor.h"

HelloJuceAudioProcessorEditor::HelloJuceAudioProcessorEditor(HelloJuceAudioProcessor& processor)
    : AudioProcessorEditor(processor)
{
    setSize(400, 240);
}

void HelloJuceAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkslategrey);
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(32.0f));
    g.drawText("Hello JUCE", getLocalBounds(), juce::Justification::centred);
}
