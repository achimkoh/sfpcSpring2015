#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    stream.setup(this, 2, 0, 44100, 512, 4);
    soundBuffer = new float[512];
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true); // what does this do?

    rotateX = 0;
    rotateY = 1;
    rotateZ = 0;
    
    rec = FALSE; // additional bool that lets me draw a point every frame even if mouse isn't moving

    keyDown = FALSE; // used to stop Y rotation when rotating on X axis, and restoring it
    tempY = 0;
    
    frameCount = 0; // i could use ofGetFrameNum() but i want it to stop when not rotating on Y axis

    click.loadSound("stick.wav");
    click.setVolume(0);
    
    rpf = 0.75;
    cycleLength = 360 / rpf;
    gridInterval = 50;
    snap = TRUE;
    
    center.x = ofGetWidth()/2;
    center.y = ofGetHeight()/2;
    
    reverb = TRUE;
    wave.loadSound("440hz.aiff");
    wave.setLoop(TRUE);
    wave.play();

}

//--------------------------------------------------------------
void ofApp::audioOut( float * output, int bufferSize, int nChannels ) {

        for (int i = 0; i < bufferSize; i++) {
            soundBuffer[i] = sinWave.getSample(); // current clip
            for (int j = 0; j < clips.size(); j++) {
                soundBuffer[i] += clips[j].getSample(); // every other clips
            }
            output[i*nChannels    ] = soundBuffer[i];
            output[i*nChannels + 1] = soundBuffer[i];
        }

}

