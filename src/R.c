#include "uuid.h"

#include <Rinternals.h>

SEXP UUID_gen(SEXP sTime) {
    uuid_t u;
    char c[40];
    int use_time = asInteger(sTime);
    if (use_time == TRUE)
	uuid_generate_time(u);
    else if (use_time == FALSE)
	uuid_generate_random(u);
    else
	uuid_generate(u);
    uuid_unparse_lower(u, c);
    return mkString(c);
}

static const R_CallMethodDef uuid_entries[] = {
  {"UUID_gen", (DL_FUNC) &UUID_gen, 1},
  {NULL, NULL, 0}
};

void R_init_uuid(DllInfo *dll) {
  R_registerRoutines(dll, NULL, uuid_entries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
