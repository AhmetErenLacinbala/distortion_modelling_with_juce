/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionModellingAudioProcessorEditor::DistortionModellingAudioProcessorEditor(DistortionModellingAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    GainSliderAttachment(audioProcessor.apvts, "Gain", GainSlider),
    HardClipSliderAttachment(audioProcessor.apvts, "Hard Clip", HardClipSlider),
    LowCutFreqSliderAttachment(audioProcessor.apvts, "LowCut Freq", LowCutFreqSlider),
    HighCutFreqSliderAttachment(audioProcessor.apvts, "HighCut Freq", HighCutFreqSlider),
    VolumeSliderAttachment(audioProcessor.apvts, "Volume", VolumeSlider)

{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    GainLabel.setText("Gain", juce::dontSendNotification);
    HardClipLabel.setText("Hard Clip", juce::dontSendNotification);
    LowCutFreqLabel.setText("LowCut Freq", juce::dontSendNotification);
    HighCutFreqLabel.setText("HighCut Freq", juce::dontSendNotification);
    VolumeLabel.setText("Volume", juce::dontSendNotification);

    GainLabel.attachToComponent(&GainLabel, true);
    GainLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    GainLabel.setJustificationType(juce::Justification::right);
   
    for (int i = 0; i <= sizeof(getComps()) / sizeof(getComps().at(0)); ++i) {
        addAndMakeVisible(getComps().at(i));
        addAndMakeVisible(getLabels().at(i));
        getLabels().at(i)->attachToComponent(getComps().at(i), false);
        getLabels().at(i)->setColour(juce::Label::textColourId, juce::Colours::orange);
        getLabels().at(i)->setJustificationType(juce::Justification::centred);
    }



    setSize(1000, 250);
}

DistortionModellingAudioProcessorEditor::~DistortionModellingAudioProcessorEditor()
{
}

//==============================================================================
void DistortionModellingAudioProcessorEditor::paint(juce::Graphics& g)
{

    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void DistortionModellingAudioProcessorEditor::resized()
{
    auto SliderX = 1000;
    auto SliderY = 40;
    GainSlider.setBounds(5, SliderY, 190, 190);
    HardClipSlider.setBounds(10 + SliderX / 5, SliderY, 170, 170);
    LowCutFreqSlider.setBounds(10 + 2 * SliderX / 5, SliderY, 170, 170);
    HighCutFreqSlider.setBounds(10 + 3 * SliderX / 5, SliderY, 170, 170);
    VolumeSlider.setBounds(10 + 4 * SliderX / 5, SliderY, 170, 170);

}

std::vector<juce::Component*> DistortionModellingAudioProcessorEditor::getComps() {
    return{
        &GainSlider, &HardClipSlider, &LowCutFreqSlider, &HighCutFreqSlider, &VolumeSlider
    };
}

using APVTS = juce::AudioProcessorValueTreeState;
using Attachment = APVTS::SliderAttachment;
std::vector<Attachment*> DistortionModellingAudioProcessorEditor::getAttachment()
{
    return{
       &GainSliderAttachment, &HardClipSliderAttachment, &LowCutFreqSliderAttachment, &HighCutFreqSliderAttachment, &VolumeSliderAttachment
    };
}

std::vector<juce::Label*> DistortionModellingAudioProcessorEditor::getLabels() {
    return{
        &GainLabel, &HardClipLabel, &LowCutFreqLabel, &HighCutFreqLabel, &VolumeLabel
    };
}

