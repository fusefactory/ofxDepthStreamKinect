//
//  KinectRemote.h
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//
//

#ifndef KinectRemote_h
#define KinectRemote_h

#include <stdio.h>
#include <string>
#include "KinectDevice.h"
#include "KinectReceiverListener.h"
#include "KinectReceiver.h"

class KinectRemote : public KinectDevice, KinectReceiverListener {
    
public:
    KinectRemote(std::string name, std::string address, int port, unsigned int bytesPerPixel = 2,  KinectDepthEnum kinectDepthEnum = KinectV2Depth);
    ~KinectRemote();
    
    void newDepthData(char *data) override;
    void start() override;
    void stop() override;
    bool isRunning() override;
    uint64_t getLastDataTimeMillis();

protected:
    float *updateEdgeData() override;
    ofVec3f &updateCom() override;
    int xKeystone(const float pX, const float pY, float keystone);
    void clearEdgeData() override;
    uint64_t lastDataTimeMillis = 0;

private:
	KinectReceiver* receiver;
    float *edgeData;
    ofVec3f com;

};
#endif /* KinectRemote_h */
