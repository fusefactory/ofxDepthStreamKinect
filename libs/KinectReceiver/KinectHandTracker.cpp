//
//  KinectHandTracker.cpp
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//
//

#ifdef ofxDepthStreamKinectReceiver


#include "KinectHandTracker.h"

KinectHandTracker::KinectHandTracker(KinectDevice *kinect) : KinectObjectTracker(kinect) {
}

void KinectHandTracker::update(int maxBlobs) {
    KinectObjectTracker::update(maxBlobs);
    calculateTrackedHands();
}

void KinectHandTracker::calculateTrackedHands() {
    hands.clear();
    
    for (ofxCvBlob &blob : contourFinder.blobs) {
//        int index = depthPixels.getPixelIndex(blob.centroid.x, blob.centroid.y);
//        float depth = (255 - depthPixels[index]) / 255.0 * (kinect->getMaxDistance() - kinect->getMinDistance()) + kinect->getMinDistance();
        
        // average blob/hand depth
        float handDepth = 0;
        int validPixels = 0;
        for (int x = 0; x < blob.boundingRect.width; x++) {
            for (int y = 0; y < blob.boundingRect.height; y++) {
                int index = depthPixels.getPixelIndex(blob.boundingRect.x + x, blob.boundingRect.y + y);
                float depth = (255 - depthPixels[index]) / 255.0 * (kinect->getMaxDistance() - kinect->getMinDistance()) + kinect->getMinDistance();
                if (depth > 0) {
                    handDepth += depth;
                    validPixels++;
                }
            }
        }
        if (validPixels > 0) {
            handDepth /= (float)validPixels;
        }

        float handX = kinect->convertToRealWorldX(blob.centroid.x, handDepth);
//        float handY = kinect->convertToRealWorldY(blob.centroid.y, depth); // not necessary here
        
        TrackedHand hand;
        hand.center.x = handX;
        hand.center.y = handDepth; // bottom kinect
        hand.width = blob.boundingRect.width;
        hand.height = blob.boundingRect.height;
        
        hands.push_back(hand);
    }
}

std::vector<TrackedHand> KinectHandTracker::getTrackedHands() {
    return hands;
}

#endif