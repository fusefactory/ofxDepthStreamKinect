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

	boundingBox_image.allocate(kinect->getResolution().x, kinect->getResolution().y);

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

	boundingBox_image.clear();
	boundingBox_image.set(0);
	boundingBox_image.getTexture().readToPixels(boundingPixels);
	for (int x = 0; x < kinect->getResolution().x; x++) {
		for (int y = 0; y < kinect->getResolution().y; y++) {
			int index = boundingPixels.getPixelIndex(x, y);
			//boundingPixels[index];

			float min_realX = kinect->convertToMapWorldX(x, y, kinect->getMinDistance());
			float min_realY = kinect->convertToMapWorldY(x, y, kinect->getMinDistance());

			float max_realX = kinect->convertToMapWorldX(x, y, kinect->getMaxDistance());
			float max_realY = kinect->convertToMapWorldY(x, y, kinect->getMaxDistance());

			if (abs(min_realX) <= Box_Xt && min_realY >= Box_Yt1 && min_realY <= Box_Yt2) {
				boundingPixels.setColor(index, ofColor(255, 255));
			}
			else if (abs(max_realX) <= Box_Xb && max_realY >= Box_Yb1 && max_realY <= Box_Yb2) {
				boundingPixels.setColor(index, ofColor(255, 255));
			}
			else {
				boundingPixels.setColor(index, ofColor(0,0));
			}
		}
	}
	boundingBox_image.setFromPixels(boundingPixels);
	//colorImage.set(255);
	colorImage.getTexture().setAlphaMask(boundingBox_image.getTexture());

	//test-> draw before alpha
    

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
    fbo.draw(x, y, width, height);
	//colorImage.draw(x, y + height + 4, width, height);
	colorImage.draw(x, y + height + 4, width, height);
    alpha_mask.draw(x, y + height + 4, width, height);
	boundingBox_image.draw(x, y + height + 4, width, height);
    glDisable(GL_BLEND);
	boundingBox_image.draw(x + width + 4, y, width, height);
	//colorImage.draw(0,0, width, height);
}


void KinectObjectTracker::setBoundingBox(float Yt1, float Yt2, float Xt, float Yb1, float Yb2, float Xb) {

	Box_Yt1 = Yt1;
	Box_Yt2 = Yt2;
	Box_Xt = Xt;
	Box_Yb1 = Yb1;
	Box_Yb2 = Yb2;
	Box_Xb = Xb;
}


//ofTexture KinectObjectTracker::getTexture() {
//	return colorImage.getTexture();
//}

#endif
