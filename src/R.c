#include "uuid.h"

#include <Rinternals.h>

SEXP UUID_gen(SEXP sTime, SEXP sN) {
    uuid_t u;
    SEXP res;
    char c[40];
    int use_time = asInteger(sTime);
    int i, n = asInteger(sN);
    if (n < 0)
	Rf_error("invalid n, must be a positive integer <2^31");
    res = PROTECT(allocVector(STRSXP, n));
    if (use_time == TRUE)
	for (i = 0; i < n; i++) {
	    uuid_generate_time(u);
	    uuid_unparse_lower(u, c);
	    SET_STRING_ELT(res, i, mkChar(c));
	}
    else if (use_time == FALSE)
	for (i = 0; i < n; i++) {
	    uuid_generate_random(u);
	    uuid_unparse_lower(u, c);
	    SET_STRING_ELT(res, i, mkChar(c));
	}
    else
	for (i = 0; i < n; i++) {
	    uuid_generate(u);
	    uuid_unparse_lower(u, c);
	    SET_STRING_ELT(res, i, mkChar(c));
	}
    UNPROTECT(1);
    return res;
}
