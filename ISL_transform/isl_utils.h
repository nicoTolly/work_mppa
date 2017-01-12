
#ifndef __ISL_UTILS__
#define __ISL_UTILS__

#include "transform.h"

typedef enum { VAL, BASIC_SET, SET, UNION_SET, SCHEDULE , SCHEDULE_NODE} isl_enum_type ;

void wrap_isl_printer(isl_ctx * ctx, __isl_keep isl_printer ** printer, void * var_to_print, isl_enum_type typ);

#endif //__ISL__TRANSFORM__
