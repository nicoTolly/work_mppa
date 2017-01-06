#include <isl/ctx.h>
#include <isl/set.h>
#include <isl/val.h>

#include <pet.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  isl_ctx * ctx = isl_ctx_alloc();
  isl_printer *printer = isl_printer_to_file(ctx, stdout);


  isl_val * vzero = isl_val_zero(ctx);
  isl_val * vinf = isl_val_infty(ctx);
  isl_val * vun = isl_val_int_from_si(ctx, 1);

  isl_bool b = isl_val_gt(vun, vzero);

  printer = isl_printer_print_val(printer, vzero);
  printf("\n");


  isl_val_free(vzero);
  isl_val_free(vun);
  isl_val_free(vinf);
  isl_printer_free(printer);
  isl_ctx_free(ctx);
  return 0;
}
