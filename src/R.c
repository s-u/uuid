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


SEXP UUID_validate(SEXP uuids) {
  SEXP out = PROTECT(allocVector(LGLSXP, length(uuids))); // allocate return value (out).
  uuid_t uuid_bin;

  for(int i = 0; i < length(uuids); i++) {
    if(STRING_ELT(uuids, i) == NA_STRING) {
      LOGICAL(out)[i] = NA_LOGICAL;
      continue;
    }

    // uuid_parse returns 0 on success, -1 on failure.
    // adding 1 will coerce to C bool, but the below is more defensive.
    // 0 is success, _anything_ else is failure (to future-proof against possible libuuid changes to error return values).
    int x = uuid_parse(CHAR(STRING_ELT(uuids, i)), uuid_bin);
    LOGICAL(out)[i] = x == 0 ? 1 : 0;
  } // closes for-loop
  
  UNPROTECT(1);
  return out;
}
