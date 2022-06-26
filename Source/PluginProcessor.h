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
class DistortionModellingAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    DistortionModellingAudioProcessor();
    ~DistortionModellingAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getState();

    //---------------------------------------
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };



private:
    void waveshape(float* input, std::atomic<float>* clip);

    float lastSampleRate;
    using Filter = juce::dsp::IIR::Filter<float>;
    juce::dsp::ProcessorDuplicator<Filter, juce::dsp::IIR::Coefficients<float>> PassFilter;

    using CutFilter = juce::dsp::ProcessorChain<Filter, Filter>;
    using MonoChain = juce::dsp::ProcessorChain<CutFilter, CutFilter>;
    MonoChain leftChain, rightChain;

    enum ChainPositions {
        LowCut,
        HighCut
    };


    void updateConvolution(double sampleRate, juce::uint32 maxBlockSize, juce::uint32 totalNumInputChannels);

    juce::dsp::ProcessorChain<juce::dsp::Convolution> processorChain;
    juce::dsp::Convolution convolution;
    juce::dsp::ProcessSpec spec;






    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistortionModellingAudioProcessor)
};