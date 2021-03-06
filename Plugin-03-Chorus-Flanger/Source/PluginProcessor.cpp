/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ChorusflangerAudioProcessor::ChorusflangerAudioProcessor()
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
    
    mLFOPhase = 0;
    
    mDepth = new AudioParameterFloat("depth","Depth",0.0f,1.0,0.5f);
    addParameter(mDepth);
    
    mRate = new AudioParameterFloat("rate","Rate",0.1f,20.0f,10.f);
    addParameter(mRate);
    
    mPhaseOffset = new AudioParameterFloat("phaseOffset","Phase Offset",0.f,1.0f,0.f);
    addParameter(mPhaseOffset);
    
    mDryWet = new AudioParameterFloat("dryWet","Dry / Wet",0.01f,1.0f,0.5f);
    addParameter(mDryWet);
    
    mFeedback = new AudioParameterFloat("feedback","Feedback",0.0f,0.99f,0.4f);
    addParameter(mFeedback);
    
    mType = new AudioParameterInt("type","Type",0,1,0);
    addParameter(mType);
    
    mFeedbackLeft = 0;
    mFeedbackRight = 0;
    mDelayTimeSmoothLeft = 0;
    mDelayTimeSmoothRight = 0;
}

ChorusflangerAudioProcessor::~ChorusflangerAudioProcessor()
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
const String ChorusflangerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ChorusflangerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ChorusflangerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ChorusflangerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ChorusflangerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChorusflangerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ChorusflangerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ChorusflangerAudioProcessor::setCurrentProgram (int index)
{
}

const String ChorusflangerAudioProcessor::getProgramName (int index)
{
    return {};
}

void ChorusflangerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ChorusflangerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
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
         std::fill_n(mCircularBufferLeft, mCircularBufferlength, 0);
     }
     
     if (mCircularBufferRight == nullptr) {
         mCircularBufferRight = new float[mCircularBufferlength];
         std::fill_n(mCircularBufferRight, mCircularBufferlength, 0);
     }
     
//     mDelayTimeInSamples = *mDelayTime  * sampleRate;
     mCircularBufferWriteHead = 0; //mDelayTimeInSamples / 2;
    mDelayTimeSmoothLeft = 1;//*mDelayTime;
    mDelayTimeSmoothRight = 1;//*mDelayTime;
    mLFOPhase = 0;
}

void ChorusflangerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ChorusflangerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ChorusflangerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(1);
    
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        

        mLFOPhase += *mRate / getSampleRate();
        if ( mLFOPhase > 1) {
            mLFOPhase =-1;
        }
        
        float lfoOutLeft = *mDepth * sin(2 * M_PI * mLFOPhase);
        float lfoOutRight = *mDepth *  sin(2 * M_PI * mLFOPhase * *mPhaseOffset);
        
        float lfoOutLeftMapped = 0;
        float lfoOutRightMapped = 0;
        
        /* Chorus */
        if (*mType == 0) {
            lfoOutLeftMapped = jmap(lfoOutLeft, 0.005f, 0.03f);
            lfoOutRightMapped = jmap(lfoOutRight, 0.005f, 0.03f);
        } else {
        /* Flanger */
            lfoOutLeftMapped = jmap(lfoOutLeft, 0.001f, 0.005f);
            lfoOutRightMapped = jmap(lfoOutRight, 0.001f, 0.005f);
            
        }
        
        mDelayTimeSmoothLeft = mDelayTimeSmoothLeft - 0.001 * (mDelayTimeSmoothLeft - lfoOutLeftMapped);
        int delayTimeInSamplesLeft = mDelayTimeSmoothLeft * getSampleRate();
        
        mDelayTimeSmoothRight = mDelayTimeSmoothRight - 0.001 * (mDelayTimeSmoothRight - lfoOutRightMapped);
        int delayTimeInSamplesRight = mDelayTimeSmoothRight * getSampleRate();
        
        mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i] + mFeedbackLeft;
        mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i] + mFeedbackRight;
        
        float delayReadHeadLeft = mCircularBufferWriteHead - delayTimeInSamplesLeft;
        if (delayReadHeadLeft < 0) {
            delayReadHeadLeft += mCircularBufferlength;
        }
        
        float delayReadHeadRight = mCircularBufferWriteHead - delayTimeInSamplesRight;
        if (delayReadHeadRight < 0) {
            delayReadHeadRight += mCircularBufferlength;
        }
        
        int readHeadLeft_x = (int)delayReadHeadLeft;
        int readHeadLeft_x1 = readHeadLeft_x + 1;
        if (readHeadLeft_x1 >= mCircularBufferlength) {
            readHeadLeft_x1 -= mCircularBufferlength;
        }
        
        float readHeadFloatLeft = delayReadHeadLeft - readHeadLeft_x;
        
        
        int readHeadRight_x = (int)delayReadHeadRight;
        int readHeadRight_x1 = readHeadRight_x + 1;
        if (readHeadRight_x1 >= mCircularBufferlength) {
            readHeadRight_x1 -= mCircularBufferlength;
        }
        
        float readHeadFloatRight = delayReadHeadRight - readHeadRight_x;
        
        float delay_sample_left = lin_interp(mCircularBufferLeft[readHeadLeft_x], mCircularBufferLeft[readHeadLeft_x1], readHeadFloatLeft);
        float delay_sample_right = lin_interp(mCircularBufferRight[readHeadRight_x], mCircularBufferRight[readHeadRight_x1], readHeadFloatRight);
        
        mFeedbackLeft = delay_sample_left * *mFeedback;
        mFeedbackRight = delay_sample_right * *mFeedback;
        
        buffer.setSample(0, i, buffer.getSample(0, i) * (1 - *mDryWet) + delay_sample_left * *mDryWet);
        buffer.setSample(1, i, buffer.getSample(1, i) * (1 - *mDryWet) + delay_sample_right * *mDryWet);
        
        mCircularBufferWriteHead++;
        
        if (mCircularBufferWriteHead >= mCircularBufferlength) {
            mCircularBufferWriteHead = 0;
        }
    }
}

//==============================================================================
bool ChorusflangerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ChorusflangerAudioProcessor::createEditor()
{
    return new ChorusflangerAudioProcessorEditor (*this);
}

//==============================================================================
void ChorusflangerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ChorusflangerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChorusflangerAudioProcessor();
}

float ChorusflangerAudioProcessor::lin_interp(float sample_x, float sample_x1, float inPhase) {
    return (1 - inPhase) * sample_x + inPhase * sample_x1;
}
