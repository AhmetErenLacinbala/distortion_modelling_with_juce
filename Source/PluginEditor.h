/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DistortionModellingAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DistortionModellingAudioProcessorEditor (DistortionModellingAudioProcessor&);
    ~DistortionModellingAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    /*juce::ScopedPointer<juce::Slider> gainKnob;
    juce::ScopedPointer<juce::Slider> rangeKnob;
    juce::ScopedPointer<juce::Slider> blendKnob;
    juce::ScopedPointer<juce::Slider> volumeKnob;

    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment; //saves parameter values
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> rangeAttachment;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> blendAttachment;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;*/

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionModellingAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionModellingAudioProcessorEditor)
};

/*class QuickSlider : public DistortionModellingAudioProcessorEditor {
public:    
    juce::ScopedPointer<juce::Slider> knob;
    juce::ScopedPointer<juce::AudioProcessorValueTreeState::SliderAttachment> knob; //saves parameter values


};*/