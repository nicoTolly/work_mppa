#include <isl/ctx.h>
#include <isl/set.h>
#include <isl/val.h>
#include <isl/schedule.h>
#include <isl/ast.h>
#include <isl/space.h>
#include <isl/set.h>
#include <isl/union_set.h>


#include <pet.h>

#include <stdio.h>
#include <stdlib.h>

typedef enum { VAL, BASIC_SET, SET, UNION_SET, SCHEDULE} isl_enum_type ;

void wrap_isl_printer(isl_ctx * ctx, __isl_keep isl_printer ** printer, void * var_to_print, isl_enum_type typ);
