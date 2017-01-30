
/*
 * Transformations inspired by ppcg
 * Using pet for extract polyhedral scope
 * from inputs
 */

//#include "globals.h"
//#include "isl_utils.h"
//#include "transform.h"
//#include "schedule_visitor.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#include <isl/printer.h>
#include <isl/ctx.h>
#include <pet.h>





int main(int argc, char** argv)
{
	isl_ctx * ctx = isl_ctx_alloc_with_pet_options();
	isl_printer *printer = isl_printer_to_file(ctx, stdout);
	//isl_printer *printer = isl_printer_to_file(ctx, stdout);




	// check that user gave an argument and that this
	// argument is indeed a valid file
	if (argv[1] == NULL || access(argv[1], F_OK))
	{
		printf( "You must pass a valid file name as argument\n");
		return -1;
	}
	pet_scop* scop =  pet_scop_extract_from_C_source(ctx, argv[1], NULL);
	if (scop == NULL)
	{
		printf("Could not build a valid scop\n");
		return -1;
	}

	isl_schedule * filesched = pet_scop_get_schedule(scop);
	//printf("printing schedule from file\n");
	//wrap_isl_printer(ctx, (void *) filesched, SCHEDULE);
	int containsDependances = pet_scop_has_data_dependent_accesses(scop);
	if (containsDependances)
		printf("This scop does contain data dependances\n");
	else
		printf("This scop does not contain data dependances\n");
	//schedule_visitor( filesched, NULL);


	isl_schedule_free(filesched);

	printf("scop starts at offset %d\n", pet_loc_get_start(scop->loc));

	pet_scop_free(scop);


	isl_printer_free(printer);
	isl_ctx_free(ctx);
	return 0;
}
