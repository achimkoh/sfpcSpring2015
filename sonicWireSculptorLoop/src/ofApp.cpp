#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true); // what does this do?
    
    rotateX = 0;
    rotateY = 1;
    rotateZ = 0;
    
    rec = FALSE; // additional bool that lets me draw a point every frame even if mouse isn't moving
    
    keyDown = FALSE; // used to stop Y rotation when rotating on X axis, and restoring it
    tempY = 0;
    
    frameCount = 0; // i could use ofGetFrameNum() but i want it to stop when not rotating on Y axis
    
    rotAngle = 0.75;
    cycleLength = 360 / rotAngle;
    beatLength = cycleLength / 16;
    
    gridInterval = 20;
    
    snap = TRUE;
    
    wave.loadSound("440hz.aiff");
    wave.setLoop(TRUE);
    wave.setVolume(0);
    wave.play();
    
}

//-----
float ofApp::pitch(int y){
    
    float pitchReturn = ofMap(y, ofGetHeight(), 0, 0.0625, 2);
    return pitchReturn;
    
}
//-----
float ofApp::pan(int x){
    
    float panReturn = ofMap(ofGetWidth()/2 - x, ofGetWidth()/2, -1*ofGetWidth()/2, -0.5, 0.5);
    return panReturn;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (ofGetKeyPressed('z')) rotateX = 1;
    if (ofGetKeyPressed('x')) rotateX = -1;
    
    // you can't cross the center of the screen horizontally
    // eventually, ditch this limitation by dealing with playheads where x == ofGetWidth/2 differently?
    if (curLeft) {
        if (ofGetMouseX() > (ofGetWidth()/2) - 1 ) rec = FALSE;
    } else {
        if (ofGetMouseX() < (ofGetWidth()/2) + 1 ) rec = FALSE;
    }
    
    // record current line: add points, play sound, add to vector when done
    if (rec) {
        wave.setVolume(0.3);
        snapPoint.x = ofGetMouseX();
        int newY = ofGetMouseY();
        wave.setSpeed(pitch(newY));
        wave.setPan(pan(snapPoint.x));
        if (snap) {
            if (ofGetMouseY() % gridInterval >= gridInterval / 2) newY = ofGetMouseY() + gridInterval - (ofGetMouseY() % gridInterval);
            else newY = ofGetMouseY() - (ofGetMouseY() % gridInterval);
        }
        snapPoint.y = previousY * 0.85 + newY * 0.15;
        previousY = snapPoint.y;
        
        line.addVertex(snapPoint);
        line.getSmoothed(1);
        
        if (line.size() > cycleLength-1) rec = FALSE;
    } else {
        wave.setVolume(0);
        if (line.size() > 1) {
            lines.push_back(line);
            left.push_back(curLeft);
            waves.push_back(ofSoundPlayer());
            waves.back().loadSound("440hz.aiff");
            waves.back().setLoop(TRUE);
            waves.back().setVolume(0);
            waves.back().play();
        }
        line.clear();
    }
    
    // rotate current line by rotAngle
    for (int i = 0; i < line.getVertices().size(); i++){
        ofPoint fromCenter = line.getVertices()[i] - ofPoint(ofGetWidth()/2, ofGetHeight()/2);
        ofMatrix4x4 rotateMatrix;
        rotateMatrix.makeRotationMatrix(rotAngle, rotateX, rotateY, rotateZ);
        ofPoint rot = fromCenter * rotateMatrix + ofPoint(ofGetWidth()/2, ofGetHeight()/2);
        line.getVertices()[i] = rot;
    }
    
    // rotate every other lines by rotAngle
    for (int i = 0; i < lines.size(); i++){
        for (int j = 0; j < lines[i].getVertices().size(); j++){
            ofPoint fromCenter = lines[i].getVertices()[j] - ofPoint(ofGetWidth()/2, ofGetHeight()/2);
            ofMatrix4x4 rotateMatrix;
            rotateMatrix.makeRotationMatrix(rotAngle, rotateX, rotateY, rotateZ);
            ofPoint rot = fromCenter * rotateMatrix + ofPoint(ofGetWidth()/2, ofGetHeight()/2);
            lines[i].getVertices()[j] = rot;
        }
    }
    
    if (keyDown == FALSE) frameCount++;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // background
    ofSetColor(127,127,127);
    
    for (int i = 0; i < (ofGetHeight()/gridInterval) - 1; i++) {
        ofSetLineWidth(1);
        ofLine(0, (i+1)*gridInterval, ofGetWidth(), (i+1)*gridInterval);
    }
    
    // cursor
    ofCircle(ofGetMouseX(), ofGetMouseY(), 2);
    if (rec) ofCircle(ofGetMouseX(), ofGetMouseY(), 5);
    
    // crosshair (serves as click)
    // todo: hook up numbers w/ rotAngle
    int crosshairTime = 10;
    if (((frameCount+3*beatLength) % (4*beatLength)) < crosshairTime) crosshair(4, ofColor::black);
    if (((frameCount+2*beatLength) % (4*beatLength)) < crosshairTime) crosshair(6, ofColor::black);
    if (((frameCount+beatLength) % (4*beatLength)) < crosshairTime) crosshair(8, ofColor::black);
    if ((frameCount % (4*beatLength)) < crosshairTime) {
        ofSetColor(255,0,0);
        ofCircle(ofPoint(ofGetWidth()/2, ofGetHeight()/2), 5);
        crosshair(10, ofColor::black);
    }
    
    // draw current line
    if (line.size() > 1) {
        for (int i = 0; i < line.size() - 1; i++){
            ofPoint pta = line[i];
            ofPoint ptb = line[i+1];
            
            ofSetColor(ofMap(pta.z, 800, -800, 127, 0));
            ofSetLineWidth(ofMap(pta.z, -800, 800, 1, 5));
            ofLine(pta, ptb);
        }
    }
    
    // draw every other lines
    for (int i = 0; i < lines.size(); i++){
        
        bool playing = FALSE;
        
        ofPoint crossing;
        
        for (int j = 0; j < lines[i].size()-1; j++){
            ofPoint pta = lines[i][j];
            ofPoint ptb = lines[i][j+1];
            
            ofSetColor(ofMap(pta.z, -800, 800, 127, 0));
            ofSetLineWidth(ofMap(pta.z, -800, 800, 1, 5));
            ofLine(pta, ptb);
            
            if (!(pta.z < 0 && ptb.z < 0) && !(pta.z >= 0 && ptb.z >= 0)) {
                if ((left[i] && pta.x < ofGetWidth()/2) || (!left[i] && pta.x > ofGetWidth()/2)) {
                    playing = TRUE;
                    crossing = (pta + ptb)/2;
                    ofSetColor(255,0,0);
                    ofCircle(crossing.x, crossing.y, 5);
                    // eventually deal with center points
                    // playheads.push_back(crossing);
                    waves[i].setSpeed(pitch(crossing.y));
                }
            }
        }
        
        // set volume and pan of each player
        float vol = waves[i].getVolume();
        if (playing) {
            if (vol != 0.3) waves[i].setVolume(0.3);
            waves[i].setPan(pan(crossing.x));
        } else {
            if (vol != 0) waves[i].setVolume(0.8*waves[i].getVolume());
        }
    }
    
    // instructions
    ofDrawBitmapStringHighlight("z<->x to rotate on X axis", ofPoint(20,20));
    ofDrawBitmapStringHighlight("r to reverse playback, s to stop playback", ofPoint(20,40));
    ofDrawBitmapStringHighlight("spacebar to undo", ofPoint(20,60));
    ofDrawBitmapStringHighlight(" ", ofPoint(20,80));
    
}

//--------------------------------------------------------------
void ofApp::crosshair(int size, ofColor color){
    
    ofSetColor(color);
    ofPoint center = ofPoint(ofGetWidth()/2, ofGetHeight()/2);
    ofLine(center.x-size, center.y, center.x+size, center.y);
    ofLine(center.x, center.y-size, center.x, center.y+size);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    // undo
    if (key == ' ') if (lines.size() > 0) {
        lines.pop_back();
        waves.pop_back();
        left.pop_back();
    }
    
    // y rotation
    if (key == 'r') rotateY *= -1;
    if (key == 's') {
        if (rotateY == 0) rotateY = 1;
        else rotateY = 0;
    }
    
    // x rotation
    if (key == 'z' || key == 'x') {
        if (!keyDown) {
            tempY = rotateY;
            keyDown = TRUE;
        }
        rotateY = 0;
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
    
    if (abs(ofGetWidth()/2 - x) >= 5) {
        previousY = y;
        snapPoint = ofPoint(x,y);
        rec = TRUE;
        if (x <= ofGetWidth()/2) curLeft = TRUE;
        else curLeft = FALSE;
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    rec = FALSE;
    
}