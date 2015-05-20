#pragma once

#include "ofMain.h"

//--------------------------------------------------
class sonicWire {
    
public:
    
    void startRec(int counter, float rotAngle);
    void stopRec();
    bool rec;
    bool noLine();
    
    ofPolyline line;
    
    void update(float rotAngle, int rotateX, int rotateY, int rotateZ, int counter);
    void draw();
    int maxFrames;
    
    void play();
    void stop();
    void reverse(int dir);
    int start, playhead, cur;
    bool playing, hasChangedDirection;
    
    // sound
    ofSoundPlayer wave;
    void waveSetup(ofSoundPlayer player, ofPoint point);
    float pitch(int y);
    float pitchCache;
    float pan(int x);
    float volume(int x, int y);
    
};