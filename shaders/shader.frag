#version 450

#define PI 3.14159265359

layout(binding = 0) uniform UBO{
  	vec4  vP;
	vec4  vD;
	vec4  lP;
    float time;
    uvec2 res;
    float sharp;
} ubo;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 vP;
layout(location = 4) in vec3 vD;//Maybe problem
layout(location = 5) in vec3 wP;

layout(location = 0) out vec4 outColor;

vec3 aces_tonemap(vec3 color){
	const mat3 m1 = mat3(
        .59719, .07600, .02840,
        .35458, .90834, .13383,
        .04823, .01566, .83777
	);
	const mat3 m2 = mat3(
        1.60475, -.10208, -.00327,
        -.53108, 1.10813, -.07276,
        -.07367, -.00605, 1.07602
	);
	vec3 v = m1 * color;
	vec3 a = v * (v + 0.0245786) - 0.000090537;
	vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
	return pow(clamp(m2 * (a / b), 0.0, 1.0), vec3(1.0 / 2.2));
}

uvec2 res     = ubo.res;
vec4  fragPos = gl_FragCoord;
float time    = ubo.time;

float line(vec2 v, float f, float w){
    if(v.y > f - w && v.y < f)
    return 1.; 
    else
    return .0;
}

void main(){
    vec2 vpuv = fragPos.xy/res;
    vpuv.y    = 1. - vpuv.y;
    vec2 px   = 1. / res;

    float x = vpuv.x;
    float y = smoothstep(0.,.5.x.x,x);
//     outColor.rgb  = vec3(y * 0.25);
//     outColor.rgb += vec3(1.,0.,0.) * line(vpuv.xy,.0011,px.y);
//     outColor.rgb += vec3(0.,1.,0.) * line(vpuv.xy,    y,px.y);

    if(vpuv.x < .5f)
    outColor.rgb = vec3(texture(texSampler,fragTexCoord).rgb);
    else
    outColor.rgb = aces_tonemap(texture(texSampler,fragTexCoord).rgb);
}
