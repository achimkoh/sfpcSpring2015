#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    width = ofGetWindowWidth();
    height = ofGetWindowHeight();

    ofSetRectMode(OF_RECTMODE_CENTER);
    ofSetCircleResolution(32);
    ofSetFrameRate(12);
    
    catDistance = 200;
    catScale = 0.3;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    for (int i = 0; i < width/catDistance; i++) {
        for (int j = 0; j < height/catDistance; j++) {
            drawCat(catDistance*(i+0.5), catDistance*(j+0.5), catScale);
        }
    }
}

//--------------------------------------------------------------
void ofApp::square(int x, int y, int squareSize){
    ofRect(x, y, squareSize, squareSize);
}

//--------------------------------------------------------------
void ofApp::drawCat(int x, int y, float scale){


    ofPoint catCenter;
    catCenter.set(x, y);
    int catUnit = width/8*scale;
    int xMultiplier = 1;

    // ears
    // drawing half-circles via iteration
    for (int c = 0; c < 2; c++) {
        for (int i = 0; i < 2*catUnit; i++) {
            for (int j = 0; j < catUnit; j++) {
                ofPoint current;
                ofPoint earCenter;
                current.set(catCenter.x+(catUnit+j)*xMultiplier,catCenter.y-3*catUnit+i);
                earCenter.set(catCenter.x+2*catUnit*xMultiplier, catCenter.y-2*catUnit);
                float earMeasure = (current - earCenter).length();
                float earMeasureX = abs(current.x - earCenter.x);
                if (earMeasure <= catUnit) {
                    if (earMeasure > 0.75*catUnit || earMeasureX < abs(0.25*catUnit*xMultiplier)) {
                        ofSetColor(ofColor::black);
                        ofRect(current, 1, 1);
                    } else {
                        ofSetColor(ofColor::mistyRose);
                        ofRect(current, 1, 1);
                    }
                }
            }
        }
        xMultiplier *= -1;
    }

    // head
    ofSetColor(ofColor::black);
    ofCircle(catCenter.x, catCenter.y, 2.4*catUnit);
    ofSetColor(ofRandom(150,250),ofRandom(150,250),ofRandom(150,250));
    //    ofSetColor(ofColor::white);
    ofCircle(catCenter.x, catCenter.y, 2*catUnit);
    
    for (int i = 0; i < 2; i++) {
        ofSetColor(ofColor::black);
        // hair
        ofCircle(catCenter.x + 0.3*catUnit*xMultiplier, catCenter.y-2*catUnit, 0.35*catUnit);
        // eyes
        ofCircle(catCenter.x-catUnit*xMultiplier,catCenter.y-catUnit/2,catUnit*0.55);
        ofSetColor(ofColor::white);
        ofCircle(catCenter.x-catUnit*xMultiplier,catCenter.y-catUnit/2,catUnit*0.45);
        ofSetColor(ofColor::black);
        ofCircle(catCenter.x-catUnit*xMultiplier,catCenter.y-catUnit/2,catUnit*0.35);
        // mouth outline
        ofCircle(catCenter.x + 0.4*catUnit*xMultiplier, catCenter.y+0.8*catUnit, catUnit/2);
        // cheeks
        ofSetColor(ofRandom(200,250),ofRandom(200,250),ofRandom(200,250));
        ofCircle(catCenter.x + 0.6*catUnit*xMultiplier, catCenter.y+catUnit*2/3, 0.6*catUnit);
        xMultiplier *= -1;
    }
    
    // nose
    ofSetColor(ofColor::black);
    ofCircle(catCenter.x, catCenter.y+0.4*catUnit, 0.4*catUnit);
    square(catCenter.x, catCenter.y, 0.8*catUnit);
    ofSetColor(ofRandom(200,250),ofRandom(200,250),ofRandom(200,250));
    //    ofSetColor(ofColor::white);
    ofCircle(catCenter.x, catCenter.y+0.3*catUnit, 0.35*catUnit);
    square(catCenter.x, catCenter.y-0.05*catUnit, 0.7*catUnit);
    
    // whiskers
    ofSetColor(ofColor::black);
    ofSetLineWidth(catUnit/10);
        for (int i = 0; i < 2; i++) {
            float whisker = 0;
            for (int j = 0; j < 3; j++) {
                ofLine(catCenter.x+catUnit*xMultiplier, catCenter.y+0.5*catUnit, catCenter.x+3*catUnit*xMultiplier, catCenter.y + whisker*catUnit);
                whisker += 0.5;
            }
        xMultiplier *= -1;
    }
 
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

/*
    float rD = (width/2 - ofGetMouseX())/50;
    float rS = ofMap(abs(height/2 - ofGetMouseY()), 0, 1500, 0.97, 1.2);
    catDistance -= rD;
    catScale *= rS;
*/

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    catDistance = 200;
    catScale = 0.3;
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
