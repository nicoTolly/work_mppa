#include "isl_utils.h"
#include "globals.h"


/* Wrap around all the pretty print defined
 * in isl_printer with an enum for deciding which 
 * type to use
 * */
void wrap_isl_printer(isl_ctx * ctx,  void * var_to_print, isl_enum_type typ)
{
	switch (typ)
	{
		case VAL:
			printer = isl_printer_print_val(printer, (isl_val *) var_to_print );
			break;
		case BASIC_SET:
			printer = isl_printer_print_basic_set(printer, (isl_basic_set *) var_to_print );
			break;
		case UNION_SET:
			printer = isl_printer_print_union_set(printer, (isl_union_set *) var_to_print );
			break;
		case SCHEDULE:
			printer = isl_printer_print_schedule(printer, (isl_schedule *) var_to_print );
			break;
		case SCHEDULE_NODE:
			printer = isl_printer_print_schedule_node(printer, (isl_schedule_node *) var_to_print );
			break;
		default:
			printf("problem : not a valid type");
			break;
	}
	printf("\n");
}
