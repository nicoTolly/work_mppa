#include "schedule_visitor.h"


/* Tile "band" with tile size specified by "sizes".
 *
 * Since the tile loops will be mapped to block ids, we forcibly
 * turn off tile loop scaling.  We may want to enable tile loop scaling
 * at some later point, but then we would have to support the detection
 * of strides during the mapping to block ids.
 * Similarly, since the point loops will be mapped to thread ids,
 * we forcibly shift the point loops so that they start at zero.
 */
static __isl_give isl_schedule_node *tile_band(
	__isl_take isl_schedule_node *node, __isl_take isl_multi_val *sizes)
{
	isl_ctx *ctx = isl_schedule_node_get_ctx(node);
	int scale_tile;
	int shift_point;

	scale_tile = isl_options_get_tile_scale_tile_loops(ctx);
	isl_options_set_tile_scale_tile_loops(ctx, 1);
	shift_point = isl_options_get_tile_shift_point_loops(ctx);
	isl_options_set_tile_shift_point_loops(ctx, 1);

	node = isl_schedule_node_band_tile(node, sizes);

	isl_options_set_tile_scale_tile_loops(ctx, scale_tile);
	isl_options_set_tile_shift_point_loops(ctx, shift_point);

	return node;
}


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


//callback for map_descendant_bottom_up
// Call node_band_tile each time a band_node 
// is visited
isl_schedule_node * node_tiler(isl_schedule_node * node, void * user)
{
	enum isl_schedule_node_type ntype = isl_schedule_node_get_type( node);
	print_type(node);
	isl_ctx * ctx = isl_schedule_node_get_ctx(node);
	// value to be returned
	isl_schedule_node * return_node = NULL;

	switch(ntype){
	case isl_schedule_node_band:
		
		if ( isl_schedule_node_has_children(node) == isl_bool_true)
		{
			isl_schedule_node * tiled_node;
			isl_val * val = isl_val_int_from_si(ctx, 32 );
			isl_space *space = isl_schedule_node_band_get_space(node);
			// This is the only way I found to initialize correctly
			// a multi_val
			isl_multi_val * tile_sizes = isl_multi_val_zero(space);
			tile_sizes = isl_multi_val_add_val(tile_sizes, val);


			//tiled_node = isl_schedule_node_band_tile(node, tile_sizes);
			//tiled_node = isl_schedule_node_band_split(node, 1);
			tiled_node = tile_band(node, tile_sizes);
			return_node = tiled_node;
			
		}
		
	default:
		return_node = node;
	break;
	}
	return return_node;
}




// Take a node as argument and visit
// each descendant manually (probably a bad idea,
// better use some pre-defined functions)
// This function segfaults at this point, do not use it
__isl_give isl_schedule_node* schedule_tree_visitor(isl_ctx *ctx,  __isl_take isl_schedule_node * node  )
{
	enum isl_schedule_node_type ntype = isl_schedule_node_get_type( node);
	print_type(node);
	if (node == NULL)
	{
		printf("Node is NULL\n");
		return NULL;
	}
	isl_schedule_node * return_node  = NULL;
	switch(ntype)
	{
case isl_schedule_node_filter :
	if ( isl_schedule_node_has_children(node))
	{
		printf("node has children\n");
		//isl_schedule_node * pnode = isl_schedule_node_copy(node);
		//isl_schedule_node * cnode = isl_schedule_node_first_child(node);
		isl_schedule_node * cnode = isl_schedule_node_get_child(node, 0);
		//node = pnode;
		isl_id *mark = isl_id_alloc(ctx, "A mark", NULL);
		isl_schedule_node * new_node = isl_schedule_node_insert_mark( cnode, mark);
		//wrap_isl_printer(ctx, (void * ) new_node, SCHEDULE_NODE);
		return_node =  schedule_tree_visitor(ctx, new_node);

	}
	break;
case isl_schedule_node_band :
	if ( isl_schedule_node_has_children(node) == isl_bool_true)
	{
		printf("node has children\n");
		isl_multi_val *tile_sizes = NULL;
		tile_sizes = isl_multi_val_add_val( tile_sizes, isl_val_int_from_si(ctx, 2 ));
		isl_schedule_node * new_node = isl_schedule_node_band_tile(node, tile_sizes);
		return_node =  schedule_tree_visitor(ctx,new_node);

	}
case isl_schedule_node_context :
case isl_schedule_node_domain :
	if ( isl_schedule_node_has_children(node) == isl_bool_true)
	{
		printf("node has children\n");
		//isl_schedule_node * pnode = isl_schedule_node_copy(node);
		//isl_schedule_node * cnode = isl_schedule_node_first_child(node);
		isl_schedule_node * cnode = isl_schedule_node_get_child(node, 0);
		//wrap_isl_printer(ctx, (void * ) cnode, SCHEDULE_NODE);
		//node = pnode;
		return_node = (cnode != NULL)? schedule_tree_visitor(ctx,  cnode):NULL;

	}
	break;
case isl_schedule_node_leaf :
	return_node = node;
case isl_schedule_node_sequence : 
	// apparently, you just can't declare a variable after a label
	;
	//get the number of children of this node 
	int nchild;
	nchild = isl_schedule_node_n_children(node);
	isl_schedule_node ** children = (isl_schedule_node **) malloc (sizeof( intptr_t) * nchild);
	for (int i =0; i < nchild; i++)
	{
		//copying the parent node and getting children
		isl_schedule_node * pnode = isl_schedule_node_copy(node);
		//warning : This frees the node pointer !!!!
		children[i] = isl_schedule_node_child(node, i);
		children[i] = schedule_tree_visitor(ctx,  children[i]);
		printf("print node in child\n");
		//wrap_isl_printer(ctx, (void * ) children[i], SCHEDULE_NODE);
		node = pnode;
	}
	return_node = node;
	break;
default :
	break;
	}
	if (return_node == NULL)
		return NULL;
	else
		return return_node;
}



//A function for visiting a schedule tree, maybe modifying it
int schedule_visitor( __isl_keep isl_schedule * schedule, void * (*fn) (void *))
{
	isl_schedule_node * root = isl_schedule_get_root(schedule);
	isl_ctx * ctx = isl_schedule_get_ctx(schedule);
	wrap_isl_printer(ctx,  (void * ) root, SCHEDULE_NODE);
	int nband = count_band_nodes(root);
	printf("number of band in original schedule :%d\n", nband);
	//isl_schedule_node_free(root);
	//isl_schedule_node * new_root = schedule_tree_visitor(ctx,root);
	//if (new_root == NULL)
	//{
	//	printf("schedule tree visitor failed \n");
	//	return -1;
	//}
	//wrap_isl_printer(ctx, (void * ) new_root, SCHEDULE_NODE);
	//isl_schedule_node_foreach_descendant_top_down is NOT supposed to change the state of the schedule node
	//It is better to use map_descendant_bottom_up which seems better suited
	//for that
	isl_schedule_node *new_root = isl_schedule_node_map_descendant_bottom_up(root, node_tiler,  NULL );
	if (new_root == NULL)
	{
		printf("schedule tree visitor failed \n");
		return -1;
	}
	printf("\n\nNew schedule \n\n");
	wrap_isl_printer(ctx, (void * ) new_root, SCHEDULE_NODE);
	//
	////printf("number of band in new schedule  :");
	printf("new number of band nodes : %d\n", count_band_nodes(new_root));
	isl_schedule_node_free(new_root);
	return 0;
}
