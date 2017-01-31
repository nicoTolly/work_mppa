#ifndef __SCHEDULE_VISITOR_H__
#define __SCHEDULE_VISITOR_H__

#include "globals.h"
#include "transform.h"
#include "isl_utils.h"
#include "node_count.h"
#include "band_fuse.h"

#include <isl/schedule.h>
#include <isl/schedule_node.h>
#include <isl/schedule_type.h>
#include <isl/ctx.h>

typedef struct 
{
	isl_ctx * ctx;
}arg_t;

// entry point for visiting a tree
// the callback function can perform
// certain transformations on schedule
int schedule_visitor(   isl_schedule * schedule, void * (*fn) (void *));
isl_schedule_node * schedule_tree_visitor(isl_ctx *ctx, isl_schedule_node * node  );
// function used as callback for map_descendant_bottom_up
isl_schedule_node * node_tiler(isl_schedule_node * node, void * user);
#endif //__SCHEDULE_VISITOR_H__
