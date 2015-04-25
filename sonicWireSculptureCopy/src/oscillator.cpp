
#include "oscillator.h"


    
void oscillator::setup (int rate){
    sampleRate = rate;
    targetVolume = 0;
}

void oscillator::setFrequency (float freq){
    frequency = freq;
    phaseAdder = (float)(frequency * TWO_PI) / (float)sampleRate;
}

void oscillator::setVolume (float vol){
    volume = vol;
}

float oscillator::getSample(){
    
    setVolume(0.9995 * volume + 0.0005 * targetVolume);
    phase += phaseAdder;
    while (phase > TWO_PI) phase -= TWO_PI;
    return sin(phase) * volume;
    

}

