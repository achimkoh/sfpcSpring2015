#include "sonicWire.h"

//--------------------------------------------------------

void sonicWire::update(float rotAngle, int rotateX, int rotateY, int rotateZ, int counter) {
    
    cur = counter % maxFrames;
    if (cur < 0) cur += maxFrames;
    
    // while recording, add current point to line and play it
    if (rec) {
        ofPoint mouse;
        mouse.set(ofGetMouseX(), ofGetMouseY());
        line.addVertex(mouse);
        wave.setPan(pan(mouse.x));
        wave.setSpeed(pitch(mouse.y));
        
    // while not recording, playback for the right duration
    } else {

        if (playing) {
            if (playhead > 0 && playhead < line.size()) play();
            else {
                stop();
                if (playhead == 0 || playhead == line.size()) playhead = line.size() - playhead;
            }
        } else {
            if (cur == start) play();
            else stop();
        }
    }
    
    // rotate!
    for (int i = 0; i < line.size(); i++){
        ofPoint fromCenter = line[i] - ofPoint(ofGetWidth()/2, ofGetHeight()/2);
        ofMatrix4x4 rotateMatrix;
        rotateMatrix.makeRotationMatrix(rotAngle, rotateX, rotateY, rotateZ);
        line[i] = fromCenter * rotateMatrix + ofPoint(ofGetWidth()/2, ofGetHeight()/2);
    }

}

//--------------------------------------------------------

void sonicWire::play() {
   
    playing = TRUE;
    
    wave.setVolume(0.5*wave.getVolume() + 0.5*0.3);
    wave.setPan(pan(line[playhead].x));
    wave.setSpeed(pitch(line[playhead].y));
    
    if (hasChangedDirection) playhead--;
    else playhead++;

}

//--------------------------------------------------------

void sonicWire::stop() {
    
    playing = FALSE;
    wave.setVolume(0.9*wave.getVolume());

    if (hasChangedDirection) playhead = line.size() - 1;
    else playhead = 0;
}

//--------------------------------------------------------

void sonicWire::reverse(int dir) {

    hasChangedDirection = !hasChangedDirection;
    start = (start - line.size() * dir) % maxFrames;

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
//        ofPoint pta = line[i];
//        ofPoint ptb = line[i+1];
        ofSetColor(ofMap(line[i].z, -400, 200, 127, 255, TRUE));
        ofCircle(line[i], ofMap(line[i].z, -800, 800, 1, 5));
        
        //        ofSetColor(ofMap(pta.z, -800, 800, 127, 0));
//        ofSetLineWidth(ofMap(pta.z, -800, 800, 1, 5));
//        ofLine(pta, ptb);
    } 
    if (rec) {
        ofSetColor(255,0,0);
        ofCircle(ofGetMouseX(), ofGetMouseY(), 5);
    } else if (playing) {
        ofSetColor(0,255,0);
        ofCircle(line[playhead], 5);
    }

}

//--------------------------------------------------------

void sonicWire::startRec(int counter, float rotAngle) {

    maxFrames = 360 / rotAngle;
    
    wave.loadSound("440hz.aiff");
    wave.setLoop(TRUE);
    
    wave.setPan(pan(ofGetMouseX()));
    wave.setSpeed(pitch(ofGetMouseY()));
    wave.setVolume(0.3);
    wave.play();

    start = counter % maxFrames;
    if (start < 0) start += maxFrames;

    rec = TRUE;
    playing = FALSE;
    hasChangedDirection = FALSE;
    
    playhead = 0;

}

//--------------------------------------------------------

void sonicWire::stopRec() {
    
    rec = FALSE;
    stop();
    
}

//--------------------------------------------------------

float sonicWire::pitch(int y){
    
    // chromatic scale according to grid
    int p = ofMap(y, ofGetHeight(), 0, 0, 80, TRUE);
    float q = p - 40;
    float tone = powf(2,q/12);
    
    return tone;
    
//    float pitchReturn = ofMap(y, ofGetHeight(), 0, 0.0625, 2);
//    return pitchReturn;
    
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