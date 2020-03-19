#pragma once

#include "ofMain.h"
#include "ofxDepthStreamKinectReceiver.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
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
    
private:
    const int kinectPort = 4445;
    const string kinectIp = "192.168.1.74";
    KinectRemote kinect = KinectRemote("kinect", kinectIp, kinectPort, 2, KinectV2Depth);
    KinectBasePanel kinectGui;
    
    bool bShowGui;
};
