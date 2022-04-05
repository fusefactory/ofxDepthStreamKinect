//
//  TrackedHand.h
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//
//

#ifndef TrackedHand_h
#define TrackedHand_h

class TrackedHand {
    
public:
	//center: {x_pos, depth} | z: distance from screen

    ofVec2f center;
    ofVec2f top;
    ofVec2f front;
	float center_z;
	float top_z;
	float front_z;

	ofVec2f top_on_texture;
	ofVec2f front_on_texture;

	float width;
	float height;
	float range_start;
	float range_end;
};

#endif /* TrackedHand_h */
