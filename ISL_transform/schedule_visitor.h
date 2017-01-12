#ifndef __SCHEDULE_VISITOR_H__
#define __SCHEDULE_VISITOR_H__

#include "transform.h"
#include "isl_utils.h"

#include <isl/schedule.h>
#include <isl/schedule_node.h>
#include <isl/schedule_type.h>
#include <isl/ctx.h>

typedef struct 
{
	isl_ctx * ctx;
	isl_printer * printer;
}print_arg_t;

// entry point for visiting a tree
// the callback function can perform
// certain transformations on schedule
int schedule_visitor( isl_ctx * ctx, __isl_keep isl_printer *printer, isl_schedule * schedule, void * (*fn) (void *));
void schedule_tree_visitor(isl_ctx *ctx, isl_printer * printer, isl_schedule_node * node  );
#endif //__SCHEDULE_VISITOR_H__
