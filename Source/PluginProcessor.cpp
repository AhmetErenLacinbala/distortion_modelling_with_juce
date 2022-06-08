/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
# define M_PI           3.14159265358979323846

//==============================================================================
DistortionModellingAudioProcessor::DistortionModellingAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
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

void DistortionModellingAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DistortionModellingAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistortionModellingAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DistortionModellingAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    using namespace juce;
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    updateConvolution(sampleRate, samplesPerBlock, getTotalNumInputChannels());



  
}

void DistortionModellingAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionModellingAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void DistortionModellingAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    using namespace juce;
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto gain = apvts.getRawParameterValue("Gain");
    gain->load(); std::cout << gain << std::endl;
    auto blend = apvts.getRawParameterValue("Blend");
    blend->load();
    auto volume = apvts.getRawParameterValue("Volume");
    volume->load();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            

            float cleanSignal = channelData[sample];
            channelData[sample] *= *gain;
            channelData[sample] = ((((2.f / M_PI) * atan(channelData[sample])) * *blend) + (cleanSignal * (1.f / *blend)) / 2) * *volume;     

        }



        // ..do something to the data...
    }
    dsp::AudioBlock<float> block(buffer);
    dsp::ProcessContextReplacing<float> context(block);
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
    //return new DistortionModellingAudioProcessorEditor (*this);
    //-----------------------------------------
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void DistortionModellingAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

}

void DistortionModellingAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    juce::ValueTree tree = juce::ValueTree::readFromData(data, sizeInBytes);

}

juce::AudioProcessorValueTreeState::ParameterLayout DistortionModellingAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("Gain", "Gain", juce::NormalisableRange<float>(1.f, 1000.f, 0.0001), 1.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Blend", "Blend", juce::NormalisableRange<float>(0.f, 1.f, 0.0001), 1.0));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Volume", "Volume", juce::NormalisableRange<float>(0.f, 2.f, 0.0001), 1.0));


    return layout;
}

void DistortionModellingAudioProcessor::updateConvolution(double sampleRate, juce::uint32 maxBlockSize, juce::uint32 totalNumInputChannels)
{
    convolution.reset();
   /* convolution.loadImpulseResponse(juce::File::getCurrentWorkingDirectory().getChildFile("freq_resp").getChildFile("vox custom _bright nt2 off axis.wav"),
        
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::no,
        0, juce::dsp::Convolution::Normalise::yes);*/
    convolution.loadImpulseResponse(juce::File("E:/dev/juce/test/DistortionModelling/Builds/VisualStudio2022/freq_resp/orange cust_sm57 1.wav"),

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
