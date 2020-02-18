//
//  KinectBlobPanel.h
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//

#ifndef KinectBlobPanel_h
#define KinectBlobPanel_h

#include <stdio.h>
#include "KinectBasePanel.h"
#include "KinectHandTracker.h"
#include "KinectPositionObjectTracker.h"

class KinectBlobPanel : public KinectBasePanel{
    public:
        void addGuiComponents(KinectDevice *kinectDevice, KinectObjectTracker *kinectObjectTracker);

        ofxToggle       blobToggle;
        ofxIntSlider    minAreaIntSlider;
        ofxIntSlider    maxAreaIntSlider;
    
    protected:
        KinectObjectTracker *kinectObjectTracker;
};
#endif /* KinectBlobPanel_hpp */
