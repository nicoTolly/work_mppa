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
 *  main.c: main program function.
 *
 */

#include "opal.h"

// init kernel
extern void init(config* Config, results* Results);

// compute kernel
extern void compute(
	config* Config, results* Results, unsigned int step,
	float Src[Config->Lz][Config->Ly][Config->Lx][Q], 
	float Dst[Config->Lz][Config->Ly][Config->Lx][Q],
	float Density[Config->Lz][Config->Ly][Config->Lx],
	float Velocity[Config->Lz][Config->Ly][Config->Lx][D],
	float Delta[Config->Lz][Config->Ly][Config->Lx]
	);

int main(int argc, char **argv)
{
	config* Config;
	data* Data;
	results* Results;
	timespec Start, Stop;

	// Start timer
	clock_gettime(CLOCK_REALTIME, &Start);
	
	// Create containers
	Config = getConfig(argc, argv);
	Data = createData(Config);
	Results = createResults(Config);

	// Source and Destination lattice array for time steps : [Lz][Ly][Lx][Q]
	float (*LatticeEven)[Config->Lz][Config->Ly][Config->Lx][Q];
	float (*LatticeOdd)[Config->Lz][Config->Ly][Config->Lx][Q];

	// Density[Lz][Ly][Lx], Velocity[Lz][Ly][Lx][D] and Delta[Lz][Ly][Lx]
	float (*Density)[Config->Lz][Config->Ly][Config->Lx];
	float (*Velocity)[Config->Lz][Config->Ly][Config->Lx][D];
	float (*Delta)[Config->Lz][Config->Ly][Config->Lx];

	// Malloc
	LatticeEven = (float(*)[Config->Lz][Config->Ly][Config->Lx][Q])
			malloc(Results->DomainSize*Q*sizeof(float));
	if(LatticeEven == NULL) {
		printf("Could not allocate 'LatticeEven' array.\n"); return -1;
	}

	LatticeOdd = (float(*)[Config->Lz][Config->Ly][Config->Lx][Q]) 
			malloc(Results->DomainSize*Q*sizeof(float));
	if(LatticeOdd == NULL) {
		printf("Could not allocate 'LatticeOdd' array.\n"); return -1;
	}

	Density = (float(*)[Config->Lz][Config->Ly][Config->Lx])
			malloc(Results->DomainSize*sizeof(float));
	if(Density == NULL) {
		printf("Could not allocate 'Density' array.\n"); return -1;
	}

	Velocity = (float(*)[Config->Lz][Config->Ly][Config->Lx][D])
			malloc(Results->DomainSize*D*sizeof(float));
	if(Velocity == NULL) {
		printf("Could not allocate 'Velocity' array.\n"); return -1;
	}

	Delta = (float(*)[Config->Lz][Config->Ly][Config->Lx])
			malloc(Results->DomainSize*sizeof(float));
	if(Delta == NULL) {
		printf("Could not allocate 'Delta' array.\n"); return -1;
	}

	// Begin log
	printBanner(Config, Results);

	// Print result at step 0 
	printResults(Results, 0);

	////////////////////////////////////////////////////////////////////////
	// INIT
	////////////////////////////////////////////////////////////////////////
	init(Config, Results);

	////////////////////////////////////////////////////////////////////////
	// COMPUTE : Loop on Duration
	////////////////////////////////////////////////////////////////////////	
	for(unsigned int step = 0; step <= Config->Duration && !(Results->Converged); 
																							step++)
	{	
		// Swap Src | Dst pointers mapping on LatticeEven | LatticeOdd 
		// based on step's parity
		if(step & 1) {compute(Config, Results, step, *LatticeOdd, *LatticeEven, 
									*Density, *Velocity, *Delta);}
		else         {compute(Config, Results, step, *LatticeEven, *LatticeOdd, 
									*Density, *Velocity, *Delta);}

	} // end for step

	// Optional simulation results exportation at the end
	if (Config->Export) exportResults(Results, *Density, *Velocity);

	////////////////////////////////////////////////////////////////////////
	// Clean up
	////////////////////////////////////////////////////////////////////////	
	free((void*)LatticeEven);
	free((void*)LatticeOdd);
	free((void*)Density);
	free((void*)Velocity);
	free((void*)Delta);
	clearData(Data);
	free(Config);

	// Stop timer
	clock_gettime(CLOCK_REALTIME, &Stop);

	// Finalise log
	printSummary(Results, Config->Duration, &Start, &Stop);
	clearResults(Results);

	return EXIT_SUCCESS;
}
