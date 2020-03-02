//
//  KinectDevice.h
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//
//

#ifndef KinectDevice_h
#define KinectDevice_h

#include <stdio.h>
#include "ofMain.h"
#include "KinectDepthEnum.h"

class KinectDevice {
    
public:
    KinectDevice(std::string name, unsigned int bytesPerPixel = 2, KinectDepthEnum kinectDepthEnum = KinectV2Depth);
    virtual void start() { };
    virtual void stop() { };
	virtual bool isRunning() { return false; };
    void update();
    
    ofVec2f getResolution() {return resolution; }
    std::string getName()  { return name ;}
    
    void setAspect(float x, float y);
    void setAspect(float x, float y, float width, float height);
    
    void connectRelease();
    void setMinDistance(int &distance);
    int getMinDistance();
    void setMaxDistance(int &distance);
    int getMaxDistance();
    void setLeftMargin(float &margin);
    float getLeftMargin();
    void setRightMargin(float &margin);
    float getRightMargin();
    void setTopMargin(float &margin);
    float getTopMargin();
    void setBottomMargin(float &margin);
    float getBottomMargin();
    void setKeystone(float &_keystone);
    float getKeystone();
    void setVertCorrection(float &_vertCorrection);
    float getVertCorrection();
    void clearImage();
    void loadKinectRecording(string _filename);
    
    ofTexture &getTexture();
    ofVec3f &getCom();
    void draw();
    void drawSelected();
    
    float convertToRealWorldX(float x, float depth);
    float convertToRealWorldY(float y, float depth);
    
    //recorder
    bool readKinectRecording = false;
    string kinectRecordingFilename = "";
    ofVideoPlayer kinectRecording;
    void setReadKinectRecording(bool _readKinectRecording){readKinectRecording = _readKinectRecording;};
    bool getReadKinectRecording(){return readKinectRecording;};
    ofFbo kinectRecordingFbo;
    ofShader depthShader;
    ofMatrix4x4 projectionFlat;
    ofMatrix4x4 modelviewFlat;
    
protected:
    virtual void setupDevice();
    virtual unsigned short* updateEdgeData() { return NULL; };
    virtual void clearEdgeData() = 0;
    virtual ofVec3f& updateCom() { return com; };
    
    KinectDepthEnum kinectDepthEnum;
    ofVec2f resolution;
    ofVec2f fieldOfViewDegree, fieldOfViewRad;
    int minDistance, maxDistance;
    float leftMargin, rightMargin, topMargin, bottomMargin;
    float keystone, vertCorrection;
	
    bool bClearImage = false;
    unsigned int bytesPerPixel;
    
    std::string name, fullName;
    ofTexture texture;
    float *lastData;
    ofVec3f com;
    int movement = 0;
    float x, y, width, height;
    float XtoZ, YtoZ;
};

#endif /* KinectDevice_h */
