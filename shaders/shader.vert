#version 450
layout(binding = 0) uniform UBO{
  	vec4 vP;
	vec4 vD;
	vec4 lP;
    float time;
    uvec2 res;
    float sharp;
} ubo;

layout(push_constant) uniform MVPConst{
    mat4 m;
    mat4 v;
    mat4 p; 
} mvp;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 normal;
layout(location = 3) out vec3 vP;
layout(location = 4) out vec3 vD;
layout(location = 5) out vec3 wP;

void main(){
    vec4     wP = mvp.m * vec4(inPosition,1.0f);
    mat4    mat = mvp.p * mvp.v;
    gl_Position = mat * wP;

            normal = mat3(mvp.m) * inNormal;
                vD = normalize(vP - wP.xyz);
      fragTexCoord = inTexCoord;
         fragColor = inColor;
}
