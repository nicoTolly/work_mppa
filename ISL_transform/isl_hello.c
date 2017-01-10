/*
 * Transformations inspired by ppcg
 * Using pet for extract polyhedral scope
 * from inputs
 */


#include "isl_hello.h"

__isl_give isl_printer * transf(isl_printer *printer, pet_scop * scop,void * user)
{
	return printer;
}

/* Wrap around all the pretty print defined
 * in isl_printer with an enum for deciding which 
 * type to use
 * */
void wrap_isl_printer(isl_ctx * ctx, __isl_keep isl_printer ** printer, void * var_to_print, isl_enum_type typ)
{
	switch (typ)
	{
		case VAL:
			*printer = isl_printer_print_val(*printer, (isl_val *) var_to_print );
			break;
		case BASIC_SET:
			*printer = isl_printer_print_basic_set(*printer, (isl_basic_set *) var_to_print );
			break;
		case UNION_SET:
			*printer = isl_printer_print_union_set(*printer, (isl_union_set *) var_to_print );
			break;
		case SCHEDULE:
			*printer = isl_printer_print_schedule(*printer, (isl_schedule *) var_to_print );
			break;
		default:
			printf("problem : not a valid type");
			break;
	}
	printf("\n");
}

int main(int argc, char** argv)
{
  isl_ctx * ctx = isl_ctx_alloc_with_pet_options();
  isl_printer *printer = isl_printer_to_file(ctx, stdout);
  isl_space * space = isl_space_params_alloc(ctx, 3);

  isl_basic_set * bset = isl_basic_set_empty(space);

//  isl_val * vzero = isl_val_zero(ctx);
//  isl_val * vinf = isl_val_infty(ctx);
//  isl_val * vone = isl_val_int_from_si(ctx, 1);

  FILE * bidule = fopen("bidule.txt", "w+");



  // print empty basic set
  wrap_isl_printer(ctx, &printer, (void *) bset, BASIC_SET);
  isl_basic_set_free(bset);

  isl_basic_set * bset2 = isl_basic_set_read_from_str(ctx, "{[i,j] :   i + j = 12 and 1 < i < 10 and 0 < j < 10 }");
  printf("Printing basic set\n");
  wrap_isl_printer(ctx, &printer, (void *) bset2, BASIC_SET);

  isl_union_set * uset = isl_union_set_from_basic_set(bset2);
  printf("Printing union set\n");
  wrap_isl_printer(ctx, &printer, (void *) uset, UNION_SET);

  //get the standard schedule
  isl_schedule * sched = isl_schedule_from_domain(uset);
  isl_schedule_free(sched);

  //print it
  printf("printing schedule\n");
  wrap_isl_printer(ctx, &printer, (void *) sched, SCHEDULE);

  //TEST pet transform 
  // extract scop
  pet_scop* scop =  pet_scop_extract_from_C_source(ctx, argv[1], NULL);


  isl_schedule * filesched = pet_scop_get_schedule(scop);
  printf("printing schedule from file\n");
  wrap_isl_printer(ctx, &printer, (void *) filesched, SCHEDULE);
  isl_schedule_free(filesched);

  printf("scop starts at offset %d\n", pet_loc_get_start(scop->loc));

  // print original code
  printf("printing original scop code \n");
  pet_transform_C_source(ctx, argv[1], stdout, pet_scop_print_original, NULL); 
  pet_scop_free(scop);
  
  
 fclose(bidule);
  isl_printer_free(printer);
  isl_ctx_free(ctx);
  return 0;
}
