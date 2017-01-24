
#ifndef __BAND_FUSE_H__
#define __BAND_FUSE_H__

#include "globals.h"
#include "transform.h"
#include "isl_utils.h"

#include <isl/schedule.h>
#include <isl/schedule_node.h>
#include <isl/schedule_type.h>
#include <isl/ctx.h>


__isl_give isl_schedule_node * band_fuse(__isl_take isl_schedule_node * node);
isl_schedule_node * schedule_node_fuse_bands(isl_schedule_node * node, void * user);
#endif //__BAND_FUSE_H__
