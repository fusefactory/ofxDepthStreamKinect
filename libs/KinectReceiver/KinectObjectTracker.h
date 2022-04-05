//
//  KinectObjectTracker.h
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//
//

#ifdef ofxDepthStreamKinectReceiver

#ifndef KinectObjectTracker_h
#define KinectObjectTracker_h

#include <stdio.h>
#include <vector>
#include "ofMain.h"
#include "KinectDevice.h"
#include "ofxOpenCv.h"

class KinectObjectTracker : public ofThread {
    
public:
    KinectObjectTracker(KinectDevice *kinect);
    
    virtual void update(int maxBlobs);
    
    void setMinArea(int &area);
    void setMaxArea(int &area);
    void toggleBlobs(bool &value);
    
    int getMinArea();
    int getMaxArea();
    
    bool showingBlobs();
    void drawBlobs(float x, float y, float width, float height);
    vector<ofxCvBlob> getBlobs();

protected:
    KinectDevice *kinect;
    ofPixels depthPixels;
    ofxCvContourFinder contourFinder;
    ofFbo fbo;
	ofFbo alpha_mask;
    
private:
    int objectMinArea = 10, objectMaxArea = 10000;
    
    ofxCvColorImage colorImage;
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage alpha_image;
    
    bool showBlobs = true;
    
    std::vector<ofVec3f> objectsTracked;
};

#endif /* KinectObjectTracker_h */

#endif