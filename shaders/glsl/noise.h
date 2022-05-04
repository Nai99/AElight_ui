#ifndef A_NOISE_INCLUDED
#define A_NOISE_INCLUDED

hmp float hash(hmp vec2 p) {hmp vec3 p3 = fract(vec3(p.xyx) * 0.13); p3 += dot(p3, p3.yzx + 3.333); return fract((p3.x + p3.y) * p3.z); }

hmp float noise(hmp vec2 x) {
    hmp vec2 i = floor(x);
    hmp vec2 f = fract(x);

	// Four corners in 2D of a tile
	hmp float a = hash(i);
    hmp float b = hash(i + vec2(1.0, 0.0));
    hmp float c = hash(i + vec2(0.0, 1.0));
    hmp float d = hash(i + vec2(1.0, 1.0));

    // Simple 2D mix using smoothstep envelope between the values.
	// return vec3(mix(mix(a, b, smoothstep(0.0, 1.0, f.x)),
	//			mix(c, d, smoothstep(0.0, 1.0, f.x)),
	//			smoothstep(0.0, 1.0, f.y)));

	// Same code, with the clamps in smoothstep and common subexpressions
	// optimized away.
    hmp vec2 u = f * f * (3.0 - 2.0 * f);
	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

hmp vec3 mod289(hmp vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

hmp vec2 mod289(hmp vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

hmp vec3 permute(hmp vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

hmp float snoise(hmp vec2 v)
  {
  const hmp vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  hmp vec2 i  = floor(v + dot(v, C.yy) );
  hmp vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  hmp vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  hmp vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  hmp vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  hmp vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  hmp vec3 x = 2.0 * fract(p * C.www) - 1.0;
  hmp vec3 h = abs(x) - 0.5;
  hmp vec3 ox = floor(x + 0.5);
  hmp vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.853734568614 * ( a0*a0 + h*h );

// Compute final noise value at P
  hmp vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

// Xor - Infinite Worley Noise 
// https://www.shadertoy.com/view/4l2GzW
hmp float wnoise(hmp vec2 P)
{
    hmp float Dist = 1.;
    hmp vec2 I = floor(P);
    hmp vec2 F = fract(P);
    
    for(int X = -1;X<=1;X++)
    for(int Y = -1;Y<=1;Y++)
    {
        hmp float D = distance(hash(I+vec2(X,Y))+vec2(X,Y),F);
        Dist = min(Dist,D);
    }
    return Dist;
	
}


hmp float fbm(hmp vec2 x, hmp float lower, hmp float upper, int octaves) {
    hmp float final = 0.0;
	hmp float a = 0.5;
	for (int i = 0; i < octaves; ++i) {
		final += a * noise(x);
		x *= 2.0;
		a *= 0.5;
        x.y -= TOTAL_REAL_WORLD_TIME * 0.03 * float(i + 1);
	}
	return smoothstep(lower, upper, final);
}

#endif