// __multiversion__
// This signals the loading code to prepend either #version 100 or #version 300 es as apropriate.

#include "vertexVersionCentroidUV.h"

#include "uniformWorldConstants.h"

attribute POS4 POSITION;

varying vec3 pos;

void main()
{
    gl_Position = WORLDVIEWPROJ * POSITION;

    pos = POSITION.xyz;

}