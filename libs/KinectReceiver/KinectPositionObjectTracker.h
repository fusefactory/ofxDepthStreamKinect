//
//  KinectPositionTracker.h
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//


#ifndef KinectPositionTracker_h
#define KinectPositionTracker_h

#include <stdio.h>
#include "KinectObjectTracker.h"

class KinectPositionObjectTracker : public KinectObjectTracker {
    
public:
    KinectPositionObjectTracker(KinectDevice *kinect);
    void update(int maxBlobs) override;
    std::vector<ofVec3f> &getPositionsTracked() {return positionsTracked;}
    
private:
    std::vector<ofVec3f> positionsTracked;
    
    void calculatePositionTracked();
};

#endif /* KinectPositionTracker_h */

