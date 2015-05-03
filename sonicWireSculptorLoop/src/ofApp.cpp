#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    rotateX = 0;
    rotateY = 1;
    rotateZ = 0;
    tempY = 0;
    keyDown = FALSE; // used to store Y rotation
    
    rotAngle = 0.75;
    
    gridInterval = 20;
    
    counter = 0;
    
    metronome.loadSound("stick.wav");
    metronome.setVolume(0.3);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    counter += rotateY;
    
    for (int wireNum = 0; wireNum < wires.size(); wireNum++) {
        wires[wireNum].update(rotAngle, rotateX, rotateY, rotateZ, counter);
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // background reacts to total volume
    ofBackground(0);
    float s = *ofSoundGetSpectrum(8);
    ofBackgroundGradient(ofColor(127,127), ofColor(ofMap(s,0,1,0,255)));
    
    // draw grid
    ofSetColor(127,127,127);
    for (int i = 0; i < (ofGetHeight()/gridInterval) - 1; i++) {
        ofSetLineWidth(1);
        ofLine(0, (i+1)*gridInterval, ofGetWidth(), (i+1)*gridInterval);
    }
    
    if (counter % 30 == 0) metronome.play();
    
    ofSetColor(0);
    ofEllipse(400, 750, 100, 40);
    ofLine(400, 450, 400, 750);

    // crosshair (serves as click)
    crosshair(4, 10, ofColor::black, ofColor::red);

    // cursor
    ofSetColor(255);
    ofCircle(ofGetMouseX(), ofGetMouseY(), 2);
    
    for (int wireNum = 0; wireNum < wires.size(); wireNum++) {
        wires[wireNum].draw();
    }
    
    // instructions
    ofDrawBitmapStringHighlight("z<->x to rotate on X axis", ofPoint(20,20));
    ofDrawBitmapStringHighlight("r to reverse playback, s to stop playback", ofPoint(20,40));
    ofDrawBitmapStringHighlight("spacebar to undo, m to toggle metronome", ofPoint(20,60));
}

//--------------------------------------------------------------
void ofApp::crosshair(int size, int time, ofColor color1, ofColor color2){
    
    int beatLength = (360 / rotAngle) / 16;
    int frameCount = ofGetFrameNum();
    
    if (((frameCount+3*beatLength) % (4*beatLength)) < time) size = size;
    if (((frameCount+2*beatLength) % (4*beatLength)) < time) size *= 1.5;
    if (((frameCount+beatLength) % (4*beatLength)) < time) size *= 2;
    if ((frameCount % (4*beatLength)) < time) {
        size *= 2.5;
        ofSetColor(color2);
        ofCircle(ofPoint(ofGetWidth()/2, ofGetHeight()/2), size/2);
    }
    
    if ((frameCount % (beatLength)) < time) {
        ofSetColor(color1);
        ofPoint center = ofPoint(ofGetWidth()/2, ofGetHeight()/2);
        ofLine(center.x-size, center.y, center.x+size, center.y);
        ofLine(center.x, center.y-size, center.x, center.y+size);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    // undo
    if (key == ' ') if (wires.size() > 0) {
        wires.pop_back();
    }
    
    // y rotation control
    if (key == 'r') {
        rotateY *= -1;
        for (int wireNum = 0; wireNum < wires.size(); wireNum++) {
            wires[wireNum].reverse(rotateY);
        }
    }
    
    if (key == 's') { // drawing while stopped will cause awkward sound
        if (rotateY) {
            tempY = rotateY;
            rotateY = 0;
        } else rotateY = tempY;
    }

    // x rotation
    if (key == 'z' || key == 'x') {
        if (!keyDown) {
            if (rotateY) tempY = rotateY;
            keyDown = TRUE;
        }
        rotateY = 0;
        if (key == 'z') rotateX = 1;
        else rotateX = -1;
    }
    
    // click
    if (key == 'm') {
        if (metronome.getVolume() != 0) metronome.setVolume(0);
        else metronome.setVolume(0.3);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    // x rotation stop
    
    if (key == 'z' || key == 'x') {
        rotateY = tempY;
        rotateX = 0;
        keyDown = FALSE;
    }
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    sonicWire wire;
    wires.push_back(wire);
    wires.back().startRec(counter, rotAngle);
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    wires.back().stopRec();
    if (wires.back().noLine()) wires.pop_back();

}