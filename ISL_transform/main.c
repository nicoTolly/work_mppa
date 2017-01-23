/*
 * Transformations inspired by ppcg
 * Using pet for extract polyhedral scope
 * from inputs
 */

#include "globals.h"
#include "isl_utils.h"
#include "transform.h"
#include "schedule_visitor.h"





int main(int argc, char** argv)
{
	ctx = isl_ctx_alloc_with_pet_options();
	printer = isl_printer_to_file(ctx, stdout);
	//isl_printer *printer = isl_printer_to_file(ctx, stdout);




	// check that user gave an argument and that this
	// argument is indeed a valid file
	if (argv[1] == NULL || access(argv[1], F_OK))
	{
		printf( "You must pass a valid file name as argument\n");
		return -1;
	}
	pet_scop* scop =  pet_scop_extract_from_C_source(ctx, argv[1], NULL);


	isl_schedule * filesched = pet_scop_get_schedule(scop);
	//printf("printing schedule from file\n");
	//wrap_isl_printer(ctx, (void *) filesched, SCHEDULE);
	schedule_visitor( filesched, NULL);

	isl_schedule_free(filesched);

	printf("scop starts at offset %d\n", pet_loc_get_start(scop->loc));

	pet_scop_free(scop);


	isl_printer_free(printer);
	isl_ctx_free(ctx);
	return 0;
}
