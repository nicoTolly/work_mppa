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
	isl_space * space = isl_schedule_node_band_get_space(node);

	// get partial schedules and fuse them
	isl_multi_union_pw_aff * mu = isl_schedule_node_band_get_partial_schedule(node);
	isl_multi_union_pw_aff * pmu = isl_schedule_node_band_get_partial_schedule(parent);

	//isl_union_pw_aff_list * mupw_list = isl_union_pw_aff_list_from_union_pw_aff(
	//		mu);
	//mupw_list = isl_multi_union_pw_aff_list_add(mupw_list, pmu);
	//isl_multi_union_pw_aff * mupw_result = isl_multi_union_pw_aff_from_union_pw_aff_list(space, mupw_list);


	//isl_schedule_node * new_node = isl_schedule_insert_partial_schedule(parent, mupw_result);

	//isl_schedule_node * child = isl_schedule_node_delete(node);
	return node;


}


isl_schedule_node * schedule_node_fuse_bands(isl_schedule_node * node, void * user)
{
	enum isl_schedule_node_type ntype = isl_schedule_node_get_type( node);
	switch(ntype){
	case isl_schedule_node_band:
		return band_fuse(node);
	default:
		return node;
	}
}
