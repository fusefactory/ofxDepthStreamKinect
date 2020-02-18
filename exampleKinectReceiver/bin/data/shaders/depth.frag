#version 330

uniform sampler2DRect tex0;

#define M_PI 3.1415926535897932384626433832795

//type
uniform bool onlyDepth;

//Distance
uniform float near;
uniform float far;

uniform float lens;
uniform float edge;

//Crop
uniform float top;
uniform float bottom;
uniform float right;
uniform float left;
uniform float x, y;
uniform float scale;
uniform float keystone;
uniform float correction;
uniform float noiseT;
uniform float maxDistance;

const float kinectWidth = 512.0;
const float kinectHeight = 424.0;
const float kinectHalfWidth = 256.0;
const float kinectHalfHeight = 212.0;

const float low2 = 1.0;

float myFar;


in float discardPixel;

in vec4 depth;
in vec2 vertTexCoord;

out vec4 outColor;

void crop(const vec2 pos){
    if ( pos.x < left || pos.x > right || pos.y < top || pos.y > bottom){// pos.y > 288.0 // || pos.y > 235.0 || pos.y < 75.0) {
        discard;
    }
}

//from https://forum.unity.com/threads/round-not-supported-in-shaders.144316/
float approxRound(const float f){
    return floor(f) + 0.5;
}

//vec2 moveAndScaleAndKeystone(vec2 pos){  //now this part runs in the vert shader
////    pos.x = pos.x / scale + x;
////    pos.y = pos.y / scale + y;
//
//    float relYFromCenter = (pos.y - kinectHalfHeight) / kinectHalfHeight ; //from -1 to 1
//    float absXFromCenter = (pos.x - kinectHalfWidth);
//
//    pos.x += relYFromCenter * keystone * absXFromCenter;
//
//    return pos;
//}

float depthValue(vec2 pos){
    
    float valueTex = texture(tex0, vec2(approxRound(pos.x), approxRound(pos.y))).x;
    if(valueTex <= 0.05) valueTex = 1.0;    // ===========> IMPORTANT!!!
    float value = maxDistance * (1.0-valueTex); // depending on kinect recording: 1.0- needed if near staff are white.

//vec4 value = vec4(texture2DRect(fakeKinect, vec2(texCoordVarying.s,texCoordVarying.t)).rgb,1.0);
    //    myFar += correction * (1.0 - (kinectHeight/2.0 + pos.y) / kinectHeight);

    myFar = far * (1.0 -  correction * (cos(M_PI / 3.0 * (kinectHeight - pos.y) / kinectHeight) - 0.5));

//    return clamp(value / far, 0.0, 1.0);
//    float valuePost = value/(far-near);
//    if(valueTex>=noiseT) return 1.0;
//    else return valueTex;
    return clamp(low2 + (value - near) / (near - myFar), 0.0, 1.0);

}

void main()
{
    //    myFar = far;
    
    vec2 pos = vertTexCoord.xy;

    
//    pos = moveAndScaleAndKeystone(pos);       //move and scale and keystone
    crop(pos);                               //crop (top, bottom, left, right)
    
    float d = depthValue(pos);     //d is depth value for kinect

//    if (discardPixel == 1.0){
    if (d >= noiseT || discardPixel == 1.0){
//        if (d >= noiseT){
        discard;
    }
    else if (! onlyDepth){
        //edge detection
        int white = 0;
        int black = 0;
        int inc = 4;
        for(int ky = -inc; ky <= inc; ky+=inc){
            for(int kx = -inc; kx <= inc; kx+=inc){
                vec2 aux = vec2(pos.x+float(kx), pos.y+float(ky));
                float dd = clamp(low2 + (maxDistance*texture(tex0, aux).r - near) / (near - myFar), 0.0, 1.0);
//                if (dd < 0.01) {
//                    discard;
//                }
                
                if(dd > 0.01 && dd < noiseT && d - dd < edge){ // edge was = 0.05
//                if(dd > 0.01 && d - dd < edge){ // edge was = 0.05
                    white++;
                }
                else if (dd > 0.01  && ky < 0){
                    black++;
                }
            }
        }
//        if(white > 4 && black > 1){// white > 4 && black < 2){
        if(white > 1 && black > 1){// white > 4 && black < 2){
            //ok
        }
        else{
            d = 0.0;
        }
    }
//    gl_FragColor = vec4(1.0,0.0,0.0,1.0);
//    gl_FragColor = depth; //vec4(texture2DRect(fakeKinect, texCoordVarying.xy).rgb,1.0);
//    outColor = vec4(texture(tex0,vec2(vertTexCoord.x,vertTexCoord.y)).xyz,1.0); //
    outColor = vec4(vec3(d,d,d), 1.0);
//    outColor = vec4(0.0,1.0,1.0, 1.0);

}
