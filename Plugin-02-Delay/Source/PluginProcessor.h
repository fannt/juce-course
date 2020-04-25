/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define MAX_DELAY_TIME 2

//==============================================================================
/**
*/
class Plugin02delayAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Plugin02delayAudioProcessor();
    ~Plugin02delayAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugin02delayAudioProcessor)
    float *mCircularBufferLeft;
    float *mCircularBufferRight;
    
    int mCircularBufferWriteHead;
    int mCircularBufferlength;
    
    float mDelayReadHead;
    float mDelayTimeInSamples;
    
    float mFeedbackLeft;
    float mFeedbackRight;
    
    float mDelayTimeSmooth;
    
    AudioParameterFloat *mDryWet;
    AudioParameterFloat *mFeedback;
    AudioParameterFloat *mDelayTime;
    
    float lin_interp(float sample_x, float sample_x1, float inPhase);
};
