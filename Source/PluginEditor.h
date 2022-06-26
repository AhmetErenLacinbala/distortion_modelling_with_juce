/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct CustomRotartSlider :juce::Slider {
   
    CustomRotartSlider() : juce::Slider(
juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
        juce::Slider::TextEntryBoxPosition::NoTextBox)
    {
       
    }
};

//==============================================================================
/**
*/
class DistortionModellingAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    DistortionModellingAudioProcessorEditor(DistortionModellingAudioProcessor&);
    ~DistortionModellingAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:



    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionModellingAudioProcessor& audioProcessor;
    
    CustomRotartSlider GainSlider, HardClipSlider, LowCutFreqSlider, HighCutFreqSlider, VolumeSlider;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment GainSliderAttachment, HardClipSliderAttachment, LowCutFreqSliderAttachment, HighCutFreqSliderAttachment, VolumeSliderAttachment;
    
    juce::Label GainLabel, HardClipLabel, LowCutFreqLabel, HighCutFreqLabel, VolumeLabel;


    std::vector<juce::Component*> getComps();
    std::vector<Attachment*> getAttachment();
    std::vector<juce::Label*> getLabels();
     

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionModellingAudioProcessorEditor)
};
