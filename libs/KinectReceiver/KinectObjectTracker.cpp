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
    //colorImage.setFromPixels(depthPixels);

	boundingBox_image.clear();
	boundingBox_image.set(0);
	boundingBox_image.getTexture().readToPixels(boundingPixels);
	for (int x = 0; x < kinect->getResolution().x; x++) {
		for (int y = 0; y < kinect->getResolution().y; y++) {
			int index = boundingPixels.getPixelIndex(x, y);

			float min_realX = kinect->convertToMapWorldX(x, y, kinect->getMinDistance());
			float min_realY = kinect->convertToMapWorldY(x, y, kinect->getMinDistance());

			float mid_realX = kinect->convertToMapWorldX(x, y, ofMap(0.25, 0.0, 1.0, kinect->getMinDistance(), kinect->getMaxDistance()));
			float mid_realY = kinect->convertToMapWorldY(x, y, ofMap(0.25, 0.0, 1.0, kinect->getMinDistance(), kinect->getMaxDistance()));
			float mid2_realX = kinect->convertToMapWorldX(x, y, ofMap(0.75, 0.0, 1.0, kinect->getMinDistance(), kinect->getMaxDistance()));
			float mid2_realY = kinect->convertToMapWorldY(x, y, ofMap(0.75, 0.0, 1.0, kinect->getMinDistance(), kinect->getMaxDistance()));

			float point_maxY = ofMap(0.25, 0.0, 1.0, Box_Yt2, Box_Yb2);
			float point_minY = ofMap(0.25, 0.0, 1.0, Box_Yt1, Box_Yb1);
			float point_maxX = ofMap(0.25, 0.0, 1.0, Box_Xt, Box_Xb);
			float point2_maxY = ofMap(0.75, 0.0, 1.0, Box_Yt2, Box_Yb2);
			float point2_minY = ofMap(0.75, 0.0, 1.0, Box_Yt1, Box_Yb1);
			float point2_maxX = ofMap(0.75, 0.0, 1.0, Box_Xt, Box_Xb);

			float max_realX = kinect->convertToMapWorldX(x, y, kinect->getMaxDistance());
			float max_realY = kinect->convertToMapWorldY(x, y, kinect->getMaxDistance());


			//int depthIndex = depthPixels.getPixelIndex(x, y);
			//float depth = (255.0 - depthPixels[index]) / 255.0 * (kinect->getMaxDistance() - kinect->getMinDistance()) + kinect->getMinDistance();
			////float depth = (255.0 - depthPixels[index]) / 255.0;

			//float point_realX = kinect->convertToMapWorldX(x, y, depth);
			//float point_realY = kinect->convertToMapWorldY(x, y, depth);
			//float point_maxY = ofMap(depth, kinect->getMinDistance(), kinect->getMaxDistance(), Box_Yt2, Box_Yb2);
			//float point_minY = ofMap(depth, kinect->getMinDistance(), kinect->getMaxDistance(), Box_Yt1, Box_Yb1);
			//float point_maxX = ofMap(depth, kinect->getMinDistance(), kinect->getMaxDistance(), Box_Xt, Box_Xb);

			//if (abs(point_realX) <= point_maxX && point_realY >= point_minY && point_realY <= point_maxY) {
			//	depth = 0;
			//}
			//else {
			//	depthPixels.setColor(x, y, ofColor(0));
			//}


			if (abs(min_realX) <= Box_Xt && min_realY >= Box_Yt1 && min_realY <= Box_Yt2) {
				boundingPixels.setColor(index, ofColor(255, 255));
			}
			else if (abs(mid_realX) <= point_maxX && mid_realY >= point_minY && mid_realY <= point_maxY) {
				boundingPixels.setColor(index, ofColor(255, 255));
			}
			else if (abs(mid2_realX) <= point2_maxX && mid2_realY >= point2_minY && mid2_realY <= point2_maxY) {
				boundingPixels.setColor(index, ofColor(255, 255));
			}
			else if (abs(max_realX) <= Box_Xb && max_realY >= Box_Yb1 && max_realY <= Box_Yb2) {
				boundingPixels.setColor(index, ofColor(255, 255));
			}
			else {
				boundingPixels.setColor(index, ofColor(0,0));
				depthPixels.setColor(x, y, ofColor(0));
			}
		}
	}
	boundingBox_image.setFromPixels(boundingPixels);
	//colorImage.set(255);
	//colorImage.getTexture().setAlphaMask(boundingBox_image.getTexture());

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


	alpha_mask.getTexture().readToPixels(alphaPixels);
	for (int x = 0; x < kinect->getResolution().x; x++) {
		for (int y = 0; y < kinect->getResolution().y; y++) {

			if (alphaPixels.getColor(x, y) == ofColor(0)) {
				depthPixels.setColor(x, y, ofColor(0));
			}

			int index = alphaPixels.getPixelIndex(x, y);
		}
	}
	colorImage.setFromPixels(depthPixels);
	colorImage.dilate();
	colorImage.getTexture().readToPixels(depthPixels);


	////colorImage.blurGaussian();
	//colorImage.getTexture().setAlphaMask(alpha_mask.getTexture());
	////colorImage.erode();
	////colorImage.blurGaussian();
	//colorImage.dilate();
	////colorImage.dilate();
	////colorImage.blurGaussian();
	//colorImage.getTexture().readToPixels(depthPixels);
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
	alpha_mask.draw(x + width + 4, y, width, height);
	colorImage.draw(x + width + 4, y + height + 4, width, height);
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
