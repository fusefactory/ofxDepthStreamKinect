//
//  KinectConfig.h
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//
//

#ifndef KinectConfig_h
#define KinectConfig_h

#include <stdio.h>
#include <string>
#include "KinectDepthEnum.h"

class KinectConfig {
    
public:
    KinectDepthEnum kinectDepthEnum = KinectV2Depth;    //type of depth kinect

    std::string serverAddress;          //address (ip) of the machine where running kinect (sender)
    int port;                           //port used to connect to the kinect sender
        
    int minDistance = 1, maxDistance = 10000;
    float leftMargin = 0, rightMargin = 0, topMargin = 0, bottomMargin = 0;
    ofVec2f position;                   //TODO: unused?
    ofVec2f scale;                      //TODO: unused?
    float vertCorrection = 1;
    float keystone = 0;
};

#endif /* KinectConfig_h */
