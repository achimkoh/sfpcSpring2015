#include "ofApp.h"
#include "math.h"

//--------------------------------------------------------------
void ofApp::setup(){
    img.allocate(512, 512, OF_IMAGE_GRAYSCALE);
}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i = 0; i < img.getWidth(); i++) {
        for (int j = 0; j < img.getHeight(); j++) {
            
            // lantern
            ofPoint mousePos;
            ofPoint pixelPos;
            
            mousePos.set(mouseX, mouseY);
            pixelPos.set(i,j);
            float distance = (mousePos - pixelPos).length();
            int lightSize = 128;
            int pttrn = i^j; // tan(100*sin(i^j)+ 64);
            int lightSource = 255;
            if (distance < lightSize) {
                float pct = 1 - (distance / lightSize);
                img.getPixelsRef().setColor(i, j, ofColor(fmax(pttrn%256,(lightSource*pct)*pct+(pttrn%256)*(1-pct))));
            } else {
                img.getPixelsRef().setColor(i, j, ofColor(pttrn));
            }
        }
    }
    img.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    img.draw(0,0);
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
