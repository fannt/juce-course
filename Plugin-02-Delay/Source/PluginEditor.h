/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Plugin02delayAudioProcessorEditor  : public AudioProcessorEditor,
                                           public Slider::Listener
{
public:
    Plugin02delayAudioProcessorEditor (Plugin02delayAudioProcessor&);
    ~Plugin02delayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plugin02delayAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugin02delayAudioProcessorEditor)
    
    Slider mDelayTimeSlider;
};
