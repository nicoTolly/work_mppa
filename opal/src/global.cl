/*
 * OPAL (OpenCL Processor Array LBM solver)
 *
 * Copyright (c) 2014, Christian Obrecht <courriel@obrecht.fr>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/*******************************************************************************
 *
 *  global.cl: OpenCL and literal constants, macro definitions, and auxiliary
 *  functions
 *
 */

#if 1
// Velocity parameter
#define k0 (U0/6)

// Relaxation coefficients
#define s1 (1.19F)
#define s2 (1.40F)
#define s4 (1.20F)
#define s9 (2*Re/(6*U0*Lx + Re))
#define s10 (1.40F)
#define s16 (1.98F)

// Collision coefficients
#define c0 (1.F/19)
#define c1 (5.F/399)
#define c2 (11.F/2394)
#define c3 (4.F/1197)
#define c4 (1.F/21)
#define c5 (1.F/63)
#define c6 (1.F/252)
#define c7 (1.F/10)
#define c8 (1.F/40)
#define c9 (1.F/18)
#define c10 (1.F/36)
#define c11 (1.F/72)
#define c12 (1.F/12)
#define c13 (1.F/24)
#define c14 (1.F/4)
#define c15 (1.F/8)
#else 
// Velocity parameter
constant float k0 = U0/6;

// Relaxation coefficients
constant float s1 = 1.19F;
constant float s2 = 1.40F;
constant float s4 = 1.20F;
constant float s9 = 2*Re/(6*U0*Lx + Re);
constant float s10 = 1.40F;
constant float s16 = 1.98F;

// Collision coefficients
constant float c0 = 1.F/19;
constant float c1 = 5.F/399;
constant float c2 = 11.F/2394;
constant float c3 = 4.F/1197;
constant float c4 = 1.F/21;
constant float c5 = 1.F/63;
constant float c6 = 1.F/252;
constant float c7 = 1.F/10;
constant float c8 = 1.F/40;
constant float c9 = 1.F/18;
constant float c10 = 1.F/36;
constant float c11 = 1.F/72;
constant float c12 = 1.F/12;
constant float c13 = 1.F/24;
constant float c14 = 1.F/4;
constant float c15 = 1.F/8;
#endif 

// Solid boundaries encoding
#define G_E01   0x000001
#define G_E02   0x000002
#define G_E03   0x000004
#define G_E04   0x000008
#define G_E05   0x000010
#define G_E06   0x000020
#define G_E07   0x000040
#define G_E08   0x000080
#define G_E09   0x000100
#define G_E10   0x000200
#define G_E11   0x000400
#define G_E12   0x000800
#define G_E13   0x001000
#define G_E14   0x002000
#define G_E15   0x004000
#define G_E16   0x008000
#define G_E17   0x010000
#define G_E18   0x020000

// Walls
#define G_W1    0x001541
#define G_W2    0x002A82
#define G_W3    0x0140C4
#define G_W4    0x028308
#define G_W5    0x00CC10
#define G_W6    0x033020

// Boundary nodes
#define G_BND   0x03FFFF

// Device memory arrays offsets
#define stride(x,y,z) ((x)*sX + (y)*sY + (z)*sZ)
#define offset(x,y,z) ((x)*rX + (y)*rY + (z)*rZ)

// Device memory lattice vectors
#ifdef USE_CLSTENCIL
// #define e00 x,y,z
// #define e01 x+1,y,z
// #define e02 x-1,y,z
// #define e03 x,y+1,z
// #define e04 x,y-1,z
// #define e05 x,y,z+1
// #define e06 x,y,z-1
// #define e07 x+1,y+1,z
// #define e08 x-1,y+1,z
// #define e09 x+1,y-1,z
// #define e10 x-1,y-1,z
// #define e11 x+1,y,z+1
// #define e12 x-1,y,z+1
// #define e13 x+1,y,z-1
// #define e14 x-1,y,z-1
// #define e15 x,y+1,z+1
// #define e16 x,y-1,z+1
// #define e17 x,y+1,z-1
// #define e18 x,y-1,z-1
#define e00 x,y,z
#define e01 x-1,y,z
#define e02 x+1,y,z
#define e03 x,y-1,z
#define e04 x,y+1,z
#define e05 x,y,z-1
#define e06 x,y,z+1
#define e07 x-1,y-1,z
#define e08 x+1,y-1,z
#define e09 x-1,y+1,z
#define e10 x+1,y+1,z
#define e11 x-1,y,z-1
#define e12 x+1,y,z-1
#define e13 x-1,y,z+1
#define e14 x+1,y,z+1
#define e15 x,y-1,z-1
#define e16 x,y+1,z-1
#define e17 x,y-1,z+1
#define e18 x,y+1,z+1
#else
#define e00 stride( 0, 0, 0)
#define e01 stride( 1, 0, 0)
#define e02 stride(-1, 0, 0)
#define e03 stride( 0, 1, 0)
#define e04 stride( 0,-1, 0)
#define e05 stride( 0, 0, 1)
#define e06 stride( 0, 0,-1)
#define e07 stride( 1, 1, 0)
#define e08 stride(-1, 1, 0)
#define e09 stride( 1,-1, 0)
#define e10 stride(-1,-1, 0)
#define e11 stride( 1, 0, 1)
#define e12 stride(-1, 0, 1)
#define e13 stride( 1, 0,-1)
#define e14 stride(-1, 0,-1)
#define e15 stride( 0, 1, 1)
#define e16 stride( 0,-1, 1)
#define e17 stride( 0, 1,-1)
#define e18 stride( 0,-1,-1)
// #define e19 stride( 1, 1, 1)
// #define e20 stride( 1, 1,-1)
// #define e21 stride( 1,-1, 1)
// #define e22 stride( 1,-1,-1)
// #define e23 stride(-1, 1, 1)
// #define e24 stride(-1, 1,-1)
// #define e25 stride(-1,-1, 1)
// #define e26 stride(-1,-1,-1)
#endif // USE_CLSTENCIL

// Access to density distributions
#define src(a,b) Src[(a)*sA + (b)]
#define dst(a)   Dst[(a)*sA]

/*
 *  writePixel: compute RGB values corresponding to velocity variation u and
 *  write them to Bitmap array (u is supposed to range from 0 to 4).
 */
void writePixel(global uchar* Bitmap, int x, int z, float u)
{
    int h;
    uchar s, r, g, b;

    Bitmap += 3*(x + z*Lx);
    h = (int)u % 6;
    s = (uchar)(255.F*(u - h));
    #if 0
    switch(h) {
        case 0: r = 255; g = s; b = 0; break;
        case 1: r = 255 - s; g = 255; b = 0; break;
        case 2: r = 0; g = 255; b = s; break;
        case 3: r = 0; g = 255 - s; b = 255; break;
        case 4: r = s; g = 0; b = 255; break;
        case 5: r = 255; g = 0; b = 255 - s; break;
        default: r = 0; g = 0; b = 0;
    }
    #else
    if (h == 0) { r = 255; g = s; b = 0; }
    else if (h == 1) { r = 255 - s; g = 255; b = 0; }
    else if (h == 2) { r = 0; g = 255; b = s; }
    else if (h == 3) { r = 0; g = 255 - s; b = 255; }
    else if (h == 4) { r = s; g = 0; b = 255; }
    else if (h == 5) { r = 255; g = 0; b = 255 - s; }
    else { r = 0; g = 0; b = 0; }
    #endif
    Bitmap[0] = r;
    Bitmap[1] = g;
    Bitmap[2] = b;
}
