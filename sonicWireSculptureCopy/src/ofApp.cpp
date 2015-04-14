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
    
    mouseDown = FALSE; // additional bool that lets me draw a point every frame even if mouse isn't moving

    keyDown = FALSE; // used to stop Y rotation when rotating on X axis, and restoring it
    tempY = 0;
    
    frameCount = 0; // i could use ofGetFrameNum() but i want it to stop when not rotating on Y axis

    click.loadSound("4d.wav");
    click.setVolume(0);
    
}

//--------------------------------------------------------------
void ofApp::audioOut( float * output, int bufferSize, int nChannels ) {

        for (int i = 0; i < bufferSize; i++) {
        
//            float sample;
            soundBuffer[i] = sinWave.getSample(); // current line

            for (int j = 0; j < oscillators.size(); j++) {
//                soundBuffer[i+j+1] = oscillators[j].getSample();
                soundBuffer[i] += oscillators[j].getSample();
            }
        }

        for (int i = 0; i < bufferSize; i++) {
            output[i*nChannels    ] = soundBuffer[i]; // / (oscillators.size()+1);
            output[i*nChannels + 1] = soundBuffer[i]; // / (oscillators.size()+1);
        }

}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (mouseDown) line.addVertex(ofPoint(ofGetMouseX(),ofGetMouseY()));
    
    if (ofGetKeyPressed('a')) rotateX = 1;
    if (ofGetKeyPressed('s')) rotateX = -1;
    
    for (int i = 0; i < line.getVertices().size(); i++){
        ofPoint fromCenter = line.getVertices()[i] - ofPoint(ofGetWidth()/2, ofGetHeight()/2);
        ofMatrix4x4 rotateMatrix;
        rotateMatrix.makeRotationMatrix(0.375, rotateX, rotateY, rotateZ);
        ofPoint rot = fromCenter * rotateMatrix + ofPoint(ofGetWidth()/2, ofGetHeight()/2);
        line.getVertices()[i] = rot; // this crashes sometimes. maybe needs some sort of buffer that causes the line to have a certain minimum amount of points?
    }
    
    for (int i = 0; i < lines.size(); i++){
        for (int j = 0; j < lines[i].getVertices().size(); j++){
            ofPoint fromCenter = lines[i].getVertices()[j] - ofPoint(ofGetWidth()/2, ofGetHeight()/2);
            ofMatrix4x4 rotateMatrix;
            rotateMatrix.makeRotationMatrix(0.375, rotateX, rotateY, rotateZ);
            ofPoint rot = fromCenter * rotateMatrix + ofPoint(ofGetWidth()/2, ofGetHeight()/2);
            lines[i].getVertices()[j] = rot;
        }
    }

    if (keyDown == FALSE) frameCount++;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackgroundGradient(255, 127);
    ofSetColor(127,127,127);

    for (int i = 0; i < (ofGetHeight()/50) - 1; i++) {
        ofSetLineWidth(1);
        ofLine(0, (i+1)*50, ofGetWidth(), (i+1)*50);
    }
    ofCircle(ofGetMouseX(), ofGetMouseY(), 2);
    if (mouseDown) ofCircle(ofGetMouseX(), ofGetMouseY(), 5);
    
    // click
    if (abs(frameCount%30 == 0)) click.play();
    if (abs(frameCount%30) < 5) ofCircle(ofGetWidth()/2,ofGetHeight()/2,10);
    if (abs(frameCount%120) < 5) ofCircle(ofGetWidth()/2,ofGetHeight()/2,30);
    
    vector < ofPoint > crossings;
    
    if (line.getVertices().size() > 0) {
        for (int i = 0; i < line.getVertices().size() -1; i++){
            ofPoint pta = line.getVertices()[i];
            ofPoint ptb = line.getVertices()[i+1];
            
            if (pta.z < 0 && ptb.z < 0){
                ofSetColor(127,127,127);
                ofSetLineWidth(1);
                ofLine(pta, ptb);
            } else if (pta.z >= 0 && ptb.z >= 0){
                ofSetColor(0,0,0);
                ofSetLineWidth(ofMap(pta.z, 0, 800, 2, 10));
                ofLine(pta, ptb);
            } else {
                
                ofPoint smallest = pta.z < ptb.z ? pta : ptb;
                ofPoint biggest = pta.z < ptb.z ? ptb : pta;
                
                float diffZ = (biggest.z - smallest.z);     // ie, (3 - -1 = 4);
                if (diffZ < 0.0001) diffZ = 0.0001;
                float pctZ = abs(smallest.z) / diffZ;
                
                ofPoint crossing = smallest + pctZ * (biggest - smallest);
                
                ofSetColor(127,127,127);
                ofLine(smallest, crossing);
                
                ofSetColor(255,0,0);
                ofCircle(crossing.x, crossing.y, 5);
                
                ofSetColor(0,0,0);
                ofLine(crossing, biggest);
                
            }
        }
    }
    
    for (int i = 0; i < lines.size(); i++){
        for (int j = 0; j < lines[i].getVertices().size()-1; j++){
            ofPoint pta = lines[i].getVertices()[j];
            ofPoint ptb = lines[i].getVertices()[j+1];
            
            if (pta.z < 0 && ptb.z < 0){
                ofSetColor(127,127,127);
                ofSetLineWidth(1);
                ofLine(pta, ptb);
                
            } else if (pta.z >= 0 && ptb.z >= 0){
                ofSetColor(0,0,0);
                ofSetLineWidth(ofMap(pta.z, 0, 800, 2, 10));
                ofLine(pta, ptb);
                
            } else {
                ofPoint smallest = pta.z < ptb.z ? pta : ptb;
                ofPoint biggest = pta.z < ptb.z ? ptb : pta;
                
                float diffZ = (biggest.z - smallest.z);     // ie, (3 - -1 = 4);
                if (diffZ < 0.0001) diffZ = 0.0001;
                float pctZ = abs(smallest.z) / diffZ;
                
                ofPoint crossing = smallest + pctZ * (biggest - smallest);
                
                ofSetColor(127,127,127);
                ofLine(smallest, crossing);
                
                ofSetColor(255,0,0);
                ofCircle(crossing.x, crossing.y, 5);
                
                ofSetColor(0,0,0);
                ofLine(crossing, biggest);
                
                oscillators[i].setFrequency(MAX(1, ofGetWidth() - pta.y));
                if (j == 0) {
                   // if (ptb.z > 0)
                        oscillators[i].setVolume(0.5);
                }
                
                if (j == lines[i].getVertices().size()-2) oscillators[i].setVolume(0);
                
            }
        }
    }
    
    ofDrawBitmapStringHighlight("a,s to rotate on X axis", ofPoint(50,50));
    ofDrawBitmapStringHighlight("r to reverse playback", ofPoint(50,70));
    ofDrawBitmapStringHighlight("spacebar to undo", ofPoint(50,90));
    ofDrawBitmapStringHighlight("click sound on/off: m/n", ofPoint(50,110));

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == ' ') if (lines.size() > 0) {
        lines.pop_back();
        oscillators.pop_back();
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

    sinWave.setFrequency(MAX(1, ofGetHeight() - y));

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    mouseDown = TRUE;

    sinWave.setup(44100);
    sinWave.setVolume(0.5);
    sinWave.setFrequency(MAX(1, ofGetHeight() - y));

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    mouseDown = FALSE;

    lines.push_back(line);
    line.clear();

    sinWave.setVolume(0);
    
    oscillators.push_back(sinWave);
    
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
