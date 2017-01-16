/*
 * Transformations inspired by ppcg
 * Using pet for extract polyhedral scope
 * from inputs
 */


#include "isl_utils.h"
#include "transform.h"
#include "schedule_visitor.h"





int main(int argc, char** argv)
{
  isl_ctx * ctx = isl_ctx_alloc_with_pet_options();
  isl_printer *printer = isl_printer_to_file(ctx, stdout);




  // check that user gave an argument and that this
  // argument is indeed a valid file
  if (argv[1] == NULL || access(argv[1], F_OK))
  {
	  printf( "You must pass a valid file name as argument\n");
	  return -1;
  }
  //pet_scop* scop =  pet_scop_extract_from_C_source(ctx, "simple.c", NULL);
  pet_scop* scop =  pet_scop_extract_from_C_source(ctx, argv[1], NULL);


  isl_schedule * filesched = pet_scop_get_schedule(scop);
  //printf("printing schedule from file\n");
  //wrap_isl_printer(ctx, &printer, (void *) filesched, SCHEDULE);
  schedule_visitor(ctx, printer, filesched, NULL);
  isl_schedule_free(filesched);

  printf("scop starts at offset %d\n", pet_loc_get_start(scop->loc));

  // print original code
  //printf("printing original scop code \n");
  //pet_transform_C_source(ctx, argv[1], stdout, pet_scop_print_original, NULL); 
  pet_scop_free(scop);
  
  
  isl_printer_free(printer);
  isl_ctx_free(ctx);
  return 0;
}
