#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    KinectConfig kinectConfig;
    kinectConfig.serverAddress = kinectIp;
    kinectConfig.port = kinectPort;
    kinectConfig.minDistance = 500;
    kinectConfig.maxDistance = 5000;
    kinectConfig.leftMargin = 0;
    kinectConfig.rightMargin = 0;
    kinectConfig.topMargin = 0;
    kinectConfig.bottomMargin = 0;
    kinectConfig.vertCorrection = 1;
    kinectConfig.keystone = 0;
    
    kinect.setMinDistance(kinectConfig.minDistance);
    kinect.setMaxDistance(kinectConfig.maxDistance);
    kinect.setLeftMargin(kinectConfig.leftMargin);
    kinect.setRightMargin(kinectConfig.rightMargin);
    kinect.setTopMargin(kinectConfig.topMargin);
    kinect.setBottomMargin(kinectConfig.bottomMargin);
    kinect.setKeystone(kinectConfig.keystone);
    kinect.setVertCorrection(kinectConfig.vertCorrection);
    
    kinect.setAspect(0, 0);
    
    kinect.start();
    
    kinectGui.setup("KINECT", "kinect.xml", 10, 50);
    kinectGui.addGuiComponents(&kinect);
    
    kinectGui.loadFromFile(kinectGui.getFilename());
    
    bShowGui = true;
    
    //put in your data folder a recorded raw data of kinect
    kinect.loadKinectRecording("kinect_smaller.mov");
    
    //reming to add dragEvent
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    kinect.draw();
    
    if (bShowGui)
        kinectGui.draw();
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
    
    //TODO: it is possible add a listenerto add inside KinectBasePanel?
    ofRectangle kinectGuiRect = ofRectangle(kinectGui.getPosition().x,
                                            kinectGui.getPosition().y,
                                            kinectGui.getWidth(),
                                            kinectGui.getHeight());
    
    if(kinectGuiRect.inside(dragInfo.position))
        kinect.loadKinectRecording(dragInfo.files[0]);
}
