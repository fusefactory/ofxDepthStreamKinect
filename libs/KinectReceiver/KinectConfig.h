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

class KinectConfig {
    
public:
    std::string serverAddress;
    int port;
    int minDistance, maxDistance;
    float leftMargin, rightMargin, topMargin, bottomMargin;
    ofVec2f position;
    ofVec2f scale;
    float vertCorrection;
    float keystone;
};

#endif /* KinectConfig_h */
