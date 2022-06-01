/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionModellingAudioProcessorEditor::DistortionModellingAudioProcessorEditor (DistortionModellingAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    /*addAndMakeVisible(gainKnob = new juce::Slider("Gain"));
    gainKnob->setSliderStyle(juce::Slider::Rotary);
    gainKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);
    addAndMakeVisible(rangeKnob = new juce::Slider("Range"));
    rangeKnob->setSliderStyle(juce::Slider::Rotary);
    rangeKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);
    addAndMakeVisible(blendKnob = new juce::Slider("Blend"));
    blendKnob->setSliderStyle(juce::Slider::Rotary);
    blendKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);
    addAndMakeVisible(volumeKnob = new juce::Slider("Volume"));
    volumeKnob->setSliderStyle(juce::Slider::Rotary);
    volumeKnob->setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);


    gainAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "gain", *gainKnob);
    rangeAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "range", *rangeKnob);
    blendAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "blend", *blendKnob);
    volumeAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(p.getState(), "volume", *volumeKnob);*/

    setSize (500, 200);
}

DistortionModellingAudioProcessorEditor::~DistortionModellingAudioProcessorEditor()
{
}

//==============================================================================
void DistortionModellingAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void DistortionModellingAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
   /* gainKnob->setBounds(((getWidth() / 5 * 1) - (100 / 2)), (getHeight() / 2) - (100 / 2), 100, 100);
    rangeKnob->setBounds(((getWidth() / 5 * 2) - (100 / 2)), (getHeight() / 2) - (100 / 2), 100, 100);
    blendKnob->setBounds(((getWidth() / 5 * 3) - (100 / 2)), (getHeight() / 2) - (100 / 2), 100, 100);
    volumeKnob->setBounds(((getWidth() / 5 * 4) - (100 / 2)), (getHeight() / 2) - (100 / 2), 100, 100);*/

}
