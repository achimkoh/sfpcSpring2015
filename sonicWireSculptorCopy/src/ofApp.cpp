#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    rotateX = 0;
    rotateY = 1;
    rotateZ = 0;
    tempY = 0;
    keyDown = FALSE; // used to store Y rotation
    
    rotAngle = 0.75;
    
    gridInterval = 20;
    
    counter = 0;
    
    metronome.loadSound("stick.wav");
    metronome.setVolume(0);
    
    // audio stream setup
    soundStream.listDevices();
    soundStream.setDeviceID(2);
    
    int bufferSize = 256;
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
    sampleRate = 44100;
    channels = 2;
    
    soundStream.setup(this, 0, channels, sampleRate, bufferSize, 4);
    
    // video recorder setup
    fileName = "sonicWire";
    fileExt = ".mov"; // ffmpeg uses the extension to determine the container type. run 'ffmpeg -formats' to see supported formats
    
    vidRecorder.setVideoCodec("mpeg4");
    vidRecorder.setVideoBitrate("800k");
    vidRecorder.setAudioCodec("mp3");
    vidRecorder.setAudioBitrate("192k");
    bRecording = false;
    ofEnableAlphaBlending();
    
    recStart = 0;
    
    // text input form
    monoLineTextInput.setup();
    monoLineTextInput.text = "";
    monoLineTextInput.bounds.x = 700;
    monoLineTextInput.bounds.y = 400;
    monoLineTextInput.bounds.height = 30;
    monoLineTextInput.bounds.width = 200;
    monoLineTextInput.cursorPosition = 0;
    waitingInput = false;
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    float curVol = 0.0;
    
    // samples are "interleaved"
    int numCounted = 0;
    
    //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2]*0.5;
        right[i]	= input[i*2+1]*0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted+=2;
    }
    
    //this is how we get the mean of rms :)
    curVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    curVol = sqrt( curVol );
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * curVol;
    
    bufferCounter++;
    
    if(bRecording) vidRecorder.addAudioSamples(input, bufferSize, nChannels);
    
}

//--------------------------------------------------------------
void ofApp::exit() {
    
    if (waitingInput) {
        ofLogToFile("myLogFile.txt", true);
        ofLog() << monoLineTextInput.text;
    }
    vidRecorder.close();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    counter += rotateY;
    
    for (int wireNum = 0; wireNum < wires.size(); wireNum++) {
        wires[wireNum].update(rotAngle, rotateX, rotateY, rotateZ, counter);
    }
    
    // update data for recording
    if(bRecording){
        ofImage current;
        current.grabScreen(400, 50, 800, 800);
        vidRecorder.addFrame(current.getPixelsRef());
    }
    
    // stop recording after 480 frames
    if (recStart < ofGetFrameNum() - 480 && recStart != 0){
        if (wires.back().rec) wires.back().stopRec();
        bRecording = false;
        vidRecorder.close();
        waitingInput = true;
        recStart = 0;
    }
    
    if (waitingInput && !monoLineTextInput.getIsEditing()) monoLineTextInput.beginEditing();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // background reacts to total volume
    ofBackground(0);
    float s = *ofSoundGetSpectrum(32);
    ofBackgroundGradient(ofColor(100, 100), ofColor(ofMap(s,0.1,1,0,200,true)));
    
    // draw grid
    ofSetColor(127,127,127);
    for (int i = 0; i < (ofGetHeight()/gridInterval) - 1; i++) {
        ofSetLineWidth(1);
        ofLine(0, (i+1)*gridInterval, ofGetWidth(), (i+1)*gridInterval);
    }
    
    // crosshair (serves as click)
    crosshair(4, 10, ofColor::black, ofColor::red);
    if (counter % 30 == 0) metronome.play();
    
    // cursor
    ofSetColor(255);
    ofCircle(ofGetMouseX(), ofGetMouseY(), 2);
    
    // draw the lines!
    ofFill();
    for (int wireNum = 0; wireNum < wires.size(); wireNum++) {
        wires[wireNum].draw();
    }
    
    // rec status
    if (bRecording) {
        ofSetColor(255,0,0);
        ofCircle(ofGetWidth()-50, 50, 20);
    }
    
    // after finishing recording, present input form
    if (waitingInput) {
        ofSetColor(255);
        ofRect(monoLineTextInput.bounds);
        ofNoFill();
        ofSetColor(0);
        monoLineTextInput.draw();
        ofDrawBitmapStringHighlight("record complete. type in your email address and press enter", ofPoint(535,390));
    }
}

