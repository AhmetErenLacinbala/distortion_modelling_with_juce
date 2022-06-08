/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>



//==============================================================================
/**
*/
class DistortionModellingAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DistortionModellingAudioProcessor();
    ~DistortionModellingAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getState();

    //---------------------------------------
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{*this, nullptr, "Parameters", createParameterLayout()};


private:
    void updateConvolution(double sampleRate, juce::uint32 maxBlockSize, juce::uint32 totalNumInputChannels);
    juce::dsp::ProcessorChain<juce::dsp::Convolution> processorChain;
    juce::dsp::Convolution convolution;
    juce::dsp::ProcessSpec spec;
    

   
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionModellingAudioProcessor)
};

/*class CabSim {
private:
    enum {
        convolutionIndex
    };
    using convol = juce::dsp::ProcessorChain<juce::dsp::Convolution>;
    convol processorChain;
    void reset() noexcept {
        processorChain.reset();
    }
    CabSim()
    {
        auto dir = juce::File::getCurrentWorkingDirectory();

        int numTries = 0;

        while (!dir.getChildFile("Resources").exists() && numTries++ < 15)
            dir = dir.getParentDirectory();

        auto& convolution = processorChain.template get<convolutionIndex>();    // [5]

        convolution.loadImpulseResponse(dir.getChildFile("freq_resp").getChildFile("orange cust_sm57 1.wav"),
            juce::dsp::Convolution::Stereo::no,
            juce::dsp::Convolution::Trim::no,
            1024);                                 
    }
  
};*/