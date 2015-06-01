#include "sonicWire.h"

//--------------------------------------------------------

void sonicWire::update(float rotAngle, int rotateX, int rotateY, int rotateZ, int counter) {
    
    cur = counter % maxFrames;
    if (cur < 0) cur += maxFrames;
    
    // while recording, add current point to line and play it
    if (rec) {
        ofPoint mouse;
        mouse.set(ofGetMouseX(), ofGetMouseY());
        line.addVertex(mouse);
        waveSetup(wave, mouse);
        
        // while not recording, playback for the right duration
    } else {
        
        if (playing) {
            if (playhead > 0 && playhead < line.size()) play();
            else {
                stop();
                if (playhead == 0 || playhead == line.size()) playhead = line.size() - playhead;
            }
        } else {
            if (cur == start) play();
            else stop();
        }
    }
    
    // rotate!
    for (int i = 0; i < line.size(); i++){
        ofPoint fromCenter = line[i] - ofPoint(ofGetWidth()/2, ofGetHeight()/2);
        ofMatrix4x4 rotateMatrix;
        rotateMatrix.makeRotationMatrix(rotAngle, rotateX, rotateY, rotateZ);
        line[i] = fromCenter * rotateMatrix + ofPoint(ofGetWidth()/2, ofGetHeight()/2);
    }
    
}

//--------------------------------------------------------

void sonicWire::play() {
    
    playing = TRUE;
    
    waveSetup(wave, line[playhead]);
    
    if (hasChangedDirection) playhead--;
    else playhead++;
    
}

//--------------------------------------------------------

void sonicWire::stop() {
    
    playing = FALSE;
    wave.setVolume(0.9*wave.getVolume());
    
    if (hasChangedDirection) playhead = line.size() - 1;
    else playhead = 0;
    
    pitchCache = 0;
}

//--------------------------------------------------------

void sonicWire::reverse(int dir) {
    
    hasChangedDirection = !hasChangedDirection;
    start = (start - line.size() * dir) % maxFrames;
    
}

//--------------------------------------------------------

void sonicWire::draw() {
    
    for (int i = 0; i < line.size() - 1; i++){
        ofSetColor(ofMap(line[i].z, -400, 200, 127, 255, TRUE));
        ofCircle(line[i], ofMap(line[i].z, -800, 800, 1, 5));
    }
    if (rec) {
        ofSetColor(255,0,0);
        ofCircle(ofGetMouseX(), ofGetMouseY(), 5);
    } else if (playing) {
        ofSetColor(0,255,0);
        ofCircle(line[playhead], 5);
    }
    
}

//--------------------------------------------------------

void sonicWire::startRec(int counter, float rotAngle) {
    
    maxFrames = 360 / rotAngle;
    
    wave.loadSound("440hz.aiff");
    wave.setLoop(TRUE);
    
    waveSetup(wave, ofPoint(ofGetMouseX(), ofGetMouseY()));
    wave.play();
    
    start = counter % maxFrames;
    if (start < 0) start += maxFrames;
    
    rec = TRUE;
    playing = FALSE;
    hasChangedDirection = FALSE;
    
    playhead = 0;
    
    pitchCache = 0;
    
}

//--------------------------------------------------------

void sonicWire::stopRec() {
    
    rec = FALSE;
    stop();
    
}

//--------------------------------------------------------

float sonicWire::pitch(int y){
    
    // chromatic scale according to grid
    int p = ofMap(y, ofGetHeight(), 0, 0, 80, TRUE);
    float q = p - 40;
    float tone = powf(2,q/12);
    
    // glide
    if (pitchCache != 0) tone = (0.1 * tone) + (0.9 * pitchCache);
    pitchCache = tone;
    
    return tone;
}

//--------------------------------------------------------

float sonicWire::pan(int x){
    
    // map current point's relative x position from center with left/right pan
    float panReturn = ofMap(ofGetWidth()/2 - x, ofGetWidth()/2, -1*ofGetWidth()/2, -0.99, 0.99, TRUE);
    
    // multiply the pan value by its absolute value to make change more dramatic
    panReturn = panReturn * abs(panReturn);
    
    return panReturn;
    
}

//--------------------------------------------------------

float sonicWire::volume(int x, int y){
    
    // the further the current x position is from the center, the louder it gets
    float volumeReturn = ofMap(abs(ofGetWidth()/2 - x), 0, ofGetWidth()/2, 0.1, 1);
    
    // soften higher notes and boost lower notes
    float yFactor = ofMap(y, 0, ofGetHeight(), 0.3, 1.3, TRUE);
    if (yFactor > 1) yFactor += 0.5 * yFactor;
    volumeReturn *= yFactor * yFactor;
    volumeReturn += 0.3;
    return volumeReturn;
    
}

//--------------------------------------------------------

void sonicWire::waveSetup(ofSoundPlayer player, ofPoint point){
    
    wave.setPan(pan(point.x));
    wave.setSpeed(pitch(point.y));
    wave.setVolume(volume(point.x, point.y));
    
}

//--------------------------------------------------------

bool sonicWire::noLine(){
    
    return line.size() <= 1;
    
}