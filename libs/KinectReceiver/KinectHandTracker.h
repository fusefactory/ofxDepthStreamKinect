//
//  KinectHandTracker.h
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//
//

#ifdef ofxDepthStreamKinectReceiver

#ifndef KinectHandTracker_h
#define KinectHandTracker_h

#include <stdio.h>
#include "KinectObjectTracker.h"
#include "TrackedHand.h"

class KinectHandTracker : public KinectObjectTracker {
    
public:
    KinectHandTracker(KinectDevice *kinect);
    std::vector<TrackedHand> getTrackedHands();
    void update(int maxBlobs) override;
	ofVec4f getPointsInfo(int cx, int cy);

private:
    std::vector<TrackedHand> hands;
    void calculateTrackedHands();
};
#endif /* KinectHandTracker_h */

#endif
