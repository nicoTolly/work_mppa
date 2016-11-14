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
 *  utils.c: miscellaneous utility functions.
 *
 */

#include "opal.h"

/*
 *  eprintf: print error message and exit.
 */

void eprintf(const char* fmt, ...)
{
    printf("[OPAL] ");
    printf(fmt);
    printf("Exiting...\n");
    exit(EXIT_FAILURE);
}

/*
 *  exitOnFail: check Status variable and exit on error.
 */

// void exitOnFail(cl_int Status, const char* Message)
// {
//     if (Status != CL_SUCCESS) { 
//         fprintf(stderr, "[OPAL] %s", Message);
//         fprintf(stderr, "Exiting...\n");
//         exit(EXIT_FAILURE);
//     }
// }

/*
 *  elapsedTime: compute elapsed time in seconds from two timespec structures.
 */

// double elapsedTime(timespec* Start, timespec* Stop)
// {
//     return (double)(Stop->tv_sec - Start->tv_sec) 
//            + (double)(Stop->tv_nsec - Start->tv_nsec)*1E-9;
// }
double elapsedTime(uint64_t Start, uint64_t Stop)
{
    // return (((double) (Stop - Start))/ (double) CHIP_FREQ) *1E-6;
    return (((double) (Stop - Start))/ (double) __bsp_frequency);
}


// /*
//  *  readFile: append file to Buffer array and update Length counter.
//  *  NOTE: prior to first call, Buffer must be set to NULL and Length to 0.
//  */

// void readFile(const char* Name, char** Buffer, size_t* Length)
// {
//     FILE* File;
//     size_t FileSize;

//     File = fopen(Name, "rb");
//     if (File == NULL) eprintf("Could not open file '%s'.\n", Name);
//     fseek(File, 0, SEEK_END);
//     FileSize = ftell(File);
//     rewind(File);
//     *Buffer = realloc(*Buffer, (*Length + FileSize + 1)*sizeof(char));
//     if (*Buffer == NULL) eprintf("Could not allocate file buffer.\n");
//     *Length += fread(*Buffer + *Length, sizeof(char), FileSize, File);
//     (*Buffer)[*Length] = '\0';
//     fclose(File);
// }

/*
 *  sumArray: add up an array of floating point numbers.
 */

float sumArray(float* Array, size_t Size)
{
    int i;
    float Sum = 0;

    for (i = 0; i < Size; i++)
        Sum += *(Array++);

    return Sum;
}
