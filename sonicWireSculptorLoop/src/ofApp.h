#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);

    void crosshair(int size, ofColor color);
    
    // drawing lines
    ofPolyline line;
    vector < ofPolyline > lines;
    bool curLeft;
    vector < bool > left;
    float rotateX, rotateY, rotateZ, tempY;
    bool rec, keyDown;
    int frameCount;
    
    bool snap;
    ofPoint snapPoint;
    int previousY;
    
    int beatLength;
    int cycleLength;
    float rotAngle;
    int gridInterval;
    
    // sound
    ofSoundPlayer wave;
    vector < ofSoundPlayer > waves;
    vector < ofPoint > playheads;
    float pitch(int y);
    float pan(int x);
    
};
