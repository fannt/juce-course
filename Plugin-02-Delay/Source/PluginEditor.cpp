/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Plugin02delayAudioProcessorEditor::Plugin02delayAudioProcessorEditor (Plugin02delayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    mDelayTimeSlider.setBounds(8, 8, 100, 100);
    mDelayTimeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mDelayTimeSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    auto& params = processor.getParameters();
    AudioParameterFloat* delay = (AudioParameterFloat*)params.getUnchecked(0);
    mDelayTimeSlider.setRange(delay->range.start, delay->range.end);
    mDelayTimeSlider.setValue(*delay);
    
    mDelayTimeSlider.addListener(this);
    addAndMakeVisible(mDelayTimeSlider);
}

Plugin02delayAudioProcessorEditor::~Plugin02delayAudioProcessorEditor()
{
}

//==============================================================================
void Plugin02delayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void Plugin02delayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}


void Plugin02delayAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    auto& params = processor.getParameters();
    
    if (slider == &mDelayTimeSlider) {
        DBG("delay slider change");
        AudioParameterFloat* delay = (AudioParameterFloat*)params.getUnchecked(0);
        *delay = mDelayTimeSlider.getValue();
    }
}
