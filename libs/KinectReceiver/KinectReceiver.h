//
//  KinectReceiver.h
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//
//

#ifndef KinectStreamReceiver_h
#define KinectStreamReceiver_h

#include <stdio.h>
#include <vector>
#include "ofMain.h"
#include "ofxTCPClient.h"
#include "KinectReceiverListener.h"

class KinectReceiver : public ofThread {
    
public:
	KinectReceiver(std::string address, int port);
    ~KinectReceiver();
    void start();
    void stop();
    bool isRunning();
    void addListener(KinectReceiverListener *listener);
    unsigned int getReconnectionTime() { return reconnectionTime; }
    void setReconnectionTime(unsigned int time) { reconnectionTime = time ;}
    
protected:
    //read bytes from the tcp client
    void readBytes(char *buffer, unsigned int length);
    
private:
    void threadedFunction();
    
    ofxTCPClient tcpClient;
    std::string address;                      //address of the remote kinect
    int port;                                  //port tcp used for the communication
    
    char *uncompressedDump, *compressedDump;   //dump memory to receive data and uncompress it
   
    int connectTime;
    int deltaTime;
    bool firstAttempConnection;
    unsigned int reconnectionTime = 5000;
    std::vector<KinectReceiverListener*> listeners;
};

#endif /* KinectStreamReceiver_h */
