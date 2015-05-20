#ifndef __sonicWireSculptorLoop__sonicWire__
#define __sonicWireSculptorLoop__sonicWire__
#include "ofMain.h"

//--------------------------------------------------
class sonicWire {
    
public:
    
    void startRec(int counter, float rotAngle);
    void stopRec();
    ofPolyline line;
    bool rec;
    bool noLine();

    void update(float rotAngle, int rotateX, int rotateY, int rotateZ, int counter);
    int maxFrames;
    bool hasChangedDirection;
    void draw();

    // smoothing line
//    bool snap;
//    ofPoint snapPoint;
//    int previousY;

    // sound
    ofSoundPlayer wave;
    float pitch(int y);
    float pan(int x);
    int start, playhead, cur;
    bool playing;
    void play();
    void stop();
    void reverse(int dir);
        
};



#endif