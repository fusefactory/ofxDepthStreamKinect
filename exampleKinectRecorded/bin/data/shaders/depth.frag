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

uniform float kinectWidth;
uniform float kinectHeight;
uniform float kinectHalfWidth;
uniform float kinectHalfHeight;

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

float depthValue(vec2 pos){
    float valueTex = texture(tex0, vec2(approxRound(pos.x), approxRound(pos.y))).x;
    if(valueTex <= 0.05) valueTex = 1.0;    // ===========> IMPORTANT!!!
    float value = maxDistance * (1.0-valueTex); // depending on kinect recording: 1.0- needed if near staff are white.

    myFar = far * (1.0 -  correction * (cos(M_PI / 3.0 * (kinectHeight - pos.y) / kinectHeight) - 0.5));

    return clamp(low2 + (value - near) / (near - myFar), 0.0, 1.0);
}

void main(){
    vec2 pos = vertTexCoord.xy;

    crop(pos);                               //crop (top, bottom, left, right)
    
    float d = depthValue(pos);     //d is depth value for kinect

    if (d >= noiseT || discardPixel == 1.0){
        discard;
    }
   
    outColor = vec4(vec3(d,d,d), 1.0);
}
