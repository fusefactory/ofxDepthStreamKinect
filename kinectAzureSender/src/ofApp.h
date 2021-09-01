#pragma once

#include "ofMain.h"

#include "ofxAzureKinect.h"
#include "KinectTransmitterDepthMap.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void exit();

		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void onNewDepthData() { newDepthData = true; }

private:
	ofxAzureKinect::Device kinectDevice;
	KinectTransmitterDepthMap kinectTransmitter;

	ofShortPixels depthPixels;			//depth image from kinect device
	ofShortPixels rawDepthPixels;		//image returned from addons
	ofShortPixels depthToDrawPixels;
	ofTexture depthTexture;				//texture to draw depth image

	//recording
	ofPixels depth1BytePixels;
	ofImage imageToSave;
	string recordFolder;
	int recordCount = 0;

	//gui
	ofxPanel gui;
	ofxIntSlider tcpPortSlider;
	ofParameter<string> depthModeParamString;
	ofxFloatSlider  leftMarginFloatSlider;
	ofxFloatSlider  rightMarginFloatSlider;
	ofxFloatSlider  topMarginFloatSlider;
	ofxFloatSlider  bottomMarginFloatSlider;
	ofxToggle singleClient;
	ofxToggle recordToggle;

	bool newDepthData = false;
};
