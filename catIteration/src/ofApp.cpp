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
    
    // make grid and draw cats inside grid (rather than on the intersecting points)
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

    // set the center of cat to current iteration point in draw()
    ofPoint catCenter;
    catCenter.set(x, y);
    
    // this variable is used to replace hard-coded numbers and make cat scalable. division by 8 has no special meaning, it just makes the math easier
    int catUnit = width/8*scale;
    
    // this is used for inverting x position, so as to make a horizontally symmetrical drawing
    int xDirection = 1;
    
    // ears: half-circles via iteration, symmetrical
    for (int c = 0; c < 2; c++) {
        for (int i = 0; i < 2*catUnit; i++) {
            for (int j = 0; j < catUnit; j++) {
                
                // compares distance(diagonal and horizontal) between current iteration point and center of ear
                ofPoint current;
                ofPoint earCenter;
                current.set(catCenter.x+(catUnit+j)*xDirection,catCenter.y-3*catUnit+i);
                earCenter.set(catCenter.x+2*catUnit*xDirection, catCenter.y-2*catUnit);
                float earDistance = (current - earCenter).length();
                float earDistanceX = abs(current.x - earCenter.x);
                
                // draw different colors for specific parts of ear
                bool withinEarRange = earDistance <= catUnit;
                bool isEarOutline = earDistance > 0.75*catUnit || earDistanceX < abs(0.25*catUnit*xDirection);
                if (withinEarRange == TRUE) {
                    if (isEarOutline == TRUE) {
                        ofSetColor(ofColor::black);
                        ofRect(current, 1, 1);
                    }
                    else {
                        ofSetColor(ofColor::mistyRose);
                        ofRect(current, 1, 1);
                    }
                }
            }
        }
        xDirection *= -1;
    }
    
    // head
    ofSetColor(ofColor::black);
    ofCircle(catCenter.x, catCenter.y, 2.4*catUnit);
    ofSetColor(ofRandom(150,250),ofRandom(150,250),ofRandom(150,250));
    ofCircle(catCenter.x, catCenter.y, 2*catUnit);
    
    // face: symmetrical elements
    for (int i = 0; i < 2; i++) {
        ofSetColor(ofColor::black);
        // hair
        ofCircle(catCenter.x + 0.3*catUnit*xDirection, catCenter.y-2*catUnit, 0.35*catUnit);
        // eyes
        ofCircle(catCenter.x-catUnit*xDirection,catCenter.y-catUnit/2,catUnit*0.55);
        ofSetColor(ofColor::white);
        ofCircle(catCenter.x-catUnit*xDirection,catCenter.y-catUnit/2,catUnit*0.45);
        ofSetColor(ofColor::black);
        ofCircle(catCenter.x-catUnit*xDirection,catCenter.y-catUnit/2,catUnit*0.35);
        // mouth outline
        ofCircle(catCenter.x + 0.4*catUnit*xDirection, catCenter.y+0.8*catUnit, catUnit/2);
        // cheeks
        ofSetColor(ofRandom(200,250),ofRandom(200,250),ofRandom(200,250));
        ofCircle(catCenter.x + 0.6*catUnit*xDirection, catCenter.y+catUnit*2/3, 0.6*catUnit);
        // whiskers
        float whisker = 0;
        ofSetColor(ofColor::black);
        ofSetLineWidth(catUnit/10);
        for (int j = 0; j < 3; j++) {
            ofLine(catCenter.x+catUnit*xDirection, catCenter.y+0.5*catUnit, catCenter.x+3*catUnit*xDirection, catCenter.y + whisker*catUnit);
            whisker += 0.5;
        }
        xDirection *= -1;
    }
    
    // nose(outline)
    ofSetColor(ofColor::black);
    ofCircle(catCenter.x, catCenter.y+0.4*catUnit, 0.4*catUnit);
    square(catCenter.x, catCenter.y, 0.8*catUnit);
    // nose
    ofSetColor(ofRandom(200,250),ofRandom(200,250),ofRandom(200,250));
    ofCircle(catCenter.x, catCenter.y+0.3*catUnit, 0.35*catUnit);
    square(catCenter.x, catCenter.y-0.05*catUnit, 0.7*catUnit);
    
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
    
    // reset grid and cat size to default
/*    catDistance = 200;
    catScale = 0.3;
*/
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
