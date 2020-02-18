//
//  KinectDepthEnum.h
//  ofxDepthStreamKinect
//
//  Created by Matteo Mestucci on 17/02/2020.
//

#ifndef KinectDepthEnum_h
#define KinectDepthEnum_h

enum KinectDepthEnum : unsigned int{
    KinectV2Depth                = 0,
    KinectAzureDepthNFOVUnbinned = 1,       /**< Depth captured at 640x576. Passive IR is also captured at 640x576. */
    KinectAzureDepthNFOVBinned   = 2,       /**< Depth captured at 320x288. Passive IR is also captured at 320x288. */
    KinectAzureDepthWFOVUnbinned = 3,       /**< Depth captured at 1024x1024. Passive IR is also captured at 1024x1024. */
    KinectAzureDepthWFOVBinned   = 4        /**< Depth captured at 512x512. Passive IR is also captured at 512x512. */
};

#endif /* KinectDepthEnum_h */
