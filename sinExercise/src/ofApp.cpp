#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofBackground(0,0,0);
    ofSetCircleResolution(32);
    ofSetFrameRate(30);
    
    bgm.loadSound("comtruise.aiff");
    bgm.setVolume(0.7);
    bgm.play();

    sinpoints.push_back(ofPoint(450,200));
    cospoints.push_back(ofPoint(300,450));
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    
    // screensaver
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofFill();
    int interval = 3;
    int divider = ofMap(ofGetMouseX(), 0, ofGetWidth(), 1, 128);
    for (int i = 0; i < 133; i++) {
        float sinOfTime			= sin( ofGetElapsedTimef() + i*PI/divider);
        float sinOfTimeMapped	= ofMap( sinOfTime, -1, 1, 0, 255);
        
        ofSetColor(sinOfTimeMapped,sinOfTimeMapped,sinOfTimeMapped,127);
        ofCircle(ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()/2-interval*i);
        
        ofSetColor(sinOfTimeMapped*cos(ofGetElapsedTimef()+1),sinOfTimeMapped*sin(ofGetElapsedTimef()),sinOfTimeMapped,127);
        ofRect(interval*i,interval*i,ofGetWidth()-2*interval*i, ofGetHeight()-2*interval*i);
        
    }

    // sin, cos gif
    ofSetLineWidth(2);
    ofNoFill();
    ofPoint center = ofPoint(ofGetWidth()/4, ofGetHeight()/4);
    int rad = 100;
    ofCircle(center, rad);
    float time = ofGetElapsedTimef();
    ofFill();
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRect(center.x+cos(time)*100, center.y-sin(time)*100, 10, 10);
    
    // sin
    ofLine(center.x+250, center.y-100, center.x+250, center.y+100);
    ofLine(center.x+250, center.y, ofGetWidth(), center.y);

    ofPoint temp;
    temp.x = center.x+250;
    temp.y = center.y-sin(time)*100;
    
    sinpoints.push_back(temp);
    if (sinpoints.size() > 400) sinpoints.erase(sinpoints.begin());
    
    ofNoFill();
    ofBeginShape();
        for (int i = 0; i < sinpoints.size(); i++){
            sinpoints[i].x = sinpoints[i].x + 1;
            ofVertex(sinpoints[i].x, sinpoints[i].y);
        }
    ofEndShape();

    // cos
    ofLine(center.x-100, center.y+250,center.x+100, center.y+250);
    ofLine(center.x, center.y+250, center.x, ofGetHeight());
    
    temp.x = center.x+cos(time)*100;
    temp.y = center.y+250;
    
    cospoints.push_back(temp);
    if (cospoints.size() > 400) cospoints.erase(cospoints.begin());
    
    ofNoFill();
    ofBeginShape();
    for (int i = 0; i < cospoints.size(); i++){
        cospoints[i].y = cospoints[i].y + 1;
        ofVertex(cospoints[i].x, cospoints[i].y);
    }
    ofEndShape();
    

    // lissajous shape
    float angle = ofGetElapsedTimef()*5;
    float x = ofGetWidth()-200;
    float y = ofGetHeight()-200;
    float radius = 150;
    
    float xratio = ofMap(ofGetMouseX(), 0, 1440, 0, 10);
    float yratio = ofMap(ofGetMouseY(), 0, 900, 0, 10);
    temp.x = x + radius*cos(time*xratio);
    temp.y = y + radius*sin(time*yratio);
    
 
    points.push_back(temp);
    if (points.size() > 300) points.erase(points.begin());
    
    ofNoFill();
    ofBeginShape();
    for (int i = 0; i < points.size(); i++){
        ofVertex(points[i].x, points[i].y);
    }
    ofEndShape();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    for (int i=0;i<10;i++) if (points.size() > 0) points.erase(points.begin());
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
