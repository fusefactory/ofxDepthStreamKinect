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

	void setBoundingBox(float Yt1, float Yt2, float Xt, float Yb1, float Yb2, float Xb);
    
    int getMinArea();
    int getMaxArea();
    
    bool showingBlobs();
    void drawBlobs(float x, float y, float width, float height);
    vector<ofxCvBlob> getBlobs();
	ofTexture getTexture();

protected:
    KinectDevice *kinect;
    ofPixels depthPixels;
    ofPixels boundingPixels;
    ofxCvContourFinder contourFinder;
    ofFbo fbo;
	ofFbo alpha_mask;
	//ofFbo boundingBox_image;
    
private:
    int objectMinArea = 10, objectMaxArea = 10000;
    
    ofxCvColorImage colorImage;
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage alpha_image;
    ofxCvGrayscaleImage boundingBox_image;
    
    bool showBlobs = true;
    
    std::vector<ofVec3f> objectsTracked;

	float Box_Yt1;
	float Box_Yt2;
	float Box_Xt;
	float Box_Yb1;
	float Box_Yb2;
	float Box_Xb;
};

#endif /* KinectObjectTracker_h */

#endif