#include "uuid.h"

#include <Rinternals.h>
#include <string.h>
#include <stddef.h>

#define OF_STRING  0
#define OF_RAW     1
#define OF_CPLX    2
#define OF_VALID   3

/* x is expected to be protected, this does allocate; n is the number of uuids */
static void set_raw_dim(SEXP x, size_t n) {
    /* FIXME: INT won't be enough for n > 2^31 */
    SEXP sDim = Rf_allocVector(INTSXP, 2);
    INTEGER(sDim)[0] = sizeof(uuid_t);
    INTEGER(sDim)[1] = n;
    Rf_setAttrib(x, R_DimSymbol, sDim); /* setAttrib does PROTECT */
}

SEXP UUID_gen(SEXP sN, SEXP sOut, SEXP sType, SEXP sNS) {
    uuid_t u;
    SEXP res;
    char c[40];
    int form = asInteger(sOut);
    int ver  = asInteger(sType); /* use.time=TRUE -> 1, use.time=FALSE -> 4, use.time=NA -> NA */
    ptrdiff_t i, n = -1;
    uuid_t *dst = &u;
    uuid_t ns;
    if (ver == 3 || ver == 5) {
	/* use sN as the name vector and sNS UUID; R code does the necessary coersions */
	n = (ptrdiff_t) XLENGTH(sN);
	if (!(TYPEOF(sNS) == CPLXSXP && Rf_inherits(sNS, "UUID")))
	    Rf_error("namespace must be a valid UUID object");
	memcpy(&ns, (uuid_t*) COMPLEX(sNS), sizeof(uuid_t));
    } else {
	if (TYPEOF(sN) == INTSXP && LENGTH(sN) > 0)
	    n = INTEGER(sN)[0];
	if (TYPEOF(sN) == REALSXP && LENGTH(sN) > 0)
	    n = (ptrdiff_t)REAL(sN)[0];
	if (n < 0)
	    Rf_error("invalid n, must be a positive integer");
    }
    switch(form) {
    case OF_STRING: res = PROTECT(allocVector(STRSXP, n)); break;
    case OF_RAW:    res = PROTECT(allocVector(RAWSXP, n * sizeof(uuid_t))); dst = (uuid_t*) RAW(res); break;
    case OF_CPLX:   res = PROTECT(allocVector(CPLXSXP, n)); dst = (uuid_t*) ((void*) COMPLEX(res)); break;
    default:
	Rf_error("invalid output format specification");
    }
    for (i = 0; i < n; i++) {
	switch(ver) {
	case 1: 
	    uuid_generate_time(*dst); break;
	case 4:
	    uuid_generate_random(*dst); break;
	case 3:
	case 5:
	    {
		const char *name = CHAR(STRING_ELT(sN, i));
		if (ver == 3)
		    uuid_generate_md5(*dst, ns, name, strlen(name));
		else
		    uuid_generate_sha1(*dst, ns, name, strlen(name));
		break;
	    }
	default:
	    uuid_generate(*dst);
	}

	if (form == OF_STRING) {
	    uuid_unparse_lower(u, c);
	    SET_STRING_ELT(res, i, mkChar(c));
	} else
	    dst++;
    }
    if (form == OF_RAW && n > 1)
	set_raw_dim(res, n);
    if (form == OF_CPLX)
	Rf_setAttrib(res, R_ClassSymbol, Rf_mkString("UUID"));
    UNPROTECT(1);
    return res;
}

static void cplx_set_NA(void* dst) {
    Rcomplex *c = (Rcomplex*) dst;
    c->r = NA_REAL;
    c->i = NA_REAL;
}

SEXP UUID_parse(SEXP sWhat, SEXP sOut) {
    uuid_t u;
    SEXP res;
    char c[40];
    int form = asInteger(sOut);
    size_t i, n = (size_t) XLENGTH(sWhat);
    uuid_t *dst = &u;
    int *lgl = 0;
    if (TYPEOF(sWhat) != STRSXP)
	Rf_error("input must be a character vector");
    switch(form) {
    case OF_STRING: res = PROTECT(allocVector(STRSXP, n)); break;
    case OF_RAW:    res = PROTECT(allocVector(RAWSXP, n * sizeof(uuid_t))); dst = (uuid_t*) RAW(res); break;
    case OF_CPLX:   res = PROTECT(allocVector(CPLXSXP, n)); dst = (uuid_t*) ((void*) COMPLEX(res)); break;
    case OF_VALID:  res = PROTECT(allocVector(LGLSXP, n)); lgl = LOGICAL(res); break;
    default:
	Rf_error("invalid output format specification");
    }
    for (i = 0; i < n; i++) {
	SEXP elt = STRING_ELT(sWhat, i);
	if (elt == NA_STRING) {
	    switch (form) {
	    case OF_STRING: SET_STRING_ELT(res, i, NA_STRING); break;
	    case OF_RAW:
	    case OF_CPLX:
		cplx_set_NA(*dst);
		dst++;
		break;
	    case OF_VALID: lgl[i] = NA_LOGICAL; break;
	    }
	    continue;	
	}

	int pr = uuid_parse(CHAR(STRING_ELT(sWhat, i)), *dst);
	switch (form) {
	case OF_STRING:
	    if (pr) /* FAIL */
		SET_STRING_ELT(res, i, NA_STRING);
	    else {
		uuid_unparse_lower(u, c);
		SET_STRING_ELT(res, i, mkChar(c));
	    }
	    break;
	case OF_VALID:
	    lgl[i] = (pr == 0) ? 1 : 0;
	    break;
	case OF_CPLX:
	case OF_RAW:
	    if (pr != 0)
		cplx_set_NA(*dst);
	    dst++;
	    break;
	}
    }
    if (form == OF_RAW && n > 1)
	set_raw_dim(res, n);
    if (form == OF_CPLX)
	Rf_setAttrib(res, R_ClassSymbol, Rf_mkString("UUID"));
    UNPROTECT(1);
    return res;
}

