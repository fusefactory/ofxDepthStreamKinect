//
//  KinectReceiver.cpp
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//
//

#include "KinectReceiver.h"
#include "../Zlib/ZlibUtils.h"
#include "KinectDevice.h"

KinectReceiver::KinectReceiver(std::string address, int port) {
	KinectReceiver::address = address;
	KinectReceiver::port = port;
    firstAttempConnection = true;
    
    uncompressedDump = new char[1024 * 1024 * 2];   //TODO: this is the current maximum size of transmission
    compressedDump = new char [1024 * 1024 * 2];    //TODO: this is the current maximum size of transmission
}

KinectReceiver::~KinectReceiver(){
    delete[] uncompressedDump;
}

void KinectReceiver::addListener(KinectReceiverListener * listener) {
    listeners.push_back(listener);
}

void KinectReceiver::start() {
    connectTime = 0;
    deltaTime = 0;
    startThread();
}

void KinectReceiver::stop() {
    stopThread();
    if (tcpClient.isConnected()) {
        tcpClient.close();
    }
    
    firstAttempConnection = true;
}

bool KinectReceiver::isRunning() {
    return isThreadRunning();
}

void KinectReceiver::threadedFunction() {
    while (isThreadRunning()) {
        
        if (!tcpClient.isConnected()) {
            
            // if we are not connected lets try and reconnect every 5 seconds
            deltaTime = ofGetElapsedTimeMillis() - connectTime;
            
            if (firstAttempConnection || deltaTime > reconnectionTime){
                firstAttempConnection = false;
                
                cout << ofGetTimestampString() + " - KinectStreamReceiver: trying connection to " + address + ":" + to_string(port) << endl;
                
                tcpClient.setup(address, port);
                connectTime = ofGetElapsedTimeMillis();
                
                if (tcpClient.isConnected()) {
                    cout << ofGetTimestampString() + " - KinectStreamReceiver connected to " + address + ":" + to_string(port) << endl;
                }
            }
        }
        
        if (tcpClient.isConnected()) {
            // header (data length)
            char *header = new char[4];
            readBytes(header, 4);
            int b0 = (int)(0x0ff & header[0]);
            int b1 = (int)(0x0ff & header[1]);
            int b2 = (int)(0x0ff & header[2]);
            int b3 = (int)(0x0ff & header[3]);
            unsigned int length = ((b0 << 24) | (b1 << 16) | (b2 << 8) | b3);

            if (length > 0) {
                // data
                readBytes(compressedDump, length);
                
                unsigned int uncompressedSize = 1024 * 1024 * 2;       //FIXME: setup right dimension
                
                //decompress data
                unsigned int uncompressedRealSize = ZlibUtils::decompress(uncompressedDump, uncompressedSize, compressedDump, length);
                
                for (KinectReceiverListener *listener : listeners) {
                    listener->newDepthData(uncompressedDump, uncompressedRealSize);
                }
            }
            
            delete[] header;
        }
    }
}

void KinectReceiver::readBytes(char *buffer, unsigned int length) {
    int bytesRead = 0;
    int result;
    while (tcpClient.isConnected() && bytesRead < length){
        result = tcpClient.receiveRawBytes(buffer + bytesRead, length - bytesRead);
        if (result >= 0) {
            bytesRead += result;
        }
    }
}
