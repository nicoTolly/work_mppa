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
 *  geometry.cl: test case geometry.
 *
 */

/*
 * getCoordinates: retrieve global coordinates.
 */ 

int getCoordinate(char c)
{
#if 1
    c -= 'x';
    return get_global_offset(c) + get_local_id(c) + get_local_size(c)*get_group_id(c);
#else
    return get_global_id((int)(c-'x'));
#endif
}

/*
 *  getGeometry: encode local geometry informations in a bitfield.
 *  NOTE: In case of more complex geometries, the bitfields might be created
 *  during initialisation and stored in device memory.
 */

int getGeometry(int x, int y, int z)
{
    int g = 0;

    if (x == Lx-1) g |= G_W1;
    if (x == 0)    g |= G_W2;
    if (y == Ly-1) g |= G_W3;
    if (y == 0)    g |= G_W4;
    if (z == Lz-1) g |= G_W5;
    if (z == 0)    g |= G_W6;

    return g;
}
