//
//  KinectDevice.cpp
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//
//

#include "KinectDevice.h"
#include <algorithm>

KinectDevice::KinectDevice(std::string name, unsigned int bytesPerPixel, KinectDepthEnum kinectDepthEnum) {
    KinectDevice::name = name;
    KinectDevice::bytesPerPixel = bytesPerPixel;
    KinectDevice::kinectDepthEnum = kinectDepthEnum;
    
    setupDevice();
    
    std::transform(fullName.begin(), fullName.end(), fullName.begin(), ::toupper);
        
    XtoZ = (float)tan(fieldOfViewRad.x / 2) * 2;
    YtoZ = (float)tan(fieldOfViewRad.y / 2) * 2;
    
    const int numOfPixels = resolution.x * resolution.y;
    lastData = new float[numOfPixels];
    texture.allocate(resolution.x, resolution.y, GL_RGB);
    texture.setTextureMinMagFilter(GL_NEAREST, GL_NEAREST); // for collison map
    kinectRecordingFbo.allocate(resolution.x, resolution.y, GL_RGB);
    
    // init black texture
    float *data = new float[numOfPixels * 3];
    for (int i = 0; i < resolution.x * resolution.y * 3; i++) {
        data[i] = 0;
    }
    texture.loadData(data, resolution.x, resolution.y, GL_RGB);
    delete[] data;
    
    depthShader.load("shaders/depth.vert","shaders/depth.frag");
    
    float viewW = resolution.x;
    float viewH = resolution.y;
    
    float fov = 60.0;
    float eyeX = viewW / 2.0;
    float eyeY = viewH / 2.0;
    float halfFov = PI * fov / 360;
    float theTan = tanf(halfFov);
    float cameraDist = eyeY / theTan;
    float aspect = (float) viewW /  (float)viewH;
    float nearDist = cameraDist / 10.0f;
    float farDist = cameraDist * 10.0f;
    projectionFlat.makeIdentityMatrix();
    projectionFlat.makePerspectiveMatrix(fov, aspect, nearDist, farDist);
    modelviewFlat.makeIdentityMatrix();
    modelviewFlat.makeLookAtViewMatrix(ofVec3f(eyeX, eyeY, cameraDist), ofVec3f(eyeX, eyeY, 0), ofVec3f(0.0, 1.0, 0.0));
}

void KinectDevice::setupDevice(){
    switch(kinectDepthEnum){
        case KinectV2Depth:
            fullName = name + " KINECTV2";
            resolution.set(512, 424);
            fieldOfViewDegree.set(70.6, 60);
            
            break;
        case KinectAzureDepthNFOVUnbinned:
            fullName = name + " AZURE-Near-Unbinned";

            resolution.set(640, 576);
            fieldOfViewDegree.set(75, 65);
            break;
            
        case KinectAzureDepthNFOVBinned:
            fullName = name + " AZURE-Near-Binned";
            
            resolution.set(320, 288);
            fieldOfViewDegree.set(75, 65);
            break;
            
        case KinectAzureDepthWFOVUnbinned:
            fullName = name + " AZURE-Wide-Unbinned";

            resolution.set(1024, 1024);
            fieldOfViewDegree.set(120, 120);
            break;
            
        case KinectAzureDepthWFOVBinned:
            fullName = name + " AZURE-Wide-Binned";
            
            resolution.set(512, 512);
            fieldOfViewDegree.set(120, 120);
            break;
    }
    
    fieldOfViewRad.set(ofDegToRad(fieldOfViewDegree.x), ofDegToRad(fieldOfViewDegree.y));
}

void KinectDevice::connectRelease(){
    if (isRunning()) {
        stop();
    } else {
        start();
    }
}

void KinectDevice::setMinDistance(int &distance) {
    minDistance = distance;
}

int KinectDevice::getMinDistance() {
    return minDistance;
}

void KinectDevice::setMaxDistance(int &distance) {
    maxDistance = distance;
}

int KinectDevice::getMaxDistance() {
    return maxDistance;
}

void KinectDevice::setLeftMargin(float &margin) {
    leftMargin = margin;
}

float KinectDevice::getLeftMargin() {
    return leftMargin;
}

void KinectDevice::setRightMargin(float &margin) {
    rightMargin = margin;
}

float KinectDevice::getRightMargin() {
    return rightMargin;
}

void KinectDevice::setTopMargin(float &margin) {
    topMargin = margin;
}

float KinectDevice::getTopMargin() {
    return topMargin;
}

void KinectDevice::setBottomMargin(float &margin) {
    bottomMargin = margin;
}

float KinectDevice::getBottomMargin() {
    return bottomMargin;
}

void KinectDevice::setKeystone(float &_keystone) {
    keystone = _keystone;
}

float KinectDevice::getKeystone() {
    return keystone;
}

