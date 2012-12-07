#include "testApp.h"

void testApp::setup(){
	
	ofSetVerticalSync(true);
	ofSetCircleResolution(2);
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
    
	int bufferSize = 256;
	
	
	left.assign(bufferSize, 0.0);
	right.assign(bufferSize, 0.0);
	
	smoothedVol     = 0.0;
	scaledVol		= 0.0;

	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);


    ofToggleFullscreen();
    
    vinyl1.setup();
    vinyl2.setup();
    vinyl3.setup();
}

//--------------------------------------------------------------
void testApp::update(){
#ifdef USE_SOUND_SOURCE
 ofSoundUpdate();
#endif
    
	//lets scale the vol up to a 0-1 range 
	scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(225);
	ofDrawBitmapString("PINKY CONTROL revision 02, updated 26 NOV 2012", 32, 32);
}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){	
	
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
		
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
//	if( key == 's' )
//		soundStream.start();
//
//	if( key == 'e' )
//		soundStream.stop();
}

void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y ){}
void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int w, int h){}
void testApp::gotMessage(ofMessage msg){}
void testApp::dragEvent(ofDragInfo dragInfo){}
