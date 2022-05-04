// __multiversion__
// This signals the ading code to prepend either #version 100 or #version 300 es as apropriate.

#ifdef GL_FRAGMENT_PRECISION_HIGH
	#define hmp highp
#else
	#define hmp mediump
#endif

#include "fragmentVersionCentroid.h"
#include "uniformShaderConstants.h"
#include "util.h"

LAYOUT_BINDING(0) uniform sampler2D TEXTURE_0;

uniform vec4 FOG_COLOR;
uniform vec2 FOG_CONTROL;
uniform hmp float TOTAL_REAL_WORLD_TIME;

#include "config.h"
#include "noise.h"

varying vec3 pos;

void main()
{

vec4 diffuse;

vec3 p = pos;
p /= (p.y - 0.15);

if(pos.y - 0.15 >= 0.0) {
    discard;
}
 
float daylight = smoothstep(0.15, 0.25, FOG_COLOR.g);
float rain = smoothstep(0.5, 0.4, FOG_CONTROL.x);
float twilight = clamp(FOG_COLOR.r - FOG_COLOR.g, 0.0, 0.5) * 1.0;

#if defined(A_FBM_CLOUD)
    vec3 dayCloud = vec3(2.55, 2.55, 2.55);
    vec3 nightCloud = vec3(0.4, 0.4, 0.6);
    vec3 twilightCloud = vec3(1, 0.8, 0.3);
    vec3 rainCloud = vec3(0.2, 0.2, 0.2);

    vec3 cloudColor = mix(mix(mix(nightCloud, dayCloud, daylight), twilightCloud, twilight), rainCloud, rain);

    float cloudLower = mix(0.44, 0.0, 0.77);
    float cloud = fbm(p.xz * 1.2 + TOTAL_REAL_WORLD_TIME * 0.03, cloudLower, 0.7, 6);

#   if A_FBM_CLOUD == 1
        if(cloud > 0.0) {
            cloudColor *= mix(1.0, 0.85, fbm(p.xz + TOTAL_REAL_WORLD_TIME * 0.03, cloudLower, 1.0, 4));
        }
#   endif

    diffuse = vec4(cloudColor, 1.0);
    diffuse.a = mix(0.0, mix(0.0, diffuse.a, smoothstep(0.0, 0.85, p.y)), cloud);

#endif

gl_FragColor = diffuse;

}
