/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Plugin01AudioProcessorEditor::Plugin01AudioProcessorEditor (Plugin01AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    mGainControlSlider.setBounds(0, 0, 100, 100);
    mGainControlSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mGainControlSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    auto& params = processor.getParameters();
    AudioParameterFloat* gain = (AudioParameterFloat*)params.getUnchecked(0);
    mGainControlSlider.setRange(gain->range.start, gain->range.end);
    mGainControlSlider.setValue(*gain);
    
    mGainControlSlider.addListener(this);
    addAndMakeVisible(mGainControlSlider);
}

Plugin01AudioProcessorEditor::~Plugin01AudioProcessorEditor()
{
}

//==============================================================================
void Plugin01AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void Plugin01AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void Plugin01AudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    auto& params = processor.getParameters();
    
    if (slider == &mGainControlSlider) {
        DBG("gain slider change");
        AudioParameterFloat* gain = (AudioParameterFloat*)params.getUnchecked(0);
        *gain = mGainControlSlider.getValue();
        
    }
}
