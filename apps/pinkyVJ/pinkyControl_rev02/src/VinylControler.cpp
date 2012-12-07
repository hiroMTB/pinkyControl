//
//  VinylContoroler.cpp
//  pinkyControl_rev01
//
//  Created by matobahiroshi on 12/12/03.
//
//

#include "VinylControler.h"

#define kMaxSampFramesInIOCycle 32768
#define kMaxNumValsInIOCycle kMaxSampFramesInIOCycle>>6


void VinylControler::setup(){
    setupPinky();

#ifdef USE_SOUND_SOURCE
    // sound
    s_player.loadSound("test.wav");
    s_player.setVolume(1);
    //s_player.play();
#endif

    // movie
    m_player.loadMovie("test.mov");
    m_player.play();

}


void VinylControler::setupPinky(){
    mVinylTracker= MPVT2_CreateNew(kMaxSampFramesInIOCycle,44100.0);
    MPVT2_SetSignalPowerThreshold(mVinylTracker, -37.0);
	MPVT2_SetCutoffSharpness(mVinylTracker, 2.0);
	MPVT2_SetAbsoluteMode(mVinylTracker, true);
	MPVT2_SetVinylGeneration(mVinylTracker, 5);
    
	mTempBufferLeft = new Float32[kMaxSampFramesInIOCycle];
	mTempBufferRight = new Float32[kMaxSampFramesInIOCycle];
	mVelocityVals = new Float64[kMaxNumValsInIOCycle];
	mPowerVals = new Float64[kMaxNumValsInIOCycle];
	mPositionVals = new Float64[kMaxNumValsInIOCycle];
}


void VinylControler::processSignal(Float32 * mTempBufferLeft, Float32 * mTempBufferRight){
    long num_measurements = 256;
    
    //Call the vinyl demod routine
    MPVT2_ProcessBuffer(mVinylTracker, mTempBufferLeft, mTempBufferRight, 256, mVelocityVals, mPowerVals, mPositionVals, num_measurements);
}

void VinylControler::queryData(){
    
    Float64 velocity  = MPVT2_Query_Velocity(mVinylTracker);
	Float64 power     = MPVT2_Query_SignalPower(mVinylTracker);
	Float64 position  = MPVT2_Query_AbsolutePosition(mVinylTracker);
	Float64 error     = MPVT2_Query_ErrorMetric(mVinylTracker);
    
#ifdef USE_SOUND_SOURCE
    s_player.setVolume(sqrt(power));
    s_player.setSpeed(velocity);
    //s_player.setPosition(<#float pct#>)
#endif
    
    m_player.setSpeed((*mVelocityVals));
    //m_player.setVolume(scaledVol*255.0);
}

void VinylControler::update(){
    m_player.idleMovie();
}


void VinylControler::drawDataString(){
    Float64 velocity  = MPVT2_Query_Velocity(mVinylTracker);
	Float64 power     = MPVT2_Query_SignalPower(mVinylTracker);
	Float64 position  = MPVT2_Query_AbsolutePosition(mVinylTracker);
	Float64 error     = MPVT2_Query_ErrorMetric(mVinylTracker);
    
	ofNoFill();
    
    int x = 500;
    int line = 18;
   	ofDrawBitmapString("velocity: "+ofToString(mVelocityVals[0]), 32, x);
   	ofDrawBitmapString("power: "+ofToString(mPowerVals[0]), 32, x+=line);
    ofDrawBitmapString("position; "+ofToString(mPositionVals[0]), 32, x+=line);
    ofDrawBitmapString("error: "+ofToString(mErrorVal), 32, x+=line);
    
    x+=line;
    //ofDrawBitmapString("smoothedVol: "+ofToString(smoothedVol), 32, x+=line);
    //ofDrawBitmapString("scaledVol: "+ofToString(scaledVol), 32, x+=line);
}

void VinylControler::drawInputSignal(){
    
//	// draw the left channel:
//	ofPushStyle();
//    ofPushMatrix();
//    ofTranslate(32, 70, 0);
//    
//    ofSetColor(225);
//    ofDrawBitmapString("INPUT 1 L", 4, 18);
//    
//    ofSetLineWidth(1);
//    ofRect(0, 0, 512, 200);
//    
//    ofSetColor(245, 58, 135);
//    ofSetLineWidth(3);
//    
//    ofBeginShape();
//    for (int i = 0; i < left.size(); i++){
//        ofVertex(i*2, 100 -left[i]*180.0f);
//    }
//    ofEndShape(false);
//    
//    ofPopMatrix();
//	ofPopStyle();
//    
//	// draw the right channel:
//	ofPushStyle();
//    ofPushMatrix();
//    ofTranslate(32, 270, 0);
//    
//    ofSetColor(225);
//    ofDrawBitmapString("INPUT 1 R", 4, 18);
//    
//    ofSetLineWidth(1);
//    ofRect(0, 0, 512, 200);
//    
//    ofSetColor(245, 58, 135);
//    ofSetLineWidth(3);
//    
//    ofBeginShape();
//    for (int i = 0; i < right.size(); i++){
//        ofVertex(i*2, 100 -right[i]*180.0f);
//    }
//    ofEndShape(false);
//    
//    ofPopMatrix();
//	ofPopStyle();
}

void VinylControler::drawVideo(){
	// draw the average volume:
	ofPushMatrix();
    ofTranslate(565, 70, 0);
    
    ofSetHexColor(0xFFFFFF);
    int c=255.0; //*scaledVol
    ofSetColor(c,c,c);
    m_player.draw(0,0);
    
    ofPopMatrix();
	
}