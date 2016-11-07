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
 *  export.c: results exportation functions.
 *
 */

#include "opal.h"

/*
 *  exportImage: write monitoring image file.
 *  NOTE: The image width must be a multiple of 4.
 */

void exportImage(config* Config, results* Results, int Time)
{
    char* ImageName;
    FILE* ImageFile;

    // Create image file name using 'Path' and 'Prefix' configuration parameters
    // and current time step.
    asprintf(&ImageName, "%s/%s%07d.bmp", Config->Path, Config->Prefix, Time);
    ImageFile = fopen(ImageName, "w");
    if (ImageFile == NULL)
            eprintf("Failed to open image file '%s'.\n", ImageName);
    // Write BMP header
    fputs("BM", ImageFile);
    fwrite(Results->Header, sizeof(unsigned int), 13, ImageFile);
    // Write bitmap
    fwrite(Results->Bitmap, sizeof(unsigned char), Results->BitmapSize, ImageFile);
    fclose(ImageFile);
    free(ImageName);
}

/*
 *  writeB64: encode memory chunk in B64 format and write to given file.
 */

const char B64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                 "0123456789+/";

void writeB64(unsigned char* B, int Size, FILE* File)
{
    int i;
    int r = Size%3;

    for (i = 0; i < Size - r; i += 3)
        fprintf(File, "%c%c%c%c",
                B64[B[i]>>2],
                B64[((B[i] & 003)<<4) | (B[i+1]>>4)],
                B64[((B[i+1] & 017)<<2) | (B[i+2]>>6)],
                B64[B[i+2] & 077]);

    if (r == 1)
        fprintf(File, "%c%c==",
                B64[B[i]>>2],
                B64[(B[i] & 003)<<4]);

    if (r == 2)
        fprintf(File, "%c%c%c=",
                B64[B[i]>>2],
                B64[((B[i] & 003)<<4) | (B[i+1]>>4)],
                B64[(B[i+1] & 017)<<2]);
}

/*
 *  exportResults: export results in VTK format.
 */

void exportResults(results* Results, 
    float Density[Results->Lz][Results->Ly][Results->Lx],
    float Velocity[Results->Lz][Results->Ly][Results->Lx][D])
{
    FILE* File = Results->ExportFile;
    int Length;

    // Preamble
    fprintf(File,
        "<?xml version='1.0'?>\n"
        "<VTKFile type='ImageData'>\n"
        "<ImageData WholeExtent='0 %1$d 0 %2$d 0 %3$d' Origin='0 0 0'>\n"
        "<Piece Extent='0 %1$d 0 %2$d 0 %3$d'>\n",
        Results->Lx - 1, Results->Ly - 1, Results->Lz - 1);
    
    // Density
    fprintf(File,
        "<PointData Vectors='Velocity'>\n"
        "<DataArray type='Float32' Name='Density' format='binary' "
            "encoding='base64' NumberOfComponents='1'>\n");
    Length = Results->DomainSize*sizeof(float);
    writeB64((unsigned char*)&Length, 4, File);
    writeB64((unsigned char*)Density, Length, File);
    fprintf(File, "\n</DataArray>\n");

    // Velocity
    fprintf(File,
        "<DataArray type='Float32' Name='Velocity' format='binary' "
            "encoding='base64' NumberOfComponents='3'>\n");
    Length = D*Results->DomainSize*sizeof(float);
    writeB64((unsigned char*)&Length, 4, File);
    writeB64((unsigned char*)Velocity, Length, File);
    fprintf(File, "\n</DataArray>\n");

    // Closing
    fprintf(File,
        "</PointData>\n"
        "</Piece>\n"
        "</ImageData>\n"
        "</VTKFile>\n");
}
