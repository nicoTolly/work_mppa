/*
 * Transformations inspired by ppcg
 * Using pet for extract polyhedral scope
 * from inputs
 */


#include "isl_utils.h"
#include "transform.h"
#include "schedule_visitor.h"

__isl_give isl_printer * transf(isl_printer *printer, pet_scop * scop,void * user)
{
	return printer;
}




int main(int argc, char** argv)
{
  isl_ctx * ctx = isl_ctx_alloc_with_pet_options();
  isl_printer *printer = isl_printer_to_file(ctx, stdout);
  //isl_space * space = isl_space_params_alloc(ctx, 3);

  //isl_basic_set * bset = isl_basic_set_empty(space);

//  isl_val * vzero = isl_val_zero(ctx);
//  isl_val * vinf = isl_val_infty(ctx);
//  isl_val * vone = isl_val_int_from_si(ctx, 1);

  //FILE * bidule = fopen("bidule.txt", "w+");



  // print empty basic set
  //wrap_isl_printer(ctx, &printer, (void *) bset, BASIC_SET);
  //isl_basic_set_free(bset);

  isl_basic_set * bset2 = isl_basic_set_read_from_str(ctx, "{[i,j] :   i + j = 12 and 1 < i < 10 and 0 < j < 10 }");
  printf("Printing basic set\n");
  wrap_isl_printer(ctx, &printer, (void *) bset2, BASIC_SET);

  // build an union set from basic set. Free bset
  isl_union_set * uset = isl_union_set_from_basic_set(bset2);
  printf("Printing union set\n");
  wrap_isl_printer(ctx, &printer, (void *) uset, UNION_SET);

  //get the standard schedule
  isl_schedule * sched = isl_schedule_from_domain(uset);

  //print it
  printf("printing schedule\n");
  wrap_isl_printer(ctx, &printer, (void *) sched, SCHEDULE);
  isl_union_set_free(uset);
  isl_schedule_free(sched);

  //TEST pet transform 
  // extract scop
  if (argv[1] == NULL)
  {
	  printf( "You must pass a file name as argument\n");
	  return -1;
  }
  //pet_scop* scop =  pet_scop_extract_from_C_source(ctx, "simple.c", NULL);
  pet_scop* scop =  pet_scop_extract_from_C_source(ctx, argv[1], NULL);


  isl_schedule * filesched = pet_scop_get_schedule(scop);
  printf("printing schedule from file\n");
  wrap_isl_printer(ctx, &printer, (void *) filesched, SCHEDULE);
  schedule_visitor(ctx, filesched, NULL);
  isl_schedule_free(filesched);

  printf("scop starts at offset %d\n", pet_loc_get_start(scop->loc));

  // print original code
  printf("printing original scop code \n");
  //pet_transform_C_source(ctx, argv[1], stdout, pet_scop_print_original, NULL); 
  pet_scop_free(scop);
  
  
  isl_printer_free(printer);
  isl_ctx_free(ctx);
  return 0;
}
