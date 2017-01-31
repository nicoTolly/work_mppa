
#ifndef __ISL_UTILS__
#define __ISL_UTILS__

#include "transform.h"

typedef enum { VAL, BASIC_SET, SET, UNION_SET, SCHEDULE , SCHEDULE_NODE, MULTI_UNION_PW_AFF} isl_enum_type ;

void wrap_isl_printer(isl_ctx * ctx, void * var_to_print, isl_enum_type typ);


void print_type(isl_schedule_node * node);
#endif //__ISL__TRANSFORM__
