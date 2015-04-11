#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

//    ofEnableAlphaBlending();
//    ofSetBackgroundAuto(FALSE);
    ofBackground(255);
    
    myGradient.loadImage("circle_gradient.png");
    myImage.loadImage("jake.png");
    
    myGradient.setImageType(OF_IMAGE_COLOR_ALPHA);
    myImage.setImageType(OF_IMAGE_COLOR_ALPHA);
    
    myGradient.resize(500, 500);
    myImage.resize(500, 500);
    
    for (int x = 0; x < 500; x++) {
        for (int y = 0; y < 500; y++) {
            ofColor cur = myGradient.getColor(x, y);
            float alpha = cur.a;
            ofColor cur2 = myImage.getColor(x, y);
            myImage.setColor(x, y, ofColor(cur2, alpha));
        }
    }

    myImage.update();
    origin.x = ofGetWidth()/2;
    origin.y = ofGetHeight()/2;
    angle = 10;
}

//--------------------------------------------------------------
void ofApp::update(){


}

//--------------------------------------------------------------
void ofApp::draw(){

    ofPushMatrix();         // push the current coordinate position
        ofTranslate(origin);
        ofRotateZ(angle);
    
        ofSetRectMode(OF_RECTMODE_CENTER);
        myImage.draw(0,0);
        angle += 5;
    ofPopMatrix();          // recall the pushed coordinate position

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

    ofColor c = myImage.getColor(x,y);
    ofColor d = myGradient.getColor(x,y);
    
    float a1 = c.a;
    float a2 = d.a;
    float r1 = c.r;
    float r2 = d.r;
    float g1 = c.g;
    float g2 = d.g;
    float b1 = c.b;
    float b2 = d.b;
    
    cout << "myImage : " << r1 << "," << g1 << "," << b1 << "," << a1 << endl;
    cout << "myGradient : " << r2 << "," << g2 << "," << b2 << "," << a2 << endl;
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
