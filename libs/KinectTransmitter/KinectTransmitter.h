#pragma once

#include "ofxNetwork.h"
#include "ofMain.h"

using namespace std;

struct dataLengthStruct {
	char* data;
	unsigned int length;
};

class KinectTransmitter : public ofThread {

public:
	void setup(int port, int numBytePerPixel = 2, bool bSingleClient = true);		//port to open tcp/ip server, numberBytePerPixel used for the transmission
	void start();										//start thread and open tcp/server
	void stop();										//stop thread and close tcp/server
	void newData(ofShortPixels& imageToSend);			//update image to send using network

	float& getBitrate() { return bitrate; }				//return the current bitrate in Mbit/s
	int getPort() { return port; }						//return tcp/ip server port
	bool isSingleClient() { return bSingleClient; }		//true transmits only to one client, false to all clients connected

protected:
	virtual dataLengthStruct prepareData() = 0;			//override to prepare data
	void threadedFunction();
	bool send(bool sendRawBytesToAll = false);			//send function to send data throught tcp/ip

	bool bSingleClient;				//transmit only to one client

	int numBytePerPixel;			//number of byte for each pixels used for the transmission
	ofShortPixels imageToSend;		//depth image to sed

	std::mutex mtx;
	std::condition_variable cv;

private:
	ofxTCPServer tcpServer;

	int clientId;			//current clientID to 
	int port;				//port for tpc server
	bool running;
	
	//bitrate calculations
	long lastTime = 0;		//last time of calc bitrate
	float bitrate = 0;		//Mbit
	long bytePerSecond = 0;
};

