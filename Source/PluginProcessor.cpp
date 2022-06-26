/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include<stdlib.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"
# define M_PI           3.14159265358979323846

//==============================================================================
DistortionModellingAudioProcessor::DistortionModellingAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
#endif
{


}

DistortionModellingAudioProcessor::~DistortionModellingAudioProcessor()
{
}

//==============================================================================
const juce::String DistortionModellingAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionModellingAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool DistortionModellingAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool DistortionModellingAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double DistortionModellingAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionModellingAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistortionModellingAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionModellingAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String DistortionModellingAudioProcessor::getProgramName(int index)
{
    return {};
}

void DistortionModellingAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void DistortionModellingAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    using namespace juce;
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    updateConvolution(sampleRate, samplesPerBlock, getTotalNumInputChannels());
    leftChain.prepare(spec);
    rightChain.prepare(spec);

    auto HighCutCoefficients = dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(
        apvts.getRawParameterValue("HighCut Freq")->load(),
        getSampleRate(),
        1);

    auto& leftHighCut = leftChain.get<ChainPositions::HighCut>();
    auto& rightHighCut = rightChain.get<ChainPositions::HighCut>();

    leftHighCut.get<0>().coefficients = *HighCutCoefficients[0];
    leftHighCut.setBypassed<0>(false);

    rightHighCut.get<0>().coefficients = *HighCutCoefficients[0];
    rightHighCut.setBypassed<0>(false);




    auto LowCutCoefficients = dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(
        apvts.getRawParameterValue("LowCut Freq")->load(),
        getSampleRate(),
        1);


    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();
    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();

    *rightLowCut.get<0>().coefficients = *LowCutCoefficients[0];
    rightLowCut.setBypassed<0>(false);







}

void DistortionModellingAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionModellingAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void DistortionModellingAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    using namespace juce;
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    auto gain = apvts.getRawParameterValue("Gain");
    gain->load();
    auto volume = apvts.getRawParameterValue("Volume");
    volume->load();
    auto clip = apvts.getRawParameterValue("Hard Clip");
    clip->load();
    auto lowCutFreq = apvts.getRawParameterValue("LowCut Freq");
    lowCutFreq->load();
    auto highCutFreq = apvts.getRawParameterValue("HighCut Freq");
    highCutFreq->load();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    dsp::AudioBlock<float> block(buffer);

    dsp::ProcessContextReplacing<float> context(block);
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto HighCutCoefficients = dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(
        apvts.getRawParameterValue("HighCut Freq")->load(),
        getSampleRate(),
        1);

    auto& leftHighCut = leftChain.get<ChainPositions::HighCut>();
    auto& rightHighCut = rightChain.get<ChainPositions::HighCut>();

    leftHighCut.get<0>().coefficients = *HighCutCoefficients[0];
    leftHighCut.setBypassed<0>(false);

    rightHighCut.get<0>().coefficients = *HighCutCoefficients[0];
    rightHighCut.setBypassed<0>(false);




    auto LowCutCoefficients = dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(
        apvts.getRawParameterValue("LowCut Freq")->load(),
        getSampleRate(),
        1);


    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();
    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();

    *leftLowCut.get<0>().coefficients = *LowCutCoefficients[0];
    leftLowCut.setBypassed<0>(false);

    *rightLowCut.get<0>().coefficients = *LowCutCoefficients[0];
    rightLowCut.setBypassed<0>(false);

    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);


    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {

            channelData[sample] *= *gain;
            waveshape(&channelData[sample], clip);
            channelData[sample] *= *volume;

        }



        // ..do something to the data...
    }

    leftChain.process(dsp::ProcessContextReplacing<float>(leftBlock));
    rightChain.process(dsp::ProcessContextReplacing<float>(rightBlock));
    convolution.process(context);

}

/*juce::AudioProcessorValueTreeState& DistortionModellingAudioProcessor::getState() {
    return *state;
}*/

//==============================================================================
bool DistortionModellingAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistortionModellingAudioProcessor::createEditor()
{
    return new DistortionModellingAudioProcessorEditor (*this);
    //-----------------------------------------
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void DistortionModellingAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream mos(destData, true);
    apvts.state.writeToStream(mos);

}

void DistortionModellingAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid()) {
        apvts.replaceState(tree);
    }

}

juce::AudioProcessorValueTreeState::ParameterLayout DistortionModellingAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("Gain", "Gain", juce::NormalisableRange<float>(1.f, 1000.f, 0.0001), 1.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Hard Clip", "Hard Clip", juce::NormalisableRange<float>(0.99f, 1.f, 0.00001), 1.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Volume", "Volume", juce::NormalisableRange<float>(0.f, 2.f, 0.0001), 1.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("LowCut Freq", "LowCut Freq", juce::NormalisableRange<float>(0.f, 1000.f, 0.5), 1.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("HighCut Freq", "HighCut Freq", juce::NormalisableRange<float>(1000.f, 20000.f, 0.5), 1.0));



    /*juce::StringArray dist_type("Overdrive", "Distortion", "Fuzz");
    layout.add(std::make_unique<juce::AudioParameterChoice>("Distortion Type", "Distortion Type", dist_type, 0));*/


    return layout;
}
void DistortionModellingAudioProcessor::waveshape(float* input, std::atomic<float>* clip) {

    *input = ((2.f / M_PI) * atan(*input)) * (1 / *clip);
    if (abs(*input) > *clip) {
        if (*input > 0) *input = *clip;
        else *input = *input *= *clip * -1;
    }
    *input *= (1 / *clip);
}



void DistortionModellingAudioProcessor::updateConvolution(double sampleRate, juce::uint32 maxBlockSize, juce::uint32 totalNumInputChannels)
{
    convolution.reset();
    convolution.loadImpulseResponse(juce::File("E:/dev/juce/test/DistortionModelling/Builds/VisualStudio2022/freq_resp/orange cust_sm57 1.wav"), //this will be changed

        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::no,
        1024, juce::dsp::Convolution::Normalise::yes);
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = maxBlockSize;
    spec.numChannels = totalNumInputChannels;
    convolution.prepare(spec);

}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionModellingAudioProcessor();
}