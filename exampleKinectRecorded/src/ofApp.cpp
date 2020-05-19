#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    KinectConfig kinectConfig;
    kinectConfig.serverAddress = "127.0.0.1";
    kinectConfig.port = 4444;
    kinectConfig.minDistance = 500;
    kinectConfig.maxDistance = 5000;
    kinectConfig.leftMargin = 0;
    kinectConfig.rightMargin = 0;
    kinectConfig.topMargin = 0;
    kinectConfig.bottomMargin = 0;
    kinectConfig.vertCorrection = 1;
    kinectConfig.keystone = 0;

    kinect = new KinectRemote("kinect", kinectConfig.serverAddress, kinectConfig.port, 1, KinectAzureDepthWFOVUnbinned);
    kinect->setMinDistance(kinectConfig.minDistance);
    kinect->setMaxDistance(kinectConfig.maxDistance);
    kinect->setLeftMargin(kinectConfig.leftMargin);
    kinect->setRightMargin(kinectConfig.rightMargin);
    kinect->setTopMargin(kinectConfig.topMargin);
    kinect->setBottomMargin(kinectConfig.bottomMargin);
    kinect->setKeystone(kinectConfig.keystone);
    kinect->setVertCorrection(kinectConfig.vertCorrection);
    
    kinect->setAspect(0, 0);


    kinect->loadKinectRecording("director-2.mov");   //change here kinect movie to load
//    kinect->loadKinectRecording("TestKinect.m4v");   //change here kinect movie to load

    kinectGui.setup("KINECT", "kinect.xml", 10, 50);
    kinectGui.addGuiComponents(kinect);
    
    kinectGui.loadFromFile(kinectGui.getFilename());
    
    bShowGui = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect->update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    kinect->draw();
    
    if (bShowGui)
        kinectGui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'g'){
        bShowGui = ! bShowGui;
    }
    else if (key == 's'){
        kinectGui.save();
    }
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
