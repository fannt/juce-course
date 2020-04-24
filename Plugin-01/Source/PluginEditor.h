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
class Plugin01AudioProcessorEditor  : public AudioProcessorEditor,
                                      public Slider::Listener
{
public:
    Plugin01AudioProcessorEditor (Plugin01AudioProcessor&);
    ~Plugin01AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Plugin01AudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugin01AudioProcessorEditor)
    
    Slider mGainControlSlider;
};
