#include "schedule_visitor.h"

// given a node, print its type
void print_type(isl_schedule_node * node)
{
	enum isl_schedule_node_type ntype = isl_schedule_node_get_type( node);

	switch(ntype){
	case isl_schedule_node_band:
		printf("band type\n");
		break;
	case isl_schedule_node_context:
		printf("context type\n");
		break;
	case isl_schedule_node_domain:
		printf("domain type\n");
		break;
	case isl_schedule_node_expansion:
		printf("expansion type\n");
		break;
	case isl_schedule_node_extension:
		printf("extension type\n");
		break;
	case isl_schedule_node_filter:
		printf("filter type\n");
		break;
	case isl_schedule_node_leaf:
		printf("leaf type\n");
		break;
	case isl_schedule_node_guard:
		printf("guard type\n");
		break;
	case isl_schedule_node_mark:
		printf("mark type\n");
		break;
	case isl_schedule_node_sequence:
		printf("sequence type\n");
		break;
	case isl_schedule_node_set:
		printf("set type\n");
		break;
	default:
	break;
	}
}

// A function to be passed as callback to foreach_descendant
// Simply print the node passed as parameter
// user is a struct containing printer and ctx
isl_bool print_node(__isl_keep isl_schedule_node *node, void* user)
{
	print_arg_t * args = (print_arg_t *) user;
	print_type(node);
	wrap_isl_printer(args->ctx, &args->printer, (void * ) node, SCHEDULE_NODE);
	return isl_bool_true;
}

//A function for visiting a schedule tree, maybe modifying it
int schedule_visitor( isl_ctx * ctx, __isl_keep isl_printer *printer, __isl_keep isl_schedule * schedule, void * (*fn) (void *))
{
	isl_schedule_node * root = isl_schedule_get_root(schedule);
	schedule_tree_visitor(ctx, printer, root);
	print_arg_t args = {ctx, printer};
	//isl_schedule_node_foreach_descendant_top_down(root, print_node,  (void * )&args );
	//isl_schedule_foreach_schedule_node_top_down(schedule, print_node,  (void * )&args );
	isl_schedule_node_free(root);
	return 0;
}

void schedule_tree_visitor(isl_ctx *ctx, isl_printer * printer, isl_schedule_node * node  )
{
	enum isl_schedule_node_type ntype = isl_schedule_node_get_type( node);
	wrap_isl_printer(ctx, &printer, (void * ) node, SCHEDULE_NODE);
	print_type( node);
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
		wrap_isl_printer(ctx, &printer, (void * ) new_node, SCHEDULE_NODE);
		schedule_tree_visitor(ctx, printer, new_node);

	}
	break;
case isl_schedule_node_band :
case isl_schedule_node_context :
case isl_schedule_node_domain :
case isl_schedule_node_leaf :
	if ( isl_schedule_node_has_children(node))
	{
		printf("node has children\n");
		//isl_schedule_node * pnode = isl_schedule_node_copy(node);
		//isl_schedule_node * cnode = isl_schedule_node_first_child(node);
		isl_schedule_node * cnode = isl_schedule_node_get_child(node, 0);
		wrap_isl_printer(ctx, &printer, (void * ) cnode, SCHEDULE_NODE);
		//node = pnode;
		schedule_tree_visitor(ctx, printer, cnode);

	}
	break;
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
		schedule_tree_visitor(ctx, printer, children[i]);
		printf("print node in child\n");
		wrap_isl_printer(ctx, &printer, (void * ) children[i], SCHEDULE_NODE);
		node = pnode;
	}
	break;
default :
	break;
	}
}
