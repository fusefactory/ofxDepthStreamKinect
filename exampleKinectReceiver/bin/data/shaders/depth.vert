#version 330
//#extension GL_ARB_texture_rectangle : enable

//uniform mat4 modelViewProjectionMatrix;
uniform mat4 projection;
uniform mat4 modelview;

uniform sampler2DRect tex0;

uniform float near;
uniform float far;
uniform float lens;
uniform float orizFow;

uniform float x, y;
uniform float scale;

uniform float kinectWidth;
uniform float kinectHeight;
uniform float kinectHalfWidth;
uniform float kinectHalfHeight;

uniform float keystone;

in vec4 position;
in vec2 texcoord;

out vec4 depth;
out vec2 vertTexCoord;

out float discardPixel;

vec2 moveAndScaleAndKeystone(vec2 pos){
    pos.x = pos.x / scale + x;
    pos.y = pos.y / scale + y;
    
    float relYFromCenter = (pos.y - kinectHalfHeight) / kinectHalfHeight ; //from -1 to 1
    float absXFromCenter = (pos.x - kinectHalfWidth);
    
    pos.x += relYFromCenter * keystone * absXFromCenter;
    
    return pos;
}

void main(){
    vertTexCoord = texcoord;
    vec4 posMod = position;
    
    discardPixel = 0.0;
    
    posMod.xy = moveAndScaleAndKeystone(posMod.xy);

    gl_PointSize = 1.0;
    vec4 eyePos = modelview * posMod;
    gl_Position = projection * eyePos;
}