SEXP UUID_unparse(SEXP sWhat, SEXP sAsUUID) {
    SEXP res;
    uuid_t *src = 0, NA_value;
    char c[40];
    size_t n, i;
    int as_uuid = Rf_asInteger(sAsUUID);
    cplx_set_NA(NA_value);
    if (TYPEOF(sWhat) == RAWSXP) {
	size_t len = XLENGTH(sWhat);
	SEXP dim = Rf_getAttrib(sWhat, R_DimSymbol);
	if ((dim != R_NilValue) &&
	    (!(
	       ((TYPEOF(dim) == INTSXP) && (LENGTH(dim) >= 1) && INTEGER(dim)[0] == sizeof(uuid_t)) ||
	       ((TYPEOF(dim) == REALSXP) && (LENGTH(dim) >= 1) && REAL(dim)[0] == (double) sizeof(uuid_t)))))
	    Rf_error("invalid dimensions, first dimension must be 16");
	/* in case there are no dimensions */
	if (len % sizeof(uuid_t))
	    Rf_error("invalid raw vector length, this cannot be UUID");
	src = (uuid_t*) RAW(sWhat);
	n = len / sizeof(uuid_t);
    } else if (TYPEOF(sWhat) == CPLXSXP && Rf_inherits(sWhat, "UUID")) {
	src = (uuid_t*) COMPLEX(sWhat);
	n = XLENGTH(sWhat);
    } else Rf_error("Unparsing only works for raw vectors and UUID objects");

    /* this is not really unparsing, but rather conversion UUID <-> raw ... */
    if (as_uuid) {
	if (as_uuid == 2) {
	    res = PROTECT(Rf_allocVector(RAWSXP, n * sizeof(uuid_t)));
	    memcpy(RAW(res), src, n * sizeof(uuid_t));
	    if (n > 1)
		set_raw_dim(res, n);
	    UNPROTECT(1);
	    return res;
	} else {
	    res = PROTECT(Rf_allocVector(CPLXSXP, n));
	    memcpy(COMPLEX(res), src, n * sizeof(uuid_t));
	    Rf_setAttrib(res, R_ClassSymbol, Rf_mkString("UUID"));
	    UNPROTECT(1);
	    return res;
	}
    }	    

    res = PROTECT(Rf_allocVector(STRSXP, n));
    for (i = 0; i < n; i++) {
	if (src[i][6] == 0xf0 && !memcmp(src[i], NA_value, sizeof(uuid_t)))
	    SET_STRING_ELT(res, i, NA_STRING);
	else {
	    uuid_unparse_lower(src[i], c);
	    SET_STRING_ELT(res, i, mkChar(c));
	}
    }
    UNPROTECT(1);
    return res;
}

SEXP UUID_is_NA(SEXP sWhat) {
    uuid_t *src, NA_value;
    size_t i, n;
    int *lgl;
    SEXP res;
    if (!(TYPEOF(sWhat) == CPLXSXP && Rf_inherits(sWhat, "UUID")))
	Rf_error("must be UUID object");
    cplx_set_NA(NA_value);
    n = XLENGTH(sWhat);
    src = (uuid_t*) COMPLEX(sWhat);
    res = Rf_allocVector(LGLSXP, n);
    lgl = LOGICAL(res);
    for (i = 0; i < n; i++)
	lgl[i] = memcmp(src[i], NA_value, sizeof(uuid_t)) ? 0 : 1;
    return res;
}

SEXP UUID_cmp(SEXP sE1, SEXP sE2, SEXP sOp) {
    uuid_t *s1, *s2, NA_value;
    size_t i1 = 0, i2 = 0, i = 0, n1, n2, n;
    SEXP res;
    int *lgl;
    int op = Rf_asInteger(sOp);
    if (!(TYPEOF(sE1) == CPLXSXP && Rf_inherits(sE1, "UUID") &&
	  TYPEOF(sE2) == CPLXSXP && Rf_inherits(sE2, "UUID")))
	Rf_error("both operands must be UUID objects");
    n1 = XLENGTH(sE1);
    n2 = XLENGTH(sE2);
    if (n1 == 0 || n2 == 0)
	return Rf_allocVector(LGLSXP, 0);

    cplx_set_NA(NA_value);
    s1 = (uuid_t*) COMPLEX(sE1);
    s2 = (uuid_t*) COMPLEX(sE2);
    n = (n1 > n2) ? n1 : n2;
    res = Rf_allocVector(LGLSXP, n);
    lgl = LOGICAL(res);
    while (i < n) {
	if ((!memcmp(s1[i1], NA_value, sizeof(uuid_t))) ||
	    (!memcmp(s2[i2], NA_value, sizeof(uuid_t))))
	    lgl[i] = NA_LOGICAL;
	else
	    lgl[i] = (memcmp(s1[i1], s2[i2], sizeof(uuid_t)) == op) ? 1 : 0;
	if (++i1 == n1)
	    i1 = 0;
	if (++i2 == n2)
	    i2 = 0;
	i++;
    }
    return res;
}
