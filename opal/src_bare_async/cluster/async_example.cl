
#define NB_SLICES_PER_WORKGROUP 100
#define SLICE_LENGTH_PER_WORKGROUP 1024
#define SLICE_LENGTH_PER_WORKITEM 32
#define NB_ACTIVE_WORKITEMS (SLICE_LENGTH_PER_WORKGROUP/SLICE_LENGTH_PER_WORKITEM)
#define NB_PREFETCH_SLICES 2

#define ALPHA 10.2f

/** 
 * This is an example of vector-scale using OpenCL asynchronous copy 
 * c = ALPHA * c
 * Each WG does 100 slices in streaming, prefetch slice n+1 while doing slice n
 * We assume get_local_size(0) >= NB_ACTIVE_WORKITEMS
 * 
 * Note that async_workgroup_copy() and wait_group_events() are WG-collective functions, 
 *   so they must be called by all work-items
 */
__kernel void compute_async(__global float* c)
{
	int gid = get_group_id(0);
	int wid = get_local_id(0);

	// async events
	event_t events[NB_PREFETCH_SLICES];

	// begin work-space of current WG
	__global float* my_c = c + gid*NB_SLICES_PER_WORKGROUP*SLICE_LENGTH_PER_WORKGROUP;

	// local buffer for prefetching : double-buffering
	__local float local_buffer[NB_PREFETCH_SLICES][SLICE_LENGTH_PER_WORKGROUP];

	/////////////////////////////////////////////////////////////////////////////
	// PROLOGUE : prefetch first slice
	/////////////////////////////////////////////////////////////////////////////
	events[0] = async_workgroup_copy(                    /* called by all WI */
			&local_buffer[0][0],                           /* local_dst */
			my_c,                                          /* global_src */
			SLICE_LENGTH_PER_WORKGROUP*sizeof(float),      /* size */
			NULL);

	/////////////////////////////////////////////////////////////////////////////
	// PIPELINE
	/////////////////////////////////////////////////////////////////////////////
	for(int slice = 0; slice < NB_SLICES_PER_WORKGROUP; slice++){

		// index of slice in local buffer
		int i_slice = slice % NB_PREFETCH_SLICES;
		int i_next_slice = (slice+1) % NB_PREFETCH_SLICES;

		// prefetch next slice (non-blocking)
		events[i_next_slice] = async_workgroup_copy(
				&local_buffer[i_next_slice][0],                 /* local_dst */
				my_c + (slice+1)*SLICE_LENGTH_PER_WORKGROUP,    /* global_src */
				SLICE_LENGTH_PER_WORKGROUP*sizeof(float),       /* size */
				NULL);

		// wait for current slice
		wait_group_events(1, &events[i_slice]);

		// compute current slice
		if(wid < NB_ACTIVE_WORKITEMS){
			int wi_begin = wid * SLICE_LENGTH_PER_WORKITEM;
			int wi_end   = (wid+1) * SLICE_LENGTH_PER_WORKITEM;
			for(int i = wi_begin; i+4 < wi_end; i+=4){
				local_buffer[i_slice][i  ] *= ALPHA;
				local_buffer[i_slice][i+1] *= ALPHA;
				local_buffer[i_slice][i+2] *= ALPHA;
				local_buffer[i_slice][i+3] *= ALPHA;
			}
		}

		// gather update from all WI of the WG
		barrier(CLK_LOCAL_MEM_FENCE);

		// put result back to global
		events[i_slice] = async_workgroup_copy(
				my_c + slice*SLICE_LENGTH_PER_WORKGROUP,        /* global_dst */
				&local_buffer[i_slice][0],                      /* local_src */
				SLICE_LENGTH_PER_WORKGROUP*sizeof(float),       /* size */
				NULL);
	}

	/////////////////////////////////////////////////////////////////////////////
	// EPILOGUE : wait on last put
	/////////////////////////////////////////////////////////////////////////////
	wait_group_events(1, &events[(NB_SLICES_PER_WORKGROUP-1) % NB_PREFETCH_SLICES]);
}