void KinectDevice::setVertCorrection(float &_vertCorrection) {
    vertCorrection = _vertCorrection;
}

float KinectDevice::getVertCorrection() {
    return vertCorrection;
}

void KinectDevice::clearImage(){
    bClearImage = true;
}


void KinectDevice::loadKinectRecording(string _filename){
    kinectRecordingFilename = _filename;
    if(kinectRecording.load(kinectRecordingFilename)){
        kinectRecording.setLoopState(OF_LOOP_NORMAL);
        kinectRecording.play();
        readKinectRecording = true;
    }
}

void KinectDevice::update() {
    if(bClearImage){
        bClearImage = false;
        clearEdgeData();
        cout << "---- CLEARED EDGE DATA ---" << endl;
    }
    
    if(!readKinectRecording){
        float *data = updateEdgeData();
        texture.loadData(data, resolution.x, resolution.y, GL_RGB);
        com = updateCom();
    }else{
        kinectRecording.update();
        
        glDisable(GL_BLEND);
        kinectRecordingFbo.begin();
        depthShader.begin();
        
        depthShader.setUniform1f("kinectWidth", resolution.x);
        depthShader.setUniform1f("kinectHeight", resolution.y);
        depthShader.setUniform1f("kinectHalfWidth", resolution.x / 2.0f);
        depthShader.setUniform1f("kinectHalfHeight", resolution.y / 2.0f);
        
        depthShader.setUniformMatrix4f("modelview", modelviewFlat);
        depthShader.setUniformMatrix4f("projection", projectionFlat);

        if(kinectRecording.getTexture().isAllocated())
            depthShader.setUniformTexture("tex0", kinectRecording.getTexture(), 0);
        
        depthShader.setUniform1f("onlyDepth", 1);
        depthShader.setUniform1f("maxDistance", 5000);

        //distance
        depthShader.setUniform1f("near", minDistance);
        depthShader.setUniform1f("far", maxDistance);

        depthShader.setUniform1f("lens", 1.0); //lensFactor);

        //crop
        depthShader.setUniform1f("top", topMargin * resolution.y);
        depthShader.setUniform1f("bottom", resolution.y - bottomMargin * resolution.y);
        depthShader.setUniform1f("right", resolution.x - rightMargin * resolution.x);
        depthShader.setUniform1f("left", leftMargin * resolution.x);

        //offset
        depthShader.setUniform1f("x", 0.0); //-xKinect);
        depthShader.setUniform1f("y", 0.0); //yKinect);

        depthShader.setUniform1f("noiseT", 1.0); //scaleKinect);

        //Scale
        depthShader.setUniform1f("scale", 1.0); //scaleKinect);

        //keystone
        depthShader.setUniform1f("keystone", keystone);

        //correction
        depthShader.setUniform1f("correction", vertCorrection); //correctionKinect);

        ofClear(0, 0, 0, 255);
        ofSetColor(255,255,255,255);
        kinectRecording.draw(0.0,0.0, resolution.x, resolution.y);
        
        depthShader.end();
        kinectRecordingFbo.end();
        texture = kinectRecordingFbo.getTexture();
    }
}

ofTexture &KinectDevice::getTexture() {
    return texture;
}

ofVec3f &KinectDevice::getCom() {
    return com;
}

void KinectDevice::setAspect(float x, float y){
    setAspect(x, y, resolution.x, resolution.y);
}

void KinectDevice::setAspect(float x, float y, float width, float height) {
    KinectDevice::aspectPosition.set(x, y);
    KinectDevice::aspectSize.set(width, height);
}

void KinectDevice::draw() {
    ofSetColor(255);
    if(texture.isAllocated())
        texture.draw(aspectPosition.x, aspectPosition.y, aspectSize.x, aspectSize.y);
    
    ofDrawBitmapStringHighlight(fullName, aspectPosition.x + 20, aspectPosition.y + 20);
    if (isRunning()) {
        ofSetColor(0, 255, 0, 255);
    } else {
        ofSetColor(255, 0, 0, 255);
    }
    ofDrawRectangle(aspectPosition.x + 10, aspectPosition.y + 6, 4, 20);
    ofSetColor(0, 0, 255);
    ofDrawLine(aspectPosition.x + aspectSize.x / 2.0, aspectPosition.y, aspectPosition.x + aspectSize.x / 2.0, aspectPosition.y + aspectSize.y);
}

void KinectDevice::drawSelected() {
    ofSetColor(20, 97, 189);
    ofDrawRectangle(aspectPosition.x + 4, aspectPosition.y + 6, 4, 20);
}

float KinectDevice::convertToRealWorldX(float x, float depth) {
    return (x / resolution.x - 0.5f) * depth * XtoZ;
}

float KinectDevice::convertToRealWorldY(float y, float depth) {
    return (0.5f - y / resolution.y) * depth * YtoZ;
}
