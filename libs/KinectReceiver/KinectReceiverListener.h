//
//  KinectReceiverListener.h
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//
//

#ifndef KinectReceiverListener_h
#define KinectReceiverListener_h

class KinectReceiverListener {
    
public:
    //new data for depth image avaiable with size and number byte per pixel
    virtual void newDepthData(char *data) = 0;
};

#endif /* KinectStreamListener_h */
