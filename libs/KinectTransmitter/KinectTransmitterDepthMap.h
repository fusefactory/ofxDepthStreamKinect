#pragma once

#include "KinectTransmitter.h"

class KinectTransmitterDepthMap : public KinectTransmitter {

public:
	~KinectTransmitterDepthMap();
	void setup(int port, int numBytePerPixel = 2);

protected:
	dataLengthStruct prepareData() override;

private:
	dataLengthStruct byteBuffer;
};

