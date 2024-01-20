#version 460 core
out vec4 FragColor;

uint hash( uint x ) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}

float random( float f ) {
    const uint mantissaMask = 0x007FFFFFu;
    const uint one          = 0x3F800000u;

    uint h = hash( floatBitsToUint( f ) );
    h &= mantissaMask;
    h |= one;

    float  r2 = uintBitsToFloat( h );
    return r2 - 1.0;
}

void main()
{
    float r = random(gl_PrimitiveID);
    float r2 = random(gl_PrimitiveID+1);
    float r3 = random(gl_PrimitiveID+2);
    FragColor = vec4(r, r2, r3, 1.0f);
}

