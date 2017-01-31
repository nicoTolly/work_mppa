#include "band_fuse.h"

// Attempt to fuse two successive band nodes in one
// Two band nodes can apply a schedule on two 
// different dimensions, but then it becomes difficult
// to tile them reasonably
__isl_give isl_schedule_node * band_fuse(__isl_take isl_schedule_node * node)
{
	if (isl_schedule_node_get_type( node) != isl_schedule_node_band)
	{
		printf("error, not a band node\n");
		exit(-1);
	}
	isl_schedule_node * parent = isl_schedule_node_parent(node);
	//verify if the child is a band node
	if (isl_schedule_node_get_type( parent) != isl_schedule_node_band)
	{
		return node;
	}
	isl_ctx * ctx = isl_schedule_node_get_ctx(node);
	isl_space * space = isl_schedule_node_band_get_space(node);

	isl_printer * print = isl_printer_to_file(ctx, stdin);

	// get partial schedules and fuse them
	isl_multi_union_pw_aff * mu = isl_schedule_node_band_get_partial_schedule(node);
	isl_multi_union_pw_aff * pmu = isl_schedule_node_band_get_partial_schedule(parent);

	isl_multi_union_pw_aff * fusion = isl_multi_union_pw_aff_range_product(mu, pmu);
	wrap_isl_printer( ctx, (void *) fusion, MULTI_UNION_PW_AFF);


	isl_schedule_node * fused_node = isl_schedule_node_insert_partial_schedule(parent, fusion);

	isl_printer_free(print);
	return fused_node;


}


