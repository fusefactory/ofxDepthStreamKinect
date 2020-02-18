#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	//ofSetLogLevel(OF_LOG_VERBOSE);

	ofSetWindowTitle("kinectAzureStreaming");

	//setup gui
	gui.setup("kinect", "config.xml"); // most of the time you don't need a name
	gui.setSize(300, gui.getHeight());
	gui.add(tcpPortSlider.setup("TCP PORT", 4444, 0, 65535));
	gui.add(singleClient.setup("SINGLE CLIENT", true));
	gui.add(depthModeParamString.set("DEPTH MODE (mouse over)", "K4A_DEPTH_MODE_NFOV_UNBINNED"));
	gui.add(leftMarginFloatSlider.setup("LEFT MARGIN", 0, 0, 0.5));
	gui.add(rightMarginFloatSlider.setup("RIGHT MARGIN", 0, 0, 0.5));
	gui.add(topMarginFloatSlider.setup("TOP MARGIN", 0, 0, 0.5));
	gui.add(bottomMarginFloatSlider.setup("BOTTOM MARGIN", 0, 0, 0.5));
	gui.loadFromFile("config.xml");

	//retrive kinect configuration
	k4a_depth_mode_t depthMode = K4A_DEPTH_MODE_NFOV_UNBINNED;
	k4a_fps_t fps = K4A_FRAMES_PER_SECOND_30;

	string depthModeString = depthModeParamString;
	if (depthModeString.compare("K4A_DEPTH_MODE_NFOV_2X2BINNED") == 0) {
		depthMode = K4A_DEPTH_MODE_NFOV_2X2BINNED;
	}
	else if (depthModeString.compare("K4A_DEPTH_MODE_NFOV_UNBINNED") == 0) {
		depthMode = K4A_DEPTH_MODE_NFOV_UNBINNED;
	}
	else if (depthModeString.compare("K4A_DEPTH_MODE_WFOV_2X2BINNED") == 0) {
		depthMode = K4A_DEPTH_MODE_WFOV_2X2BINNED;
	}
	else if (depthModeString.compare("K4A_DEPTH_MODE_WFOV_UNBINNED") == 0) {
		depthMode = K4A_DEPTH_MODE_WFOV_UNBINNED;
		fps = K4A_FRAMES_PER_SECOND_15; //no support for 30 fps
	}
	else if (depthModeString.compare("K4A_DEPTH_MODE_PASSIVE_IR") == 0) {
		depthMode = K4A_DEPTH_MODE_PASSIVE_IR;
	}

	ofAddListener(kinectDevice.onNewDepthData, this, &ofApp::onNewDepthData);

	//setup kinect
	auto kinectSettings = ofxAzureKinect::DeviceSettings();
	kinectSettings.synchronized = false;
	kinectSettings.updateWorld = false;
	kinectSettings.depthMode = depthMode;
	kinectSettings.colorResolution = K4A_COLOR_RESOLUTION_OFF;
	kinectSettings.cameraFps = fps;
	kinectSettings.updateColor = false;
	kinectSettings.updateIr = true;
	kinectSettings.updateVbo = false;
	kinectSettings.updateWorld = false;
	kinectSettings.synchronized = false;

	if (this->kinectDevice.open(kinectSettings)){
		this->kinectDevice.setUpdateTextures(false);
		this->kinectDevice.startCameras();
	}

	//setup transmitter
	kinectTransmitter.setup(tcpPortSlider, 2);
	kinectTransmitter.start();
}

//--------------------------------------------------------------
void ofApp::exit(){
	kinectTransmitter.stop();
	this->kinectDevice.close();
}

//--------------------------------------------------------------
void ofApp::update(){
	//apply crop
	if (this->kinectDevice.isStreaming() && newDepthData) {

		newDepthData = false;

		depthPixels = kinectDevice.getDepthPix();
		depthToDrawPixels = depthPixels;

		const float w = depthPixels.getWidth();
		const float h = depthPixels.getHeight();

		for (int y = 0; y < depthPixels.getHeight(); y++) {
			for (int x = 0; x < depthPixels.getWidth(); x++) {
				int index = depthPixels.getPixelIndex(x, y);
				 
				if (x > leftMarginFloatSlider* w && x < w - rightMarginFloatSlider * w &&
					y > topMarginFloatSlider* h && y < h - bottomMarginFloatSlider * h) {
					depthToDrawPixels[index] = depthToDrawPixels[index] * 9.0f;
				}
				else {
					depthPixels[index] = 0;
					depthToDrawPixels[index] = 0;
				}
			}
		}

		kinectTransmitter.newData(depthPixels);

		depthTexture.loadData(depthToDrawPixels);
	}

	//hardcore method to fix tcpPortSlider
	tcpPortSlider = kinectTransmitter.getPort();
	singleClient = kinectTransmitter.isSingleClient();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(128);

	if (this->kinectDevice.isStreaming()){
		if(depthTexture.isAllocated())
			depthTexture.draw(0, 0);
	}

	gui.draw();

	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate(), 2) + " FPS", 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
