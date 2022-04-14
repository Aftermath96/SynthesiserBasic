
#ifndef Oscillator_h
#define Oscillator_h
#include <cmath>
#include <vector>

/**
Oscillator class with three wave shapes (processSine, processSquare, processTriangle)
 */
class Oscillator
{
public:
    
    // Our parent oscillator class does the key things required for most oscillators:
    // -- handles phase
    // -- handles setters and getters for frequency and samplerate
    
   
    /**
     set the sample rate before use
     */
    void setSampleRate(float _sampleRate)
   
    {
        sampleRate = _sampleRate;
        sampleRate = 44100;
    }
    
    /**
     set the oscillator frequency
     
     @param freq oscillator frequency in Hz
     */
    void setFrequency(float freq)
   
    {
        frequency = freq;
        phaseDelta = frequency / sampleRate;
    }
    /**
     update the phase and output as a sinusoidal signal
     */
    
   
    
    /**
     sets the  phase process for sine wave
     */
    float process1()
    
    {
        phase += phaseDelta;
        
        if (phase > 1.0f)
            phase -= 1.0f;
        
        return sin(phase * 2.0 * 3.141592653589793);
    }
    
   
    
    /**
     updates the phase and output as a square wave signal
     */
    float process2()
    
    {
        phase += phaseDelta;
        
        if (phase > 1.0f)
            phase -= 1.0f;
        
        float outVal = 0.5;
        if (phase > 0.5)
            outVal = -0.5;
        
        return outVal;
    }
    
    /**
     updates the phase and output as a triangular signal
     */
    float process3()
    
    {
        
        
   
        phase += phaseDelta;
        
        if (phase > 1.0f)
            phase -= 1.0f;
        
        return fabs(phase - 0.25) - 0.25;
    
   }
    
  
    
private:
    float frequency;
    
    float sampleRate;
    
    float gain = 0.5f;
    
    float phase = 0.0f;
    
   // float phase2 = 0.0f;
    
    float phaseDelta;
    
    //float duration;
    
   // float modulationIndex = 20.0f;
    
    //float risingGain;
    
    //float phaseDelta2 = frequency  / float(sampleRate);
     //float durationinSamples =  sampleRate * duration ;
};


#endif /* Oscillators_h */
