//
//  VinylControler.h
//  pinkyControl_rev01
//
//  Created by matobahiroshi on 12/12/03.
//
//

#pragma once

#define USE_SOUND_SOURCE 1


#include <iostream>

#include "ofMain.h"
#include "VinylTrackFramework.h"


class VinylControler{

public:
    VinylControler(){ setup(); };
    ~VinylControler(){};
    
    void setup();
    void queryData();
    void processSignal(Float32 * mTempBufferLeft, Float32 * mTempBufferRight);

    void update();
    
    void draw();
    
    
private:
    
    // pinky obj
    void * mVinylTracker;
    Float32	* mTempBufferLeft;
	Float32	* mTempBufferRight;
	Float64	* mVelocityVals;
	Float64	* mPowerVals;
	Float64	* mPositionVals;
    Float64   mErrorVal;

    // source vid/snd
    ofVideoPlayer m_player;

#ifdef USE_SOUND_SOURCE
    ofSoundPlayer s_player;
#endif
    
    void setupPinky();
    
    void drawDataString();
    void drawInputSignal();
    void drawVideo();

};




