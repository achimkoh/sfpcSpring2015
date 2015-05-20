#pragma once

#include "ofMain.h"

class sonicDot {
    
public:
    
    void startRec(int counter, float rotAngle);
    void stopRec();
    bool rec;
    ofPoint center;
    int radius;
    
    void update(float rotAngle, int rotateX, int rotateY, int rotateZ, int counter);
    void draw();
    int maxFrames;
    
    ofSoundPlayer wave;
    float pitch(int y);
    float pan(int x);
    
    int start, cur;

    
};