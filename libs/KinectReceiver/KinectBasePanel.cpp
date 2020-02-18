//
//  KinectBasePanel.cpp
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//

#include "KinectBasePanel.h"

void KinectBasePanel::addGuiComponents(KinectDevice *kinectDevice){
    KinectSettings kinectSettings = getDefaultKinectSettings();
    
    addGuiComponents(kinectDevice, kinectSettings);
}

void KinectBasePanel::addGuiComponents(KinectDevice *kinectDevice, KinectSettings kinectSettings){
    KinectBasePanel::kinectDevice = kinectDevice;
    
    this->add(connectReleaseButton.setup("CONNECT / RELEASE"));
    connectReleaseButton.addListener(kinectDevice, &KinectDevice::connectRelease);
    
    this->add(saveButton.setup("SAVE"));
    saveButton.addListener(this, &KinectBasePanel::save);
    
    this->add(minDistanceIntSlider.setup("MIN DISTANCE", kinectSettings.minDistance.d, kinectSettings.minDistance.min, kinectSettings.minDistance.max));
    minDistanceIntSlider.addListener(kinectDevice, &KinectDevice::setMinDistance);
    
    this->add(maxDistanceIntSlider.setup("MAX DISTANCE", kinectSettings.maxDistance.d, kinectSettings.maxDistance.min, kinectSettings.maxDistance.max));
    maxDistanceIntSlider.addListener(kinectDevice, &KinectDevice::setMaxDistance);
    
    this->add(leftMarginIntSlider.setup("LEFT MARGIN", kinectSettings.leftMargin.d, kinectSettings.leftMargin.min, kinectSettings.leftMargin.max));
    leftMarginIntSlider.addListener(kinectDevice, &KinectDevice::setLeftMargin);
    
    this->add(rightMarginIntSlider.setup("RIGHT MARGIN", kinectSettings.rightMargin.d, kinectSettings.rightMargin.min, kinectSettings.rightMargin.max));
    rightMarginIntSlider.addListener(kinectDevice, &KinectDevice::setRightMargin);
    
    this->add(topMarginIntSlider.setup("TOP MARGIN", kinectSettings.topMargin.d, kinectSettings.topMargin.min, kinectSettings.topMargin.max));
    topMarginIntSlider.addListener(kinectDevice, &KinectDevice::setTopMargin);
    
    this->add(bottomMarginIntSlider.setup("BOTTOM MARGIN", kinectSettings.bottomMargin.d, kinectSettings.bottomMargin.min, kinectSettings.bottomMargin.max));
    bottomMarginIntSlider.addListener(kinectDevice, &KinectDevice::setBottomMargin);
    
    this->add(keystoneFloatSlider.setup("KEYSTONE", kinectSettings.keystone.d, kinectSettings.keystone.min, kinectSettings.keystone.max));
    keystoneFloatSlider.addListener(kinectDevice, &KinectDevice::setKeystone);
    
    this->add(vertCorrectionFloatSlider.setup("VERTICAL CORRECTION", kinectSettings.vertCorrection.d, kinectSettings.vertCorrection.min, kinectSettings.vertCorrection.max));
    vertCorrectionFloatSlider.addListener(kinectDevice, &KinectDevice::setVertCorrection);
    
    this->add(clearImageButton.setup("CLEAR IMAGE"));
    clearImageButton.addListener(kinectDevice, &KinectDevice::clearImage);
}

void KinectBasePanel::save(){
    if(filename.size() > 1){
        this->saveToFile(filename);
    }
    else if(kinectDevice != NULL){
        filename = kinectDevice->getName()+ "kinect.xml";
    }
    else{
        filename = "kinectPasePanel.xml";
        ofLogWarning("KinectBasePanal", "filename not setted");
    }
    
    saveToFile(filename);
}
