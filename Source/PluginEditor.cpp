/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


void KadenzeChorusAppAudioProcessorEditor::setup_sliders(int i, Slider& slider, AudioParameterFloat* p){
    slider.setBounds(20 + i*120, 20, 100, 100);
    slider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    slider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    slider.setRange(p->range.start, p->range.end);
    slider.setValue(*p);
    addAndMakeVisible(slider);
    
    slider.onValueChange = [this, p = p, &slider] (){
        *p = slider.getValue();
    };
    
    slider.onDragStart = [p] (){
        p->beginChangeGesture();
    };
    
    slider.onDragEnd = [p] (){
        p->endChangeGesture();
    };
}

//==============================================================================
KadenzeChorusAppAudioProcessorEditor::KadenzeChorusAppAudioProcessorEditor (KadenzeChorusAppAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 250);
    auto& params = processor.getParameters();
    auto* dry_wet_param = static_cast<AudioParameterFloat*>(params.getUnchecked(0));
    auto* depth_param = static_cast<AudioParameterFloat*>(params.getUnchecked(1));
    auto* rate_param = static_cast<AudioParameterFloat*>(params.getUnchecked(2));
    auto* phase_offset_param = static_cast<AudioParameterFloat*>(params.getUnchecked(3));
    auto* feedback_param = static_cast<AudioParameterFloat*>(params.getUnchecked(4));

    
    setup_sliders(0, m_slider_dry_wet, dry_wet_param);
    setup_sliders(1, m_slider_depth, depth_param);
    setup_sliders(2, m_slider_rate, rate_param);
    setup_sliders(3, m_slider_phase_offset, phase_offset_param);
    setup_sliders(4, m_slider_feedback, feedback_param);
    
    auto* type_param = static_cast<AudioParameterInt*>(params.getUnchecked(5));
    m_type_combo.setBounds(200, 200, 100, 20);
    m_type_combo.addItem("Chorus", 1);
    m_type_combo.addItem("Flanger", 2);
    addAndMakeVisible(m_type_combo);
    
    m_type_combo.onChange = [this, type_param](){
        type_param->beginChangeGesture();
        *type_param = m_type_combo.getSelectedItemIndex();
        type_param->endChangeGesture();
    };
    
    m_type_combo.setSelectedItemIndex(*type_param);
}

KadenzeChorusAppAudioProcessorEditor::~KadenzeChorusAppAudioProcessorEditor()
{
}

//==============================================================================
void KadenzeChorusAppAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);
    
    g.setColour (Colours::white);
    Font f{"Raleway",15.0f,0};
    g.setFont(f);
    
    Rectangle<int> area{45,100,50,50};
    Rectangle<int> area2{165,100,100,50};
    Rectangle<int> area3{290,100,100,50};
    Rectangle<int> area4{410,100,100,50};
    Rectangle<int> area5{530,100,100,50};

    
    g.drawFittedText("Dry/Wet", area , Justification::left, 1);
    g.drawFittedText("Depth", area2 , Justification::left, 1);
    g.drawFittedText("Rate", area3 , Justification::left, 1);
    g.drawFittedText("Phase", area4 , Justification::left, 1);
    g.drawFittedText("Feedback", area5 , Justification::left, 1);

}

void KadenzeChorusAppAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
