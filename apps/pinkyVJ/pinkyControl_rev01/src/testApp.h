#ifndef _TEST_APP
#define _TEST_APP

//#define USE_SOUND_SOURCE 1


#include "ofMain.h"

class testApp : public ofBaseApp{
	
	public:

        static const int NUM_VINYL = 3;
        static const int bufferSize = 256;
        static float signalPerSecond;
    
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
				
		void audioIn(float * input, int bufferSize, int nChannels); 
	
		vector <float> left[NUM_VINYL];
		vector <float> right[NUM_VINYL];
		vector <float> volHistory[NUM_VINYL];
		
		int 	bufferCounter[NUM_VINYL];
		
		float smoothedVol[NUM_VINYL];
		float scaledVol[NUM_VINYL];
		
		ofSoundStream soundStream;
    
    
        // pinky test
        bool  bLiveMode;
        void*			mVinylTracker[NUM_VINYL];
        void setupPinky();

        Float32			* mTempBufferLeft[NUM_VINYL];
        Float32			* mTempBufferRight[NUM_VINYL];
        Float64			* mVelocityVals[NUM_VINYL];
        Float64			* mPowerVals[NUM_VINYL];
        Float64			* mPositionVals[NUM_VINYL];

#ifdef USE_SOUND_SOURCE
        ofSoundPlayer  s_player[NUM_VINYL];
#endif
    
        ofVideoPlayer m_player[NUM_VINYL];

    
    void drawSignal();
    void drawDataString();
    void drawVideo();
};

#endif	

