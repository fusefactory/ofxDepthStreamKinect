#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	//ofSetLogLevel(OF_LOG_VERBOSE);

	ofSetWindowTitle("kinectAzureStreaming - 0.2.0");

	//setup gui
	gui.setup("kinect", "config.xml"); // most of the time you don't need a name
	gui.setSize(300, gui.getHeight());
	gui.add(tcpPortSlider.setup("TCP PORT", 4445, 0, 65535));
	gui.add(singleClient.setup("SINGLE CLIENT", true));
	gui.add(depthModeParamString.set("DEPTH MODE (mouse over)", "K4A_DEPTH_MODE_NFOV_UNBINNED"));
	gui.add(leftMarginFloatSlider.setup("LEFT MARGIN", 0, 0, 0.5));
	gui.add(rightMarginFloatSlider.setup("RIGHT MARGIN", 0, 0, 0.5));
	gui.add(topMarginFloatSlider.setup("TOP MARGIN", 0, 0, 0.5));
	gui.add(bottomMarginFloatSlider.setup("BOTTOM MARGIN", 0, 0, 0.5));
	gui.add(recordToggle.setup("RECORD", false));
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
	if(recordFolder.length() < 2 && recordToggle){
		recordFolder = ofToDataPath("/record-" + ofGetTimestampString("%C_%m_%d-%H_%M_%S") + "/");
	}
	if (recordFolder.length() > 2 && !recordToggle) {
		recordFolder = "";
	}

	/*
	- apply crop
	- modify image to draw
	- mirror raw image
	*/
	if (this->kinectDevice.isStreaming() && newDepthData) {

		newDepthData = false;

		rawDepthPixels = kinectDevice.getDepthPix();
		depthPixels = rawDepthPixels;
		depthToDrawPixels = rawDepthPixels;

		const float w = rawDepthPixels.getWidth();
		const float h = rawDepthPixels.getHeight();

		if (!depth1BytePixels.isAllocated()) {
			depth1BytePixels.allocate(rawDepthPixels.getWidth(), rawDepthPixels.getHeight(), OF_IMAGE_GRAYSCALE);
		}

		for (int y = 0; y < rawDepthPixels.getHeight(); y++) {
			for (int x = 0; x < rawDepthPixels.getWidth(); x++) {
				int index = rawDepthPixels.getPixelIndex(x, y);
					
				int mirrorX = rawDepthPixels.getWidth() - 1 - x;
				int indexMirrored = rawDepthPixels.getPixelIndex(mirrorX, y);

				if (mirrorX > leftMarginFloatSlider * w && mirrorX < w - rightMarginFloatSlider * w &&
					y > topMarginFloatSlider * h && y < h - bottomMarginFloatSlider * h) {
					depthToDrawPixels[indexMirrored] = rawDepthPixels[index] * 9.0f;
					depthPixels[indexMirrored] = rawDepthPixels[index];

					//if i'm recording
					if (recordToggle) {
						const float value = depthPixels[indexMirrored];
						const int div = 39;		//ipotizzo che il numero massimo in mm sia 10000 e lo divido per 256
						const int r = (int)(value / div) & 255;
						depth1BytePixels[indexMirrored] = 255 - (r & 0xff);
					}
				}
				else {
					depthPixels[indexMirrored] = 0;
					depthToDrawPixels[indexMirrored] = 0;
				}
			}
		}

		kinectTransmitter.newData(depthPixels);

		depthTexture.loadData(depthToDrawPixels);

		if (recordToggle && recordFolder.length() > 2) {
			imageToSave.setFromPixels(depth1BytePixels);
			imageToSave.save(recordFolder + ofToString(recordCount, 6, '0') + ".png");
			recordCount++;
		}
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
