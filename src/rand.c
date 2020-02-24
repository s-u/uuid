/* this provides an isolation between system rand()/srand() and
   the R package (where possible) to avoid API warnings */
#include "config.h"

#include <R.h>

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif

#ifdef HAS_DLSYM
static int(*fn_rand)();
static void(*fn_srand)(unsigned);

static void load_rand() {
    if (!(fn_rand = dlsym(RTLD_DEFAULT, "rand")) ||
	!(fn_srand = dlsym(RTLD_DEFAULT, "srand")))
	Rf_error("Cannot find entry points for random number generators!");
}

int uuid_rand(void) {
    if (!fn_rand)
	load_rand();
    return fn_rand();
}

void uuid_srand(unsigned seed) {
    if (!fn_srand)
	load_rand();
    fn_srand(seed);
}
#else

/* no way to find symbols, link them directly and deal with the warnings */
#include <stdlib.h>

int uuid_rand(void) { return rand(); }
void uuid_srand(unsigned seed) { srand(seed); }

#endif
