//
//  KinectObjectTracker.cpp
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//
//

#ifdef ofxDepthStreamKinectReceiver

#include "KinectObjectTracker.h"

KinectObjectTracker::KinectObjectTracker(KinectDevice *kinect) {
    KinectObjectTracker::kinect = kinect;
    
    colorImage.allocate(kinect->getResolution().x, kinect->getResolution().y);
    grayImage.allocate(kinect->getResolution().x, kinect->getResolution().y);
    
    fbo.allocate(kinect->getResolution().x, kinect->getResolution().y, GL_RGBA);
    fbo.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
	
	alpha_mask.allocate(kinect->getResolution().x, kinect->getResolution().y, GL_RGBA);
	alpha_mask.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);

	alpha_image.allocate(kinect->getResolution().x, kinect->getResolution().y);
}

void KinectObjectTracker::setMinArea(int &area) {
    objectMinArea = area;
}

int KinectObjectTracker::getMinArea() {
    return objectMinArea;
}

void KinectObjectTracker::setMaxArea(int &area) {
    objectMaxArea = area;
}

int KinectObjectTracker::getMaxArea() {
    return objectMaxArea;
}

void KinectObjectTracker::toggleBlobs(bool &value) {
    showBlobs = value;
}

bool KinectObjectTracker::showingBlobs() {
    return showBlobs;
}

void KinectObjectTracker::update(int maxBlobs) {
    kinect->getTexture().readToPixels(depthPixels);
    colorImage.setFromPixels(depthPixels);
	//colorImage.erode();
    grayImage = colorImage;
    
	//grayImage.blurGaussian();
    contourFinder.findContours(grayImage, objectMinArea, objectMaxArea, maxBlobs, false);

	alpha_mask.begin();
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	ofPushStyle();
	ofSetLineWidth(0);
	ofBackground(0);
	ofSetColor(255);
	ofFill();
	for (ofxCvBlob &blob : contourFinder.blobs) {
		ofFill();
		ofSetColor(255);
		ofBeginShape();
		for (int i = 0; i < blob.nPts; i++) {
			ofVertex(blob.pts[i].x, blob.pts[i].y);
		}
		ofEndShape(true);
	}
	alpha_mask.end();
	ofPopStyle();

	//colorImage.blurGaussian();
	colorImage.getTexture().setAlphaMask(alpha_mask.getTexture());
	//colorImage.erode();
	//colorImage.blurGaussian();
	colorImage.dilate();
	//colorImage.dilate();
	//colorImage.blurGaussian();
	colorImage.getTexture().readToPixels(depthPixels);
}

vector<ofxCvBlob> KinectObjectTracker::getBlobs() {
    return contourFinder.blobs;
}

void KinectObjectTracker::drawBlobs(float x, float y, float width, float height) {
    float factor = fbo.getWidth() / width;
    
    fbo.begin();
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    ofPushStyle();
    ofSetLineWidth(factor);
    for (ofxCvBlob &blob : contourFinder.blobs) {
//        ofNoFill();
        ofSetColor(255, 255, 0, 127);
        ofDrawRectangle(blob.boundingRect.x, blob.boundingRect.y, blob.boundingRect.width, blob.boundingRect.height);
        ofFill();
        ofSetColor(255, 255, 0, 255);
        ofDrawEllipse(blob.centroid.x, blob.centroid.y, 10 * factor, 10 * factor);
    }
    fbo.end();
	ofPopStyle();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ofSetColor(255);
    //fbo.draw(x, y, width, height);
	//colorImage.draw(x, y + height + 4, width, height);
    alpha_mask.draw(x, y + height + 4, width, height);
	colorImage.draw(x, y, width, height);
    glDisable(GL_BLEND);
}


//ofTexture KinectObjectTracker::getTexture() {
//	return colorImage.getTexture();
//}

#endif
