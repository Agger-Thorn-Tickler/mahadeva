uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;
uniform float u_frame;
uniform float u_midi0;
uniform float u_midi1;
uniform float u_midi2;
uniform float u_midi3;
uniform sampler2D videoTexture;
uniform sampler2D sdfTexture;

#include "crt.glsl"
#include "hg_sdf.glsl"
#include "PixelSpiritDeck/lib/heartSDF.glsl";
#include "PixelSpiritDeck/lib/flowerSDF.glsl";
#include "PixelSpiritDeck/lib/starSDF.glsl";
#include "PixelSpiritDeck/lib/vesicaSDF.glsl";


void main()
{
  vec2 uv = (gl_FragCoord.xy-.5*u_resolution.xy)/u_resolution.y;
  vec2 fb = gl_FragCoord.xy/u_resolution.xy;
  vec4 tex = crt(videoTexture, fb);
  vec4 tex2 = crt(sdfTexture, fb);

  vec4 mixed = mix(tex, tex2, clamp(.2, .8, abs(sin(u_time))));

  vec3 col = vec3(vesicaSDF(fract((10. * u_midi3) * uv), (u_midi1)));//, u_midi2));

  gl_FragColor = mixed * mix(mixed, vec4(col, 1.), 1./u_midi0);
}
