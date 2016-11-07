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
 *  types.h: container types definitions.
 *
 */

typedef struct timespec timespec;

/*
 *  config: container for configuration parameters.
 */

typedef struct {
    const char* Path;    // Path to output files
    const char* Prefix;  // Prefix of output files
    bool Log;            // Computation log flag
    bool Images;         // Graphic monitoring flag
    bool Export;         // Result exportation flag
    unsigned int Lx;              // Domain dimensions (in nodes)
    unsigned int Ly;
    unsigned int Lz;
    unsigned int Nx;              // Workgroup dimensions (in nodes)
    unsigned int Ny;
    unsigned int Nz;
    unsigned int Duration;        // Simulation duration (in time steps)
    unsigned int Period;          // Monitoring period
    float Re;            // Reynolds number
    float U0;            // Imposed velocity
    float epsilon;       // Convergence threshold
} config;

/*
 *  data: memory arrays for computation data.
 */

typedef struct {
    size_t DomainSize;   // Number of lattice nodes
    size_t BitmapSize;   // Number of bytes for monitoring bitmap
    // float* LatticeEven;  // Source lattice array for even time steps
    // float* LatticeOdd;   // Source lattice array for odd time steps
} data;

/*
 *  results: host memory arrays for computation results.
 */

typedef struct {
    int Lx;               // Domain dimensions (in nodes)
    int Ly;
    int Lz;
    size_t DomainSize;    // Number of lattice nodes
    size_t BitmapSize;    // Number of bytes for monitoring bitmap
    // void* Density;        // Fluid density
    // void* Velocity;       // Fluid velocity
    // void* Delta;          // Squared velocity variations
    float L2;             // L2 norm of velocity variations
    float epsilon;        // Convergence threshold
    bool Converged;       // Convergence flag
    void* Bitmap;         // Monitoring bitmap array
    unsigned int Header[13];  // Monitoring bitmap BMP header
    bool Log;             // Computation log flag
    bool Export;          // Result exportation flag
    FILE* LogFile;        // Log file handle
    FILE* ExportFile;     // Export file handle
} results;