//--------------------------------------------------------------
void ofApp::crosshair(int size, int time, ofColor color1, ofColor color2){
    
    int beatLength = (360 / rotAngle) / 16;
    int frameCount = ofGetFrameNum();
    
    if (((frameCount+3*beatLength) % (4*beatLength)) < time) size = size;
    if (((frameCount+2*beatLength) % (4*beatLength)) < time) size *= 1.5;
    if (((frameCount+beatLength) % (4*beatLength)) < time) size *= 2;
    if ((frameCount % (4*beatLength)) < time) {
        size *= 2.5;
        ofSetColor(color2);
        ofCircle(ofPoint(ofGetWidth()/2, ofGetHeight()/2), size/2);
    }
    
    if ((frameCount % (beatLength)) < time) {
        ofSetColor(color1);
        ofPoint center = ofPoint(ofGetWidth()/2, ofGetHeight()/2);
        ofLine(center.x-size, center.y, center.x+size, center.y);
        ofLine(center.x, center.y-size, center.x, center.y+size);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (!waitingInput) {
        // y rotation control
        if (key == OF_KEY_CONTROL) {
            rotateY *= -1;
            for (int wireNum = 0; wireNum < wires.size(); wireNum++) {
                wires[wireNum].reverse(rotateY);
            }
        }
        
        //    if (key == 's') { // drawing while stopped will cause awkward sound
        //        if (rotateY) {
        //            tempY = rotateY;
        //            rotateY = 0;
        //        } else rotateY = tempY;
        //    }
        
        // x rotation
        if (key == OF_KEY_UP || key == OF_KEY_DOWN) {
            if (!keyDown) {
                if (rotateY) tempY = rotateY;
                keyDown = TRUE;
            }
            rotateY = 0;
            if (key == OF_KEY_UP) rotateX = 1;
            else rotateX = -1;
        }
        
        // click
        if (key == OF_KEY_SHIFT) {
            if (metronome.getVolume() != 0) metronome.setVolume(0);
            else metronome.setVolume(0.3);
        }
        
        //    // change rotation speed .. doesn't really affect playback because playback is tied to frames and points
        //    if (key == '[') {
        //        rotAngle = rotAngle * 0.5;
        //    }
        //
        //    if (key == ']') {
        //        rotAngle = rotAngle * 2;
        //    }
        
    } else if (key == OF_KEY_RETURN) {
        ofLogToFile("myLogFile.txt", true);
        ofLog() << monoLineTextInput.text;
        monoLineTextInput.text = "";
        waitingInput = false;
        monoLineTextInput.cursorPosition = 0;
        string pythonCommand = "python "+ofToDataPath("send.py", true)+" &";
        system(pythonCommand.c_str());
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (!waitingInput) {
        // x rotation stop
        if (key == OF_KEY_UP || key == OF_KEY_DOWN) {
            rotateY = tempY;
            rotateX = 0;
            keyDown = FALSE;
        }
        
        if (key == ' ') {
            recStart = ofGetFrameNum();
            bRecording = TRUE;
            if(bRecording && !vidRecorder.isInitialized()) {
                vidRecorder.setup(fileName+ofGetTimestampString()+fileExt, 800, 800, 30, sampleRate, channels);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (!waitingInput) {
        if (button == 0) {
            sonicWire wire;
            wires.push_back(wire);
            wires.back().startRec(counter, rotAngle);
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (!waitingInput) {
        if (button == 0) {
            wires.back().stopRec();
            if (wires.back().noLine()) wires.pop_back();
        }
        
        // undo with right click
        if (button == 2) {
            if(wires.size() > 0) wires.pop_back();
        }
        
        // reset with center click
        if (button == 1) {
            wires.clear();
        }
    }
}