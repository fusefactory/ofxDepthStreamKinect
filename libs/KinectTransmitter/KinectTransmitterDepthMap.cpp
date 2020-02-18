#include "KinectTransmitterDepthMap.h"

KinectTransmitterDepthMap::~KinectTransmitterDepthMap() {
	if(byteBuffer.data = NULL)	delete[] byteBuffer.data;
}

void KinectTransmitterDepthMap::setup(int port, int numBytePerPixel) {
	KinectTransmitter::setup(port, numBytePerPixel);

	const int maxSize = 1024 * 1024 * numBytePerPixel;	//hardcoded maximum transmission size
	byteBuffer.length = 0;
	byteBuffer.data = new char[maxSize];	//maximum resolution
}

dataLengthStruct KinectTransmitterDepthMap::prepareData() {
	byteBuffer.length = 0;
	
	if (imageToSend.size() > 0) {
		for (int i = 0; i < imageToSend.size(); i++) {
			
			//1 byte
			if (numBytePerPixel == 1) {
				float value = imageToSend[i];
				int div = 39;		//ipotizzo che il numero massimo in mm sia 10000 e lo divido per 256
				int r = (int)(value / div) & 255;

				byteBuffer.data[i] = r & 0xff;
			}
			//2 bytes
			else if (numBytePerPixel == 2){
				// sending only 2 most valuable bytes
				byteBuffer.data[i * 2] = (imageToSend[i] >> 8 ) & 0xff;
				byteBuffer.data[i * 2 + 1] = (imageToSend[i] >> 0) & 0xff;
			}
			else {
				cout << "KinectTransmitterDepthMap::prepareData - Error: value of numBytePerPixel not avaiable.";
			}
		}

		byteBuffer.length = imageToSend.getWidth() * imageToSend.getHeight() * numBytePerPixel;
	}

	return byteBuffer;
}