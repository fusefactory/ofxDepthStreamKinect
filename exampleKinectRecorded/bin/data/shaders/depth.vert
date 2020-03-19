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

const float kinectWidth = 512.0;
const float kinectHeight = 424.0;
const float kinectHalfWidth = 256.0;
const float kinectHalfHeight = 256.0;

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

vec2 orthoCameraWithAngle(vec2 pos) {
    float angCx = ((pos.x - kinectHalfWidth) / kinectHalfWidth) * radians(orizFow);
    depth = vec4(texture(tex0, vec2(vertTexCoord.s,vertTexCoord.t)).rgb,1.0);
    float posX = (depth.x * tan(angCx) * depth.x * lens) / 1000.0;
    pos.x = (posX / (depth.x * tan(radians(orizFow)))) * kinectHalfWidth + 256.0;
    return pos;
}

vec2 orthoCameraWithDist(vec2 pos) {
        depth = vec4(texture(tex0, vec2(vertTexCoord.s,vertTexCoord.t)).rgb,1.0);
        float zFactor = lens * depth.x / 1000.0;
        pos.x = 256.0/scale + x + zFactor * (pos.x - 256.0/scale - x);
    return pos;
}


void main()
{
    vertTexCoord = texcoord;
    vec4 posMod = position;
    
    discardPixel = 0.0;
    
    posMod.xy = moveAndScaleAndKeystone(posMod.xy);
    
//    posMod.xy = orthoCameraWithDist(posMod.xy); // mehod I
//    posMod.xy = orthoCameraWithAngle(posMod.xy); // method II

    
    
    
//    depth = vec4(1.0,0.0,0.0,1.0);
//    if(gl_TexCoord[0].x < 500)
//        depth = vec4(0.0,1.0,0.0,1.0);

//    depth = vec4(texture2DRect(tex0, vec2(texCoordVarying.s,texCoordVarying.t)).rgb,1.0);
//    float d2 = depth.x;
//    float zFactor = lens * depth.x / 1000.0;


//    posMod.x = 256.0/scale + x + zFactor * (posMod.x - 256.0/scale - x);
    
//    depth = clamp(1.0 + (depth - near) / (near - far), 0.0, 1.0);

    
//    if(d2 < 0.1) {
//        discardPixel = 1.0;
//    }
    

    gl_PointSize = 1.0;
    vec4 eyePos = modelview * posMod; //vec4(200.0,200.0,0.0,1.0);
    gl_Position = projection * eyePos;
//    gl_Position = vec4(0.0,0.0,0.0,1.0);
//    gl_Position = modelViewProjectionMatrix * posMod;
    
    
    //    gl_Position = position;
    //
    //    if (flip) {
    //        vertTexCoord = vec2(3940 - texcoord.x, texcoord.y);
    //    } else {
    //        vertTexCoord = vec2(texcoord.x, 2160 - texcoord.y);
    //    }
    
}
