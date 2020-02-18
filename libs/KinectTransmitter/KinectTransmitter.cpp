#include "KinectTransmitter.h"
#include "ZlibUtils.h"

void KinectTransmitter::setup(int port, int numBytePerPixel, bool bSingleClient) {
	KinectTransmitter::port = port;
	KinectTransmitter::numBytePerPixel = numBytePerPixel;
	KinectTransmitter::bSingleClient = bSingleClient;

	KinectTransmitter::running = false;
}

void KinectTransmitter::start() {
	running = true;

	startThread();
}

void KinectTransmitter::stop() {
	tcpServer.close();
	stopThread();
}

void KinectTransmitter::threadedFunction() {
	//TODO: retrive current local ip
	cout << ofGetTimestampString() + " -KinectTransmitter: starting ip: TODO port: " << port << endl;

	tcpServer.setup(port, true);
	int i = 0;
	while (isThreadRunning()) {
		bitrate = 0;
		bytePerSecond = 0;
		clientId = -1;

		tcpServer.waitConnectedClient();

		// get the ip and port of the client
		string port = ofToString(tcpServer.getClientPort(i));
		string ip = tcpServer.getClientIP(i);
		cout << "CONNECTED: " << ofGetTimestampString() + " - KinectTransmitter: transmitting to " + ip + " on port: " + port << endl;
	
		do {
			// for each connected client lets get the data being sent and lets print it to the screen
			for (unsigned int i = 0; i < (unsigned int)tcpServer.getLastID(); i++) {
				if (tcpServer.isClientConnected(i)) {
					if (clientId < 0) clientId = i;

					//process once

					//send image
					if (imageToSend.isAllocated()) {
						send(bSingleClient);
					}
					
					break;
				}
				//clientId is not connected
				else if(i == clientId){
					clientId = -1;
				}
			}

			//wait new data
			std::unique_lock<std::mutex> lock(mtx);
			cv.wait(lock);

		} while (tcpServer.getNumClients() > 0);

		
	}
}

void KinectTransmitter::newData(ofShortPixels& imageToSend) {
	KinectTransmitter::imageToSend = imageToSend;
	cv.notify_one();
}

bool KinectTransmitter::send(bool singleClient) {
	bool result = false;

	//retrive data
	dataLengthStruct originalData = prepareData();

	if (originalData.length > 0) {
		dataLengthStruct sendData;
		sendData.length = originalData.length + 4;
		sendData.data = new char[sendData.length];

		//compress data and get size of compressed data, compress retrive the size of compressed data
		sendData.length = ZlibUtils::compress(originalData.data, originalData.length, &sendData.data[4], sendData.length);
		
		//first 4 byte as int to rapresent the message size
		sendData.data[0] = (sendData.length >> 24) & 0xFF;
		sendData.data[1] = (sendData.length >> 16) & 0xFF;
		sendData.data[2] = (sendData.length >> 8) & 0xFF;
		sendData.data[3] = (sendData.length >> 0) & 0xFF;

		if (singleClient) {
			result = tcpServer.sendRawBytes(clientId, sendData.data, sendData.length + 4);
		}
		else {
			result = tcpServer.sendRawBytesToAll(sendData.data, sendData.length + 4);
		}

		bytePerSecond += sendData.length + 4;

		if (ofGetElapsedTimeMillis() - lastTime > 1000) {
			double diffTime = (ofGetElapsedTimeMillis() - lastTime) / 1000.0;
			bitrate = (bytePerSecond / diffTime) * 0.0000076294;
			lastTime = ofGetElapsedTimeMillis();

			bytePerSecond = 0;
		}

		//free memory
		delete[] sendData.data;
	}
	
	return result;
}