/*
  ==============================================================================

    LFO.h
    Created: 14 Apr 2019 5:36:45pm
    Author:  Francesco Fucci

  ==============================================================================
*/

#pragma once

struct LFO {
    
    LFO() : m_lfoPhase{0.0f} {}
    
    inline float update_lfo(double rate,double sample_rate,double phase_offset = 0.0f) {
        auto val = std::sin(2*M_PI*m_lfoPhase);
        m_lfoPhase += rate/sample_rate;
        m_lfoPhase += phase_offset;
        if(m_lfoPhase > 1.){
            val -= 1.;
        }
        return val;
    }
    
    private:
        float m_lfoPhase;
    
};
