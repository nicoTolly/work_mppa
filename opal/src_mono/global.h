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

// Velocity parameter
#define k0 (Config->U0/6)

// Relaxation coefficients
#define s1 (1.19F)
#define s2 (1.40F)
#define s4 (1.20F)
#define s9 (2*Config->Re/(6*Config->U0*Config->Lx + Config->Re))
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
