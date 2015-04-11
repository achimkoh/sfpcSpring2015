#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    circleResolution.addListener(this, &ofApp::circleResolutionChanged);
    framerate.addListener(this, &ofApp::framerateChanged);

    width = ofGetWindowWidth();
    height = ofGetWindowHeight();
    
    gui.setup("panel");
    gui.add(framerate.set("framerate", 60, 6, 60));
    gui.add(r.set("R", 1, 0.5, 1));
    gui.add(g.set("G", 1, 0.5, 1));
    gui.add(b.set("B", 1, 0.5, 1));
    gui.add(colorRandomness.set("colorRandomness",100,0,200));
    gui.add(circleResolution.set("circleRes", 32, 3, 90));
    gui.add(catScale.set("scale", 0.3, 0.1, 1));
    gui.add(catDistance.set("distance", 200, 50, 1000));
    gui.add(rotate.set("rotate",FALSE));
    gui.add(counterclockwise.set("counterclockwise",FALSE));
    gui.add(rotateSpeed.set("rotateSpeed", 5, 0.1, 30));
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackgroundGradient(255,128);
    
    for (int i = 0; i < width/catDistance; i++) {
        for (int j = 0; j < height/catDistance; j++) {
            origin.x = catDistance*(i+0.5);
            origin.y = catDistance*(j+0.5);

            ofPushMatrix();
                ofTranslate(origin);
//            if (rotate) {
//                ofRotateZ(ofRandom(angle-rotateSpeed, angle));
//            }
// how can i make every cat's angle go crazy different? the above line seems to result in every frame to have cats different in angle but within rotateSpeed. I would like each cat to have their own angle var that will be modified with one rotateSpeed
                ofRotateZ(angle);
                drawCat(0,0,catScale);
            ofPopMatrix();
        }
    }

    if(rotate) {
        if(counterclockwise) {
            angle -= rotateSpeed;
        } else {
            angle += rotateSpeed;
        }
    }
    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::circleResolutionChanged(int & circleResolution){
    ofSetCircleResolution(circleResolution);
}

//--------------------------------------------------------------
void ofApp::framerateChanged(int & framerate){
    ofSetFrameRate(framerate);
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
    ofSetColor(r*ofRandom((250-colorRandomness),250),g*ofRandom(250-colorRandomness,250),b*ofRandom(250-colorRandomness,250));
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
        ofSetColor(r*ofRandom(230-colorRandomness/2,230),g*ofRandom(230-colorRandomness/2,230),b*ofRandom(230-colorRandomness/2,230));
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
    ofSetColor(r*ofRandom(240-colorRandomness/2,240),g*ofRandom(240-colorRandomness/2,240),b*ofRandom(240-colorRandomness/2,240));
    ofCircle(catCenter.x, catCenter.y+0.3*catUnit, 0.35*catUnit);
    square(catCenter.x, catCenter.y-0.1*catUnit, 0.7*catUnit);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == 's') {
        gui.saveToFile("settings.xml");
    }
    if(key == 'l') {
        gui.loadFromFile("settings.xml");
    }
    
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
