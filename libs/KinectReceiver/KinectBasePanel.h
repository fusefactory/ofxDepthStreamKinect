//
//  KinectBasePanel.hpp
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//

#ifndef KinectBasePanel_h
#define KinectBasePanel_h

#include <stdio.h>
#include "ofxGui.h"
#include "KinectDevice.h"
#include "KinectSettings.h"

class KinectBasePanel : public ofxPanel{    
    public:
        void addGuiComponents(KinectDevice *kinectDevice);
        void addGuiComponents(KinectDevice *kinectDevice, KinectSettings kinectSettings);
    
        void save();
        string &getFilename(){return filename;}
    
    protected:
        KinectDevice *kinectDevice;
        ofxButton     connectReleaseButton;
        ofxIntSlider  minDistanceIntSlider;
        ofxIntSlider  maxDistanceIntSlider;
        ofxFloatSlider  leftMarginIntSlider;
        ofxFloatSlider  rightMarginIntSlider;
        ofxFloatSlider  topMarginIntSlider;
        ofxFloatSlider  bottomMarginIntSlider;
        ofxFloatSlider  vertCorrectionFloatSlider;
        ofxFloatSlider  keystoneFloatSlider;
        ofxButton     clearImageButton;
    
        ofxButton     saveButton;
};
#endif /* KinectBasePanel_hpp */
