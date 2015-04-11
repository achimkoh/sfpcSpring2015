#pragma once

#include "ofMain.h"
#include "ofxGui.h"

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
    
        void square(int x, int y, int squareSize);
        void drawCat(int x, int y, float scale);

        void circleResolutionChanged(int & circleResolution);
        void framerateChanged(int & framerate);

        int width, height;

        ofPoint origin;
        float angle;
    
   	ofParameter<int> framerate;
    ofParameter<float> catDistance;
    ofParameter<float> catScale;
    ofParameter<float> r;
    ofParameter<float> g;
    ofParameter<float> b;
    ofParameter<ofColor> color1;
    ofParameter<int> colorRandomness;
   	ofParameter<int> circleResolution;
    ofParameter<bool> rotate;
    ofParameter<bool> counterclockwise;
    ofParameter<float> rotateSpeed;
    
    ofxPanel gui;
    
    ofSoundPlayer meow;
    
};
