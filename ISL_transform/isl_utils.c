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
		case MULTI_UNION_PW_AFF:
			printer = isl_printer_print_multi_union_pw_aff(printer, (isl_multi_union_pw_aff *) var_to_print );
			break;
		default:
			printf("problem : not a valid type");
			break;
	}
	printf("\n");
}

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
