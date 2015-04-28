#ifndef __sonicWireSculptorLoop__sonicWire__
#define __sonicWireSculptorLoop__sonicWire__
#include "ofMain.h"

//--------------------------------------------------
class sonicWire {
    
public:
    
    void setup(float rotAngle);
    void startRec(int counter);
    void stopRec(int counter);
    ofPolyline line;
    bool rec;
    bool noLine();

    void update(float rotAngle, int rotateX, int rotateY, int rotateZ, int counter);
    int maxFrames;
    void draw();
    
    // smoothing line
//    bool snap;
//    ofPoint snapPoint;
//    int previousY;

    // sound
    ofSoundPlayer wave;
    float pitch(int y);
    float pan(int x);
    int start, end, cur, playhead;
    bool playing;
    void play();
    void stop();
    
};

#endif