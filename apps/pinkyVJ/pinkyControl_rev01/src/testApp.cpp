#include "testApp.h"

#include "VinylTrackFramework.h"

#define kMaxSampFramesInIOCycle 32768
#define kMaxNumValsInIOCycle kMaxSampFramesInIOCycle>>6



float testApp::signalPerSecond = 172.265625;    // CDJ: 172.265625, Vynil: 86.1328125


//--------------------------------------------------------------
void testApp::setup(){	 
	
	ofSetVerticalSync(true);
	ofSetCircleResolution(80);
	ofBackground(54, 54, 54);	
	
	// 0 output channels, 
	// 2 input channels
	// 44100 samples per second
	// 256 samples per buffer
	// 4 num buffers (latency)
	
	soundStream.listDevices();
	
	//if you want to set a different device id 
    //bear in mind the device id corresponds to all audio devices, including  input-only and output-only devices.
	//soundStream.setDeviceID(0);
	

    setupPinky();
    
    
    for(int i=0; i<NUM_VINYL; i++){
        left[i].assign(bufferSize, 0.0);
        right[i].assign(bufferSize, 0.0);
        volHistory[i].assign(400, 0.0);
        bufferCounter[i]	= 0;

        
        m_player[i].loadMovie("test.mov");
        m_player[i].play();
        
#ifdef USE_SOUND_SOURCE
        s_player[i].loadSound("test.wav");
        s_player[i].setVolume(1);
        s_player[i].play();
#endif

        smoothedVol[i]     = 0.0;
        scaledVol[i]		= 0.0;
    }

    bool ok = soundStream.setup(this, 0, 2, 44100, bufferSize, 4);

    
    //ofToggleFullscreen();
    bLiveMode = false;
}

