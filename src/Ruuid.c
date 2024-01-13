#include "Ruuid.h"

static int API_uuid_generate(uuid_t uuid, int type) {
    if (type == GT_DEFAULT) {
	uuid_generate(uuid);
	return 0;
    }
    if (type == GT_TIME) {
	uuid_generate_time(uuid);
	return 0;
    }
    if (type == GT_RANDOM) {
	uuid_generate_random(uuid);
	return 0;
    }
    return -2;
}

static void API_uuid_unparse(uuid_t u, char *out, int lower) {
    if (lower)
	uuid_unparse_lower(u, out);
    else
	uuid_unparse_upper(u, out);
}

static int API_uuid_parse(const char *in_start, const char *in_end, uuid_t uu) {
    return (in_end) ? uuid_parse_range(in_start, in_end, uu) : uuid_parse(in_start, uu);
}

#include <R_ext/Rdynload.h>
#include <Rinternals.h>

SEXP UUID_gen(SEXP sN, SEXP sOut, SEXP sType, SEXP sNS); /* R.c */
SEXP UUID_parse(SEXP sWhat, SEXP sOut);
SEXP UUID_unparse(SEXP sWhat, SEXP sAsUUID);
SEXP UUID_is_NA(SEXP sWhat);
SEXP UUID_cmp(SEXP sE1, SEXP sE2, SEXP sOp);

static const
R_CallMethodDef callMethods[] = {
    {"UUID_gen",     (DL_FUNC) &UUID_gen,     4},
    {"UUID_parse",   (DL_FUNC) &UUID_parse,   2},
    {"UUID_unparse", (DL_FUNC) &UUID_unparse, 2},
    {"UUID_is_NA",   (DL_FUNC) &UUID_is_NA,   1},
    {"UUID_cmp",     (DL_FUNC) &UUID_cmp,     3},
    {NULL, NULL, 0}
};

void R_init_uuid(DllInfo *dll) {
    R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);

    R_RegisterCCallable("uuid", "generate", (DL_FUNC) &API_uuid_generate);
    R_RegisterCCallable("uuid", "unparse",  (DL_FUNC) &API_uuid_unparse);
    R_RegisterCCallable("uuid", "parse",    (DL_FUNC) &API_uuid_parse);
}
