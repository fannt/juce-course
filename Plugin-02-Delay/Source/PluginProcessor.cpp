/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Plugin02delayAudioProcessor::Plugin02delayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    mCircularBufferLeft = nullptr;
    mCircularBufferRight = nullptr;
    mCircularBufferWriteHead = 0;
    mCircularBufferlength = 0;
    
    mDelayReadHead = 0;
    mDelayTimeInSamples = 0;
    sampleRate = 0;
    
    mDelayTimeParam = new AudioParameterFloat("delayTime","Delay Time",0.0f,2.0f,0.2f);
    addParameter(mDelayTimeParam);
    
    float mFeedbackLeft = 0;
    float mFeedbackRight = 0;
}

Plugin02delayAudioProcessor::~Plugin02delayAudioProcessor()
{
    if (mCircularBufferLeft != nullptr) {
        delete [] mCircularBufferLeft;
        mCircularBufferLeft = nullptr;
    }
    
    if (mCircularBufferRight != nullptr) {
        delete [] mCircularBufferRight;
        mCircularBufferRight = nullptr;
    }
}

//==============================================================================
const String Plugin02delayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Plugin02delayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Plugin02delayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Plugin02delayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Plugin02delayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugin02delayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugin02delayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Plugin02delayAudioProcessor::setCurrentProgram (int index)
{
}

const String Plugin02delayAudioProcessor::getProgramName (int index)
{
    return {};
}

void Plugin02delayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Plugin02delayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    //fix changing sample reate during the session
    if (mCircularBufferLeft != nullptr ) {
      delete [] mCircularBufferLeft;
      mCircularBufferLeft = nullptr;
    }
    if (mCircularBufferRight != nullptr ) {
      delete [] mCircularBufferRight;
      mCircularBufferRight = nullptr;
   }
    
    mCircularBufferlength = sampleRate * MAX_DELAY_TIME;
    
    if (mCircularBufferLeft == nullptr) {
        mCircularBufferLeft = new float[mCircularBufferlength];
    }
    
    if (mCircularBufferRight == nullptr) {
        mCircularBufferRight = new float[mCircularBufferlength];
    }
    
    mCircularBufferWriteHead = 0;

//    mDelayReadHead = 0;
    mDelayTimeInSamples = mDelayTimeParam->get() * sampleRate;
    this->sampleRate = sampleRate;
}

void Plugin02delayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Plugin02delayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void Plugin02delayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    mDelayTimeInSamples = mDelayTimeParam->get() * sampleRate;
    
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        
        mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i] + mFeedbackLeft;
        mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i] + mFeedbackRight;
        
        mDelayReadHead = mCircularBufferWriteHead - mDelayTimeInSamples;
        
        if (mDelayReadHead < 0) {
            mDelayReadHead += mCircularBufferlength;
        }
        
        float delay_sample_left = mCircularBufferLeft[(int)mDelayReadHead];
        float delay_sample_right = mCircularBufferRight[(int)mDelayReadHead];
        
        mFeedbackLeft = delay_sample_left * 0.4;
        mFeedbackRight = delay_sample_right * 0.4;
        
        buffer.addSample(0, i, delay_sample_left);
        buffer.addSample(1, i, delay_sample_right);
        
        mCircularBufferWriteHead++;
        
        if (mCircularBufferWriteHead >= mCircularBufferlength) {
            mCircularBufferWriteHead = 0;
        }
    }
}

//==============================================================================
bool Plugin02delayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Plugin02delayAudioProcessor::createEditor()
{
    return new Plugin02delayAudioProcessorEditor (*this);
}

//==============================================================================
void Plugin02delayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Plugin02delayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugin02delayAudioProcessor();
}


//void delayTimeDidChang () {
//    *this.prepa
//}
