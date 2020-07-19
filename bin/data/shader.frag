uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;
uniform sampler2D fboTexture;
uniform sampler2D videoTexture;

#pragma include "crt.glsl"
#pragma include "hg_sdf.glsl"

/// "RayMarching starting point" 
// by Martijn Steinrucken aka BigWings/CountFrolic - 2020
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
// 
// You can use this shader as a template for ray marching shaders

#define MAX_STEPS 100
#define MAX_DIST 100.
#define SURF_DIST .001

#define S smoothstep

mat2 Rot(float a) {
    float s = sin(a);
    float c = cos(a);
    return mat2(c, -s, s, c);
}

float Hash21(vec2 p) {
    p = fract(p*vec2(123.34,233.53));
    p += dot(p, p+23.234);
    return fract(p.x*p.y);
}

float GetDist(vec3 p) {
    return fCone(p, 1., 2.);
}

float RayMarch(vec3 ro, vec3 rd) {
	float dO=0.;
    
    for(int i=0; i<MAX_STEPS; i++) {
    	vec3 p = ro + rd*dO;
        float dS = GetDist(p);
        dO += dS;
        if(dO>MAX_DIST || abs(dS)<SURF_DIST) break;
    }
    
    return dO;
}

vec3 GetNormal(vec3 p) {
	float d = GetDist(p);
    vec2 e = vec2(.001, 0);
    
    vec3 n = d - vec3(
        GetDist(p-e.xyy),
        GetDist(p-e.yxy),
        GetDist(p-e.yyx));
    
    return normalize(n);
}

vec3 GetRayDir(vec2 uv, vec3 p, vec3 l, float z) {
    vec3 f = normalize(l-p),
        r = normalize(cross(vec3(0,1,0), f)),
        u = cross(f,r),
        c = f*z,
        i = c + uv.x*r + uv.y*u,
        d = normalize(i);
    return d;
}



void main()
{
  vec2 fb = gl_FragCoord.xy/u_resolution.xy;
  vec2 uv = (gl_FragCoord.xy-.5*u_resolution.xy)/u_resolution.y;
	vec2 m = vec2(abs(atan(u_time)), abs(atan(u_time)));
    
    vec3 col = vec3(0);
    
    vec3 ro = vec3(0, 3, -3);
    ro.yz *= Rot(-m.y*3.14+1.);
    ro.xz *= Rot(-m.x*6.2831);
    
    vec3 rd = GetRayDir(uv, ro, vec3(0), 1.);

    float d = RayMarch(ro, rd);
    
    if(d<MAX_DIST) {
    	vec3 p = ro + rd * d;
    	vec3 n = GetNormal(p);
        
    	float dif = dot(n, normalize(vec3(1,2,3)))*.5+.5;
    	col += dif;  
    }
    
    col = pow(col, vec3(.4545));	// gamma correction
    
    vec4 tex2 = crt(videoTexture, fb);
    fb.y = 1. - fb.y;
    vec4 tex = crt(fboTexture, abs(sin(fb)));
    tex *= vec4(atan(u_time), atan(u_time), atan(u_time), 1.);
    vec3 mixed = mix(col, tex.xyz, mix(.4, .5, abs(cos(u_time))));

    gl_FragColor = tex2 / vec4(mixed, 1.);
}
