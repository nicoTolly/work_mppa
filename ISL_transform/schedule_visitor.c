#include "schedule_visitor.h"


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



//callback for map_descendant_bottom_up
// Call node_band_tile each time a band_node 
// is visited
// Returns a node at the same position in the tree
// as the node taken as input.
// That way, the number of reference is still the same
isl_schedule_node * node_tiler(__isl_take isl_schedule_node * node, void * user)
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
		isl_schedule_node * cnode = isl_schedule_node_get_child(node, 0);
		isl_id *mark = isl_id_alloc(ctx, "A mark", NULL);
		isl_schedule_node * new_node = isl_schedule_node_insert_mark( cnode, mark);
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
		isl_schedule_node * cnode = isl_schedule_node_get_child(node, 0);
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
// invoked in main
int schedule_visitor( __isl_keep isl_schedule * schedule, void * (*fn) (void *))
{
	isl_schedule_node * root = isl_schedule_get_root(schedule);
	isl_ctx * ctx = isl_schedule_get_ctx(schedule);
	wrap_isl_printer(ctx,  (void * ) root, SCHEDULE_NODE);
	int nband = count_band_nodes(root);
	printf("number of band in original schedule :%d\n", nband);
	isl_schedule_node * fused_bands = isl_schedule_node_map_descendant_bottom_up(root, schedule_node_fuse_bands,  NULL );
	//wrap_isl_printer(ctx, (void * ) fused_bands, SCHEDULE_NODE);
	//isl_schedule_node_foreach_descendant_top_down is NOT supposed to change the state of the schedule node
	//It is better to use map_descendant_bottom_up which seems better suited
	//for that
#if 1
	// Calling map_descendant bottom up for tiling the code
	isl_schedule_node *new_root = isl_schedule_node_map_descendant_bottom_up(root, node_tiler,  NULL );
	if (new_root == NULL)
	{
		printf("schedule tree visitor failed \n");
		return -1;
	}
	printf("\n\nNew schedule \n\n");
	wrap_isl_printer(ctx, (void * ) new_root, SCHEDULE_NODE);
	//
	printf("new number of band nodes : %d\n", count_band_nodes(new_root));
	isl_schedule_node_free(new_root);
	// These free function are safe,
	// that is to say that there is no double 
	// free risk. Therefore, we can free root
	// even if it is supposed to be done in
	// map_descendant_bottom_up
	// However, this is not recommanded by
	// the official manual. It seems that this
	// reference still counts
	//isl_schedule_node_free(root);
#else
	isl_schedule_node_free(root);
#endif

	return 0;
}
