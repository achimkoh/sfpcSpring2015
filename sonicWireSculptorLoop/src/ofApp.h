#pragma once

#include "ofMain.h"
#include "sonicWire.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    void crosshair(int size, int time, ofColor color1, ofColor color2);

    int gridInterval, counter;
    float rotAngle, rotateX, rotateY, rotateZ, tempY;
    
    vector < sonicWire > wires;
    
    bool keyDown;
    
    ofSoundPlayer metronome;
        
};
