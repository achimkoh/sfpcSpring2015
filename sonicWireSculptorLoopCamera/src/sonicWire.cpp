#include "sonicWire.h"

//--------------------------------------------------------

void sonicWire::setup(float rotAngle) {
    
    playhead = 0;
    maxFrames = 360 / rotAngle;
    start = 0;

    wave.loadSound("440hz.aiff");
    wave.setLoop(TRUE);
    wave.setVolume(0);
    wave.play();
    
    rec = FALSE;
    playing = FALSE;

}

//--------------------------------------------------------

void sonicWire::update(float rotAngle, int rotateX, int rotateY, int rotateZ, int counter) {
    
    cur = counter % maxFrames;
    
    // add current point to line and play it
    if (rec) {
        ofPoint mouse;
        ofPoint pt = cam->screenToWorld( ofPoint(ofGetMouseX(), ofGetMouseY() ));
        //mouse.set(ofGetMouseX(), ofGetMouseY());
        line.addVertex(pt);
        wave.setPan(pan(mouse.x));
        wave.setSpeed(pitch(mouse.y));
        
        if (line.size() > maxFrames - 1) stopRec(counter);

    // play everything else
    } else { // is counting system stable enough?
        if (playhead == 0) stop();
        if (cur == start) play();
        if (playing) {
            if (playhead < line.size()) play();
            else stop();
        }
    }

    
    // rotate!
    for (int i = 0; i < line.size(); i++){
        ofPoint fromCenter = line.getVertices()[i] - ofPoint(ofGetWidth()/2, ofGetHeight()/2);
        ofMatrix4x4 rotateMatrix;
        rotateMatrix.makeRotationMatrix(rotAngle, rotateX, rotateY, rotateZ);
        line[i] = fromCenter * rotateMatrix + ofPoint(ofGetWidth()/2, ofGetHeight()/2);
    }

}

//--------------------------------------------------------

void sonicWire::play() {
    
    if (playhead < 0) playhead = 0;
    if (playhead > line.size()) playhead = line.size();
    playing = TRUE;
    
    wave.setVolume(0.5*wave.getVolume() + 0.5*0.3);
    wave.setPan(pan(line[playhead].x));
    wave.setSpeed(pitch(line[playhead].y));
    
    playhead++;
    
}

//--------------------------------------------------------

void sonicWire::stop() {
    
    playing = FALSE;
    wave.setVolume(0.9*wave.getVolume());

    playhead = 0;
}

//--------------------------------------------------------

void sonicWire::draw() {
    
    // using Shape function
    /*
    ofSetColor(0,0,0);
    ofNoFill();
    ofBeginShape();
    for (int pointNum = 0; pointNum < line.size(); pointNum++){
        ofVertex(line[pointNum].x, line[pointNum].y);
    }
    ofEndShape();
     */

    // original
    for (int i = 0; i < line.size() - 1; i++){
        ofPoint pta = line[i];
        ofPoint ptb = line[i+1];
        
        ofSetColor(ofMap(pta.z, -800, 800, 127, 0));
        ofSetLineWidth(ofMap(pta.z, -800, 800, 1, 5));
        ofLine(pta, ptb);
    } 
    if (rec) {
        ofSetColor(255,0,0);
        ofCircle(ofGetMouseX(), ofGetMouseY(), 5);
    } else if (playing) {
        ofSetColor(0,255,0);
        ofCircle(line[playhead].x, line[playhead].y, 5);
    }

}

//--------------------------------------------------------

void sonicWire::startRec(int counter, float rotAngle) {
    
    wave.setPan(pan(ofGetMouseX()));
    wave.setSpeed(pitch(ofGetMouseY()));
    wave.setVolume(0.3);

    start = counter % int(360 / rotAngle);
    playhead = 0;

    rec = TRUE;
    
}

//--------------------------------------------------------

void sonicWire::stopRec(int counter) {
    
    rec = FALSE;
    
}

//--------------------------------------------------------

float sonicWire::pitch(int y){
    
    float pitchReturn = ofMap(y, ofGetHeight(), 0, 0.0625, 2);
    return pitchReturn;
    
}

//--------------------------------------------------------

float sonicWire::pan(int x){
    
    float panReturn = ofMap(ofGetWidth()/2 - x, ofGetWidth()/2, -1*ofGetWidth()/2, -0.8, 0.8);
    return panReturn;
    
}

//--------------------------------------------------------

bool sonicWire::noLine(){
    
    return line.size() <= 1;
    
}