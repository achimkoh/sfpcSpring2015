#pragma once
#include "ofMain.h"
#include "sonicWire.h"
#include "ofxVideoRecorder.h"
#include "ofxTextInputField.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    bool keyDown;
    
    int gridInterval, counter;
    float rotAngle, rotateX, rotateY, rotateZ, tempY;
    
    vector < sonicWire > wires;
    
    ofSoundPlayer metronome;
    void crosshair(int size, int time, ofColor color1, ofColor color2);
    
    // audio stream
    ofSoundStream soundStream;
    void audioIn(float * input, int bufferSize, int nChannels);
    
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    int 	bufferCounter;
    int 	drawCounter;
    
    float smoothedVol;
    float scaledVol;
    
    // video recorder
    ofxVideoRecorder    vidRecorder;
    bool bRecording;
    int sampleRate;
    int channels;
    string fileName;
    string fileExt;
    
    ofFbo recordFbo;
    ofPixels recordPixels;
    
    int recStart;
    
    ofxTextInputField monoLineTextInput;
    bool waitingInput;
};
