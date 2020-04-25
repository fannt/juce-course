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
    
    
    auto& params = processor.getParameters();
    
    // dry/wet slider
    
    mDryWetSlider.setBounds(6, 112, 100, 100);
    mDryWetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mDryWetSlider.setTextBoxStyle(Slider::TextBoxAbove, true, 80, 10);

    AudioParameterFloat* dryWet = (AudioParameterFloat*)params.getUnchecked(0);
    mDryWetSlider.setRange(dryWet->range.start, dryWet->range.end);
    mDryWetSlider.setValue(*dryWet);

    mDryWetSlider.addListener(this);
    addAndMakeVisible(mDryWetSlider);
    
    mDryWetSlider.onValueChange = [this, dryWet] { *dryWet = mDryWetSlider.getValue(); };
    mDryWetSlider.onDragStart = [dryWet] {dryWet->beginChangeGesture();};
    mDryWetSlider.onDragEnd = [dryWet] {dryWet->endChangeGesture();};
    
    // delay time slider
    
    mDelayTimeSlider.setBounds(112, 6, 100, 100);
    mDelayTimeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mDelayTimeSlider.setTextBoxStyle(Slider::TextBoxAbove, true, 80, 10);
    
    AudioParameterFloat* delay = (AudioParameterFloat*)params.getUnchecked(2);
    mDelayTimeSlider.setRange(delay->range.start, delay->range.end);
    mDelayTimeSlider.setValue(*delay);
    
    mDelayTimeSlider.addListener(this);
    addAndMakeVisible(mDelayTimeSlider);
    
    mDelayTimeSlider.onValueChange = [this, delay] { *delay = mDelayTimeSlider.getValue(); };
    mDelayTimeSlider.onDragStart = [delay] {delay->beginChangeGesture();};
    mDelayTimeSlider.onDragEnd = [delay] {delay->endChangeGesture();};
    
    // feedback slider
    
    mFeedbackSlider.setBounds(6, 6, 100, 100);
    mFeedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mFeedbackSlider.setTextBoxStyle(Slider::TextBoxAbove, true, 80, 10);

    AudioParameterFloat* feedback = (AudioParameterFloat*)params.getUnchecked(1);
    mFeedbackSlider.setRange(feedback->range.start, feedback->range.end);
    mFeedbackSlider.setValue(*feedback);

    mFeedbackSlider.addListener(this);
    addAndMakeVisible(mFeedbackSlider);
    
    mFeedbackSlider.onValueChange = [this, feedback] { *feedback = mFeedbackSlider.getValue(); };
    mFeedbackSlider.onDragStart = [feedback] {feedback->beginChangeGesture();};
    mFeedbackSlider.onDragEnd = [feedback] {feedback->endChangeGesture();};
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
   
}
