#pragma once

#include "ofMain.h"
#include "oscillator.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        
    ofPolyline line;
    vector < ofPolyline > lines;
    float rotateX, rotateY, rotateZ, tempY;
    bool rec, keyDown;
    int frameCount;
    ofPoint center;
    void crosshair(int size, ofColor color);
    
    // fix oscillator

    void audioOut( float * output, int bufferSize, int nChannels );
    ofSoundStream stream;
    float * soundBuffer;
    oscillator sinWave;
    vector < oscillator > clips;

    vector < bool > left;
    int cycleLength;
    
    float fullCircleTime;
    float rpf;

    int gridInterval;
    bool snap;
    ofPoint snapPoint;
    int previousY;
    
    ofSoundPlayer click;
    ofSoundPlayer wave;
    vector < ofSoundPlayer > waves;
    
    bool reverb;
    
};
