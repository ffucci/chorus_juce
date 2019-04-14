/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class KadenzeChorusAppAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    KadenzeChorusAppAudioProcessorEditor (KadenzeChorusAppAudioProcessor&);
    ~KadenzeChorusAppAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    
    void setup_sliders(int i, Slider& slider, AudioParameterFloat* p);
    
    Slider m_slider_depth;
    Slider m_slider_dry_wet;
    Slider m_slider_feedback;
    Slider m_slider_phase_offset;
    Slider m_slider_rate;
    ComboBox m_type_combo;
    //Slider m_slider_delay_time;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    KadenzeChorusAppAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KadenzeChorusAppAudioProcessorEditor)
};
