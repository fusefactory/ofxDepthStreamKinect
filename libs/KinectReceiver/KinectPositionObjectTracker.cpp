//
//  KinectPositionTracker.cpp
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//

#ifdef ofxDepthStreamKinectReceiver

#include "KinectPositionObjectTracker.h"


KinectPositionObjectTracker::KinectPositionObjectTracker(KinectDevice *kinect) : KinectObjectTracker(kinect) {
    
}

void KinectPositionObjectTracker::update(int maxBlobs) {
    KinectObjectTracker::update(maxBlobs);
    calculatePositionTracked();
}

void KinectPositionObjectTracker::calculatePositionTracked() {
    positionsTracked.clear();
    
    for (ofxCvBlob &blob : contourFinder.blobs) {
        // average blob/hand depth
        float posDepth = 0;
        int validPixels = 0;
        for (int x = 0; x < blob.boundingRect.width; x++) {
            for (int y = 0; y < blob.boundingRect.height; y++) {
                int index = depthPixels.getPixelIndex(blob.boundingRect.x + x, blob.boundingRect.y + y);
                float depth = (255 - depthPixels[index]) / 255.0 * (kinect->getMaxDistance() - kinect->getMinDistance()) + kinect->getMinDistance();
                if (depth > 0) {
                    posDepth += depth;
                    validPixels++;
                }
            }
        }
        if (validPixels > 0) {
            posDepth /= (float)validPixels;
        }
        
        float posX = kinect->convertToRealWorldX(blob.centroid.x, posDepth);
        float posY = kinect->convertToRealWorldY(blob.centroid.y, posDepth); // not necessary here
        ofVec3f posTracked(posX, posY, posDepth);
        positionsTracked.push_back(posTracked);
    }
}

#endif