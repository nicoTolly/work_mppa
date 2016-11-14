/*
 * Copyright (C) 2013 Kalray SA. All rights reserved.
 * This code is Kalray proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of Kalray SA.
 */
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
extern void compute_async(
	config* Config,
	off64_t Src,
	off64_t Dst,
	unsigned int step
	);

// monitor kernel
extern void monitor(
	config* Config, results* Results,
	off64_t Src,
	off64_t Velocity,
	off64_t Delta,
	unsigned int step
	);

bsp_barrier_t barrier_0;

off64_t LatticeEven = 0;
off64_t LatticeOdd = 0;
off64_t Velocity = 0;
off64_t Bitmap = 0;
#ifdef DUMP_DENSITY
off64_t Density = 0;
#endif // DUMP_DENSITY
#ifdef DUMP_DELTA
off64_t Delta = 0;
#endif // DUMP_DELTA

#ifdef TIME_PROFILING
extern uint64_t compute_time;
extern uint64_t monitor_time;
extern uint64_t wait_time;
extern uint64_t get_time;
extern uint64_t put_time;
#endif // TIME_PROFILING

/* Main executed on PE0 */
int main(void)
{
	config Config;
	data Data;
	results Results;
	uint64_t start_time  = 0;
	uint64_t end_time = 0;

	mppa_rpc_client_init();
	mppa_async_init();
	mppa_remote_client_init();

	bsp_barrier_init(&barrier_0, BSP_BARRIER_USER);

	int cid = __k1_get_cluster_id();
	if(cid==0) printf("# MPPA_RPC_CLUSTER_MAX_REQUEST = %d\n\n", MPPA_RPC_CLUSTER_MAX_REQUEST);

	// Create containers
	// Config = getConfig(argc, argv);
	Config = getConfig();
	Data = createData(&Config);
	Results = createResults(&Config);

	// Cluster 0 allocate memory, then send to others
	if(cid == 0){
		// Malloc
		mppa_async_malloc(MPPA_ASYNC_DDR_0,
			Results.DomainSize*Q*sizeof(float), &LatticeEven, NULL);
		if(LatticeEven == 0) {
			printf("Could not allocate 'LatticeEven' array.\n"); return -1;
		}

		mppa_async_malloc(
#ifdef DUPLEX
			MPPA_ASYNC_DDR_1,
#else
			MPPA_ASYNC_DDR_0,
#endif
			Results.DomainSize*Q*sizeof(float), &LatticeOdd, NULL);
		if(LatticeOdd == 0) {
			printf("Could not allocate 'LatticeOdd' array.\n"); return -1;
		}

#ifdef DUMP_DENSITY
		mppa_async_malloc(MPPA_ASYNC_DDR_0,
			Results.DomainSize*sizeof(float), &Density, NULL);
		if(Density == 0) {
			printf("Could not allocate 'Density' array.\n"); return -1;
		}
#endif // DUMP_DENSITY

		mppa_async_malloc(MPPA_ASYNC_DDR_0,
			Results.DomainSize*D*sizeof(float), &Velocity, NULL);
		if(Velocity == 0) {
			printf("Could not allocate 'Velocity' array.\n"); return -1;
		}

#ifdef DUMP_DELTA
		mppa_async_malloc(MPPA_ASYNC_DDR_0,
			Results.DomainSize*sizeof(float), &Delta, NULL);
		if(Delta == 0) {
			printf("Could not allocate 'Delta' array.\n"); return -1;
		}
#endif // DUMP_DELTA

		mppa_async_malloc(MPPA_ASYNC_DDR_0,
			Results.BitmapSize*sizeof(unsigned char), &Bitmap, NULL);
		if(Bitmap == 0) {
			printf("Could not allocate 'Bitmap' array.\n"); return -1;
		}

		// Begin log
		printBanner(&Config, &Results);

		// Print result at step 0
		printResults(&Results, 0);
	}

	////////////////////////////////////////////////////////////////////////
	// INIT
	////////////////////////////////////////////////////////////////////////
	init(&Config, &Results);

	// Start timer
	start_time = __k1_read_dsu_timestamp();

	//////////////////////////////////////////////////////////////////////
	// COMPUTE : Loop on Duration
	//////////////////////////////////////////////////////////////////////
	off64_t Src = LatticeEven;
	off64_t Dst = LatticeOdd;
	for(unsigned int step = 0; step <= Config.Duration && !(Results.Converged);
																							step++)
	{
		// Swap Src | Dst pointers mapping on LatticeEven | LatticeOdd
		// based on step's parity
		if(step & 1) {Src = LatticeOdd ; Dst = LatticeEven;}
		else         {Src = LatticeEven; Dst = LatticeOdd ;}

		// compute
		// compute(&Config, Src, Dst);
		compute_async(&Config, Src, Dst, step);

		// Monitoring: Do every 'Period' steps
		if (step > 0 && (step % Config.Period == 0)) {
			#ifdef DUMP_DELTA
			monitor(&Config, &Results, Src, Velocity, Delta, step);
			#else
			monitor(&Config, &Results, Src, Velocity, 0, step);
			#endif // DUMP_DELTA

			// print progress and exports image
			if(cid == 0) {printResults(&Results, step);}
			// if(Config->Images) exportImage(Config, Results, step);
		}
	} // end for step

	// Optional simulation results exportation at the end
	// if (Config.Export) exportResults(Results, *Density, *Velocity);

	// Stop timer
	end_time = __k1_read_dsu_timestamp();

	// printSummary(Results, Config.Duration, &Start, &Stop);
	if(cid == 0) {printSummary(&Results, Config.Duration, start_time, end_time);}
	
	mppa_rpc_barrier_all();

	#ifdef TIME_PROFILING
	#define CHIP_FREQ (__bsp_frequency/1000)
	printf("Cluster %2d compute %6.1f ms monitor %6.1f ms wait %8.1f ms get %6.1f ms put %6.1f ms\n",
			cid
			, (float)(((float) compute_time) / CHIP_FREQ)
			, (float)(((float) monitor_time) / CHIP_FREQ)
			, (float)(((float) wait_time) / CHIP_FREQ)
			, (float)(((float) get_time) / CHIP_FREQ)
			, (float)(((float) put_time) / CHIP_FREQ)
			);
	#endif // TIME_PROFILING

	// /* output file */
	// char output_file[60];
	// sprintf(output_file, "result_opal_dsm_%d_%d_%d_%d_%d.txt",
	// 	CAVITY, NX, NY, NZ, DURATION);
	// // printf("# [Cluster %d] write output in file: %s\n", cid, output_file);

	// int fd = open(output_file, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
	// write(fd, str, sizeof(*str)*1024);
	// close(fd);

	////////////////////////////////////////////////////////////////////////
	// Clean up
	////////////////////////////////////////////////////////////////////////
	clearData(&Data);
	clearResults(&Results);
	// free(Config);

	if(cid == 0){
		mppa_async_free(MPPA_ASYNC_DDR_0, LatticeEven, NULL);
		#ifdef DUPLEX
		mppa_async_free(MPPA_ASYNC_DDR_1, LatticeOdd, NULL);
		#else
		mppa_async_free(MPPA_ASYNC_DDR_0, LatticeOdd, NULL);
		#endif
		#ifdef DUMP_DENSITY
		mppa_async_free(MPPA_ASYNC_DDR_0, Density, NULL);
		#endif // DUMP_DENSITY
		mppa_async_free(MPPA_ASYNC_DDR_0, Velocity, NULL);
		#ifdef DUMP_DELTA
		mppa_async_free(MPPA_ASYNC_DDR_0, Delta, NULL);
		#endif // DUMP_DELTA
		mppa_async_free(MPPA_ASYNC_DDR_0, Bitmap, NULL);
	}

	mppa_rpc_barrier_all();
	mppa_async_final();

	return 0;
}
