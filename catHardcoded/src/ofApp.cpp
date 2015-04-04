#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetCircleResolution(32);
    ofSetFrameRate(12);

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // this is a hard-coded cat drawing, like the one i made before making loops.

    // background
    ofSetColor(ofColor::white);
    ofRect(400,400,800,800);

    // ears
    ofSetColor(ofColor::black);
    ofCircle(200, 200, 100);
    ofCircle(600, 200, 100);
    
    ofSetColor(ofColor::white);
    ofRect(100, 200, 200, 200);
    ofRect(700, 200, 200, 200);
    
    // head
    ofSetColor(ofColor::black);
    ofCircle(400, 400, 240);
    ofSetColor(ofRandom(150,250),ofRandom(150,250),ofRandom(150,250));
    ofCircle(400, 400, 200);
    
    // hair
    ofSetColor(ofColor::black);
    ofCircle(430, 200, 35);
    ofCircle(370, 200, 35);
 
    // eyes
    ofSetColor(ofColor::black);
    ofCircle(300,350,55);
    ofCircle(500,350,55);
    ofSetColor(ofColor::white);
    ofCircle(300,350,45);
    ofCircle(500,350,45);
    ofSetColor(ofColor::black);
    ofCircle(300,350,35);
    ofCircle(500,350,35);
    
    // mouth outline
    ofSetColor(ofColor::black);
    ofCircle(440, 480, 50);
    ofCircle(360, 480, 50);
    // cheeks
    ofSetColor(ofRandom(200,250),ofRandom(200,250),ofRandom(200,250));
    ofCircle(460, 467, 60);
    ofCircle(340, 467, 60);

    // whiskers
    ofSetColor(ofColor::black);
    ofSetLineWidth(10);
    ofLine(500, 450, 700, 400);
    ofLine(500, 450, 700, 450);
    ofLine(500, 450, 700, 500);
    ofLine(300, 450, 100, 400);
    ofLine(300, 450, 100, 450);
    ofLine(300, 450, 100, 500);
    
    // nose
    ofSetColor(ofColor::black);
    ofCircle(400, 440, 40);
    ofRect(400, 400, 80, 80);
    ofSetColor(ofRandom(200,250),ofRandom(200,250),ofRandom(200,250));
    ofCircle(400, 430, 35);
    ofRect(400, 395, 70, 70);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
