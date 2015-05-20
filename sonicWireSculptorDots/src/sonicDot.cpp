#include "sonicDot.h"

void sonicDot::update(float rotAngle, int rotateX, int rotateY, int rotateZ, int counter) {
    
    cur = counter % maxFrames;
    if (cur < 0) cur += maxFrames;
    
    if (rec) radius++;
    else if (cur == start) wave.play();
    else wave.stop();

    ofPoint fromCenter = center - ofPoint(ofGetWidth()/2, ofGetHeight()/2);
    ofMatrix4x4 rotateMatrix;
    rotateMatrix.makeRotationMatrix(rotAngle, rotateX, rotateY, rotateZ);
    center = fromCenter * rotateMatrix + ofPoint(ofGetWidth()/2, ofGetHeight()/2);

}

//--------------------------------------------------------

void sonicDot::draw() {

    ofSetColor(ofColor::white);
    ofCircle(center, radius);

}

//--------------------------------------------------------

void sonicDot::startRec(int counter, float rotAngle) {
    
    maxFrames = 360 / rotAngle;
    start = counter % maxFrames;
    if (start < 0) start += maxFrames;

    radius = 0;
    
    center = ofPoint(ofGetMouseX(), ofGetMouseY());
    
    wave.loadSound("440hz.aiff");
    
    wave.setPan(pan(ofGetMouseX()));
    wave.setSpeed(pitch(ofGetMouseY()));
    wave.setVolume(0.3);
    wave.play();
    
    rec = TRUE;
    
}

//--------------------------------------------------------

void sonicDot::stopRec() {
    
    rec = FALSE;
    
}

//--------------------------------------------------------

float sonicDot::pitch(int y){
    
    // chromatic scale according to grid
    int p = ofMap(y, ofGetHeight(), 0, 0, 80, TRUE);
    float q = p - 40;
    float tone = powf(2,q/12);
    
    return tone;
    
}

//--------------------------------------------------------

float sonicDot::pan(int x){
    
    float panReturn = ofMap(ofGetWidth()/2 - x, ofGetWidth()/2, -1*ofGetWidth()/2, -0.8, 0.8);
    return panReturn;
    
}