//--------------------------------------------------------------
void ofApp::update(){
    
    wave.setSpeed(ofMap(ofGetMouseY(), ofGetHeight(), 0, 0.1, 8));
    
    if (rec) {
        snapPoint.x = ofGetMouseX();
        int newY = ofGetMouseY();
        if (snap) {
            if (ofGetMouseY() % gridInterval >= gridInterval / 2) newY = ofGetMouseY() + gridInterval - (ofGetMouseY() % gridInterval);
            else newY = ofGetMouseY() - (ofGetMouseY() % gridInterval);
        }
        snapPoint.y = previousY * 0.9 + newY * 0.1;
        previousY = snapPoint.y;
        sinWave.setFrequency(MAX(1, ofGetHeight() - snapPoint.y));

        line.addVertex(snapPoint);
        line.getSmoothed(1);
        
        if (line.size() > cycleLength-1) rec = FALSE;
        } else if (line.size() > 1) {
            clips.push_back(sinWave);
            lines.push_back(line);
            line.clear();
            sinWave.targetVolume = 0.0;
    }
    
    if (ofGetKeyPressed('a')) rotateX = 1;
    if (ofGetKeyPressed('s')) rotateX = -1;
    
    for (int i = 0; i < line.getVertices().size(); i++){

        ofPoint fromCenter = line.getVertices()[i] - ofPoint(ofGetWidth()/2, ofGetHeight()/2);
        ofMatrix4x4 rotateMatrix;
        rotateMatrix.makeRotationMatrix(rpf, rotateX, rotateY, rotateZ);
        ofPoint rot = fromCenter * rotateMatrix + ofPoint(ofGetWidth()/2, ofGetHeight()/2);
        line.getVertices()[i] = rot;
    }
    
    for (int i = 0; i < lines.size(); i++){
        for (int j = 0; j < lines[i].getVertices().size(); j++){
            ofPoint fromCenter = lines[i].getVertices()[j] - ofPoint(ofGetWidth()/2, ofGetHeight()/2);
            ofMatrix4x4 rotateMatrix;
            rotateMatrix.makeRotationMatrix(rpf, rotateX, rotateY, rotateZ);
            ofPoint rot = fromCenter * rotateMatrix + ofPoint(ofGetWidth()/2, ofGetHeight()/2);
            lines[i].getVertices()[j] = rot;
        }
    }

    if (keyDown == FALSE) frameCount++;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(127,127,127);

    for (int i = 0; i < (ofGetHeight()/gridInterval) - 1; i++) {
        ofSetLineWidth(1);
        ofLine(0, (i+1)*gridInterval, ofGetWidth(), (i+1)*gridInterval);
    }
    
    ofCircle(ofGetMouseX(), ofGetMouseY(), 2);
    if (rec) ofCircle(ofGetMouseX(), ofGetMouseY(), 5);
    
    if (frameCount % 30 == 0) click.play();
    int crosshairTime = 10;
    if (((frameCount+90) % 120) < crosshairTime) crosshair(4, ofColor::black);
    if (((frameCount+60) % 120) < crosshairTime) crosshair(6, ofColor::black);
    if (((frameCount+30) % 120) < crosshairTime) crosshair(8, ofColor::black);
    if ((frameCount % 120) < crosshairTime) {
        ofSetColor(255,0,0);
        ofCircle(ofPoint(ofGetWidth()/2, ofGetHeight()/2), 5);
        crosshair(10, ofColor::black);
    }
    
    if (line.size() > 1) {
        for (int i = 0; i < line.size() - 1; i++){
            ofPoint pta = line[i];
            ofPoint ptb = line[i+1];
            
            ofSetColor(ofMap(pta.z, 800, -800, 127, 0));
            ofSetLineWidth(ofMap(pta.z, -800, 800, 1, 5));
            ofLine(pta, ptb);
        }
    }
    
    for (int i = 0; i < lines.size(); i++){

        bool playing = FALSE;
        for (int j = 0; j < lines[i].size()-1; j++){
            ofPoint pta = lines[i][j];
            ofPoint ptb = lines[i][j+1];
            if ((pta.z < 0 && ptb.z < 0) || (pta.z >= 0 && ptb.z >= 0)) {
                ofSetColor(ofMap(pta.z, 800, -800, 127, 0));
                ofSetLineWidth(ofMap(pta.z, -800, 800, 1, 5));
                ofLine(pta, ptb);
            } else {
                ofPoint front, back;
                if (pta.z > ptb.z) {
                    front = pta;
                    back = ptb;
                } else {
                    front = ptb;
                    back = pta;
                }
                if ((left[i] && pta.x < ofGetWidth()/2) || (!left[i] && pta.x > ofGetWidth()/2)) {
                    clips[i].setFrequency(MAX(1, ofGetHeight() - pta.y));
                    playing = TRUE;
                    
                    ofPoint crossing = (pta + ptb)/2;
                    
                    ofSetColor(ofMap(back.z, 800, -800, 127, 0));
                    ofSetLineWidth(ofMap(back.z, -800, 800, 1, 5));
                    ofLine(back, crossing);
                    ofSetColor(255,0,0);
                    ofCircle(crossing.x, crossing.y, 5);
                    ofSetColor(ofMap(front.z, 800, -800, 127, 0));
                    ofSetLineWidth(ofMap(front.z, -800, 800, 1, 5));
                    ofLine(front, crossing);
                } else {
                    ofSetColor(ofMap(pta.z, 800, -800, 127, 0));
                    ofSetLineWidth(ofMap(pta.z, -800, 800, 1, 5));
                    ofLine(pta, ptb);
                }
            }
        }
        if (playing) clips[i].targetVolume = 0.5;
        else clips[i].targetVolume = 0;

    }
    
    ofDrawBitmapStringHighlight("a,s to rotate on X axis", ofPoint(50,50));
    ofDrawBitmapStringHighlight("r to reverse playback", ofPoint(50,70));
    ofDrawBitmapStringHighlight("spacebar to undo", ofPoint(50,90));
    ofDrawBitmapStringHighlight("click sound on/off: m/n", ofPoint(50,110));

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

    if (key == ' ') if (lines.size() > 0) {
        lines.pop_back();
        clips.pop_back();
        left.pop_back();
    }
    if (key == 'r') rotateY *= -1;
    if (key == 'a' || key == 's') {
        if (!keyDown) {
            tempY = rotateY;
            keyDown = TRUE;
        }
        rotateY = 0;
    }
    if (key == 'n') click.setVolume(0);
    if (key == 'm') click.setVolume(0.5);
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    rotateX = 0;
    if (key == 'a' || key == 's') rotateY = tempY;
    keyDown = FALSE;
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    if (abs(ofGetWidth()/2 - x) >= 5) {
        
        previousY = y;
        snapPoint = ofPoint(x,y);
        rec = TRUE;
        if (x <= ofGetWidth()/2) left.push_back(TRUE);
        else left.push_back(FALSE);
        
        sinWave.setup(44100);
        sinWave.targetVolume = 0.5;
        sinWave.setFrequency(MAX(1, ofGetHeight() - y));

    }

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    rec = FALSE;

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
