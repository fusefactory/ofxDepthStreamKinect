//
//  KinectHandTracker.cpp
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//
//

#ifdef ofxDepthStreamKinectReceiver


#include "KinectHandTracker.h"

KinectHandTracker::KinectHandTracker(KinectDevice *kinect) : KinectObjectTracker(kinect) {
}

void KinectHandTracker::update(int maxBlobs) {
    KinectObjectTracker::update(maxBlobs);
    calculateTrackedHands();
}

void KinectHandTracker::setNoiseTh(float threshold) {
	noise_th = threshold;
}

void KinectHandTracker::calculateTrackedHands() {
    hands.clear();
    
    for (ofxCvBlob &blob : contourFinder.blobs) {
//        int index = depthPixels.getPixelIndex(blob.centroid.x, blob.centroid.y);
//        float depth = (255 - depthPixels[index]) / 255.0 * (kinect->getMaxDistance() - kinect->getMinDistance()) + kinect->getMinDistance();
		
		float min_depth = 100000;
		float max_depth = 0;
		float max_y = 100000;
		float max_y_depth = 0;
		float curr_x = 0;
		float curr_y = 0;
		float curr_x_depth = 0;
		float curr_y_depth = 0;
        // average blob/hand depth
        float handDepth = 0;
        int validPixels = 0;
        for (int x = 0; x < blob.boundingRect.width; x++) {
            for (int y = 0; y < blob.boundingRect.height; y++) {
                int index = depthPixels.getPixelIndex(blob.boundingRect.x + x, blob.boundingRect.y + y);
                float depth = (255.0 - depthPixels[index]) / 255.0;
                if (depth > 0) {
                    handDepth += depth;
                    validPixels++;

					//if (depth < min_depth) {
					//	min_depth = depth;
					//	curr_x_depth = blob.boundingRect.x + x;
					//	curr_y_depth = blob.boundingRect.y + y;
					//}

					//float real_y = kinect->convertToRealWorldY(blob.boundingRect.y + y, depth);
					//if (y < max_y) {
					//	max_y_depth = depth;
					//	max_y = real_y;
					//	curr_x = blob.boundingRect.x + x;
					//	curr_y = blob.boundingRect.y + y;
					//}
                }
            }
        }
        if (validPixels > 0) {
            handDepth /= (float)validPixels;
        }
		float handDepth_map = handDepth;
		handDepth = handDepth * (kinect->getMaxDistance() - kinect->getMinDistance()) + kinect->getMinDistance();
		//min_depth = min_depth * (kinect->getMaxDistance() - kinect->getMinDistance()) + kinect->getMinDistance();

		for (int x = 0; x < blob.boundingRect.width; x++) {
			for (int y = 0; y < blob.boundingRect.height; y++) {
				int index = depthPixels.getPixelIndex(blob.boundingRect.x + x, blob.boundingRect.y + y);
				float depth = (255.0 - depthPixels[index]) / 255.0;
				if (depth > 0 && abs(handDepth_map - depth) < noise_th) {
					depth = depth * (kinect->getMaxDistance() - kinect->getMinDistance()) + kinect->getMinDistance();
					if (depth < min_depth) {
						min_depth = depth;
						curr_x_depth = blob.boundingRect.x + x;
						curr_y_depth = blob.boundingRect.y + y;
					}

					float real_y = kinect->convertToRealWorldY(blob.boundingRect.y + y, depth);
					if (real_y < max_y) {
						max_y_depth = depth;
						max_y = real_y;
						curr_x = blob.boundingRect.x + x;
						curr_y = blob.boundingRect.y + y;
					}
				}
			}
		}

        float handX = kinect->convertToRealWorldX(blob.centroid.x, handDepth);
		float handZ = kinect->convertToRealWorldY(blob.centroid.y, handDepth); // not necessary here

		float topX = kinect->convertToRealWorldX(curr_x_depth, min_depth);
		float topZ = kinect->convertToRealWorldY(curr_y_depth, min_depth);

		float frontX = kinect->convertToRealWorldX(curr_x, max_y_depth);
		float frontZ = max_y;
		//float frontZ = kinect->convertToRealWorldY(curr_y, max_y_depth);
        
        TrackedHand hand;
		//---center
        hand.center.x = handX;
        hand.center.y = handDepth; // bottom kinect
        hand.width = blob.boundingRect.width;
        hand.height = blob.boundingRect.height;
		hand.center_z = handZ;

		//---top
		hand.top.x = topX;
		hand.top.y = min_depth;
		hand.top_z = topZ;
		
		hand.top_on_texture.x = curr_x_depth;
		hand.top_on_texture.y = curr_y_depth;

		//---front
		hand.front.x = frontX;
		hand.front.y = max_y_depth;
		hand.front_z = frontZ;

		hand.front_on_texture.x = curr_x;
		hand.front_on_texture.y = curr_y;

		hand.range_start = kinect->convertToRealWorldY(blob.boundingRect.y, handDepth);
		hand.range_end = kinect->convertToRealWorldY(blob.boundingRect.y + blob.boundingRect.height, handDepth);
        
        hands.push_back(hand);
    }
}

std::vector<TrackedHand> KinectHandTracker::getTrackedHands() {
    return hands;
}

ofVec4f KinectHandTracker::getPointsInfo(int cx, int cy) {
	//ofVec3f out = ofVec3f(0, 0, 0);
	ofVec4f out = ofVec4f(0, 0, 0, 0);
	int index = depthPixels.getPixelIndex(cx, cy);
	float depth = (255.0 - depthPixels[index]) / 255.0 * (kinect->getMaxDistance() - kinect->getMinDistance()) + kinect->getMinDistance();
	float depth_abs = (255.0 - depthPixels[index]) / 255.0;
	float cursorX = kinect->convertToRealWorldX(cx, depth);
	float cursorY = kinect->convertToRealWorldY(cy, depth);

	out.x = cursorX;
	out.y = depth;
	out.z = cursorY;
	out.w = depth_abs;

	return out;
}

#endif