//--------------------------------------------------------------
void testApp::update(){
#ifdef USE_SOUND_SOURCE
 ofSoundUpdate();
#endif
 
    for(int i=0; i<NUM_VINYL; i++){
        m_player[i].idleMovie();
    
        scaledVol[i] = ofMap(smoothedVol[i], 0.0, 0.17, 0.0, 1.0, true);
        volHistory[i].push_back( scaledVol[i] );
	
        if( volHistory[i].size() >= 400 ){
            volHistory[i].erase(volHistory[i].begin(), volHistory[i].begin()+1);
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(225);
	ofDrawBitmapString("PINKY CONTROL revision 01, updated 26 NOV 2012", 32, 32);
	ofDrawBitmapString("FPS: "+ofToString(ofGetFrameRate()), 32, 52);
	
    
	ofNoFill();
	
    for(int i=0; i<NUM_VINYL; i++){
        Float64 velocity = MPVT2_Query_Velocity(mVinylTracker[i]);
        Float64 power = MPVT2_Query_SignalPower(mVinylTracker[i]);
        Float64 position = MPVT2_Query_AbsolutePosition(mVinylTracker[i]);
        Float64 error = MPVT2_Query_ErrorMetric(mVinylTracker[i]);

        {
#ifdef USE_SOUND_SOURCE
        float pct = s_player[i].getPosition();

        s_player[i].setSpeed(velocity);
        s_player[i].setVolume(scaledVol[i]*255.0);
        s_player[i].setPosition(pct);
#endif
        }

        {
            float sec = position/signalPerSecond;
            float pct = sec / m_player[i].getDuration();
        
            m_player[i].setSpeed(velocity);
            m_player[i].setVolume(scaledVol[i]*255.0);
            m_player[i].setPosition(pct);
        }
        
    }
    
    if(ofGetWindowMode()!=OF_FULLSCREEN){
        drawSignal();
        drawDataString();
    }

    drawVideo();
    
}


void testApp::drawDataString(){
    for(int i=0; i<NUM_VINYL; i++){
//        Float64 velocity = mVelocityVals[i][0];
//        Float64 power = mPowerVals[i][0];
//        Float64 position = mPositionVals[i][0];
//         Float64 error = MPVT2_Query_ErrorMetric(mVinylTracker[i]); /** TODO **/

        Float64 velocity = MPVT2_Query_Velocity(mVinylTracker[i]);
        Float64 power = MPVT2_Query_SignalPower(mVinylTracker[i]);
        Float64 position = MPVT2_Query_AbsolutePosition(mVinylTracker[i]);
        Float64 error = MPVT2_Query_ErrorMetric(mVinylTracker[i]);
        
        int x = 32 + 300*i;
        int y = 560;
        
        int line = 18;
        ofDrawBitmapString("velocity: "+ofToString(velocity), x, y);
        ofDrawBitmapString("power: "+ofToString(power), x, y+=line);
        ofDrawBitmapString("position; "+ofToString(position), x, y+=line);
        ofDrawBitmapString("error: "+ofToString(error), x, y+=line);
        
        y+=line;
        ofDrawBitmapString("smoothedVol: "+ofToString(smoothedVol[i]), x, y+=line);
        ofDrawBitmapString("scaledVol: "+ofToString(scaledVol[i]), x, y+=line);
    }
}

void testApp::drawSignal(){

    int y = 230;
    float height = 144;
    
    for(int i=0; i<NUM_VINYL; i++){

        int color = 0xFF0000 >> (7*i);

        // LEFT
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(32+300*i, y, 0);

        ofSetColor(225);
        ofDrawBitmapString("INPUT L " + ofToString(i+1), 4, 18);

        ofSetLineWidth(1);
        ofRect(0, 0, bufferSize, height);

        ofSetHexColor(color);
        ofSetLineWidth(1);

        ofBeginShape();
        for (int j = 0; j < left[i].size(); j++){
            ofVertex(j, height*0.5 -left[i][j]*height*0.5);
        }
        ofEndShape(false);

        ofPopMatrix();
        ofPopStyle();

        // RIGHT
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(32+300*i, y+height+16, 0);

        ofSetColor(225);
        ofDrawBitmapString("INPUT R " + ofToString(i+1), 4, 18);

        ofSetLineWidth(1);
        ofRect(0, 0, bufferSize, height);

        ofSetHexColor(color);
        ofSetLineWidth(1);

        ofBeginShape();
        for (int j = 0; j < right[i].size(); j++){
            ofVertex(j, height*0.5 -right[i][j]*height*0.5);
        }
        ofEndShape(false);

        ofPopMatrix();
        ofPopStyle();
    }
}


void testApp::drawVideo(){

    if (ofGetWindowMode() == OF_FULLSCREEN) {
        for(int i=0; i<NUM_VINYL; i++){

            int index = NUM_VINYL-1-i;
            int color=scaledVol[index]*255.0;
            ofSetColor(color,color,color);
            m_player[index].draw(0, 0, ofGetWidth(), ofGetHeight());
        }
    }else{
        for(int i=0; i<NUM_VINYL; i++){
            ofPushStyle();
            ofPushMatrix();
            
            ofTranslate(32+300*i, 70, 0);
            int color=scaledVol[i]*255.0;
            ofSetColor(color,color,color);
            m_player[i].draw(0, 0, 256, 144);
            ofPopMatrix();
            ofPopStyle();

        }
    }

}

void testApp::audioIn(float * input, int bufferSize, int nChannels){
	
    int nStereoChannels = nChannels/2;
//    nStereoChannels = 3;

    for(int i=0; i<nStereoChannels; i++){
        float curVol = 0.0;
        int numCounted = 0;
        
        //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
        for (int j = 0; j < bufferSize; j++){
            
            int index = j*(i+1)*2;
            float tempL = left[i][j] = input[index]*0.5;
            float tempR = right[i][j]	= input[index+1]*0.5;
            
            mTempBufferLeft[i][j] = input[index];
            mTempBufferRight[i][j] = input[index+1];
            
            curVol += tempL * tempL;
            curVol += tempR * tempR;
            numCounted+=2;
        }
        
        //this is how we get the mean of rms :)
        curVol /= (float)numCounted;
        
        // this is how we get the root of rms :) 
        curVol = sqrt( curVol );
        
        smoothedVol[i] *= 0.93;
        smoothedVol[i] += 0.07 * curVol;
        
        bufferCounter[i]++;
        
        long num_measurements = 256;
        
        //Call the vinyl demod routine
        MPVT2_ProcessBuffer(mVinylTracker[i], mTempBufferLeft[i], mTempBufferRight[i],
                            256, mVelocityVals[i], mPowerVals[i], mPositionVals[i], num_measurements);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
//	if( key == 's' ){
//		soundStream.start();
//	}
//	
//	if( key == 'e' ){
//		soundStream.stop();
//	}
    
	if( key == 'P' ){
		bLiveMode = !bLiveMode;
        ofSetFullscreen(bLiveMode);
	}

    if( key == 'F' ){
        ofToggleFullscreen();
	}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::setupPinky(){
    for(int i=0; i<NUM_VINYL; i++){
        mVinylTracker[i]= MPVT2_CreateNew(kMaxSampFramesInIOCycle,44100.0);
        MPVT2_SetSignalPowerThreshold(mVinylTracker[i], -37.0);
        MPVT2_SetCutoffSharpness(mVinylTracker[i], 2.0);
        MPVT2_SetAbsoluteMode(mVinylTracker[i], true);
        MPVT2_SetVinylGeneration(mVinylTracker[i], 5);
        
        mTempBufferLeft[i] = new Float32[kMaxSampFramesInIOCycle];
        mTempBufferRight[i] = new Float32[kMaxSampFramesInIOCycle];
        mVelocityVals[i] = new Float64[kMaxNumValsInIOCycle];
        mPowerVals[i] = new Float64[kMaxNumValsInIOCycle];
        mPositionVals[i] = new Float64[kMaxNumValsInIOCycle];
    }
}

//void testApp::inputProc(){
//    mLastInputSampleCount = inInputTime->mSampleTime;
//	mInputBuffer->Store((const Byte *)inInputData->mBuffers[0].mData,
//                              This->mInputDevice.mBufferSizeFrames,
//                              UInt64(inInputTime->mSampleTime));
//    
//}





