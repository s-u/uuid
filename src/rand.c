/* this provides an isolation between system rand()/srand() and
   the R package (where possible) to avoid API warnings.
   It started as a small proof of concept, but got quite a bit
   out of hand due to Win32 not providing dl* POSIX API ..
*/
#include "config.h"

#include <R.h>

#ifdef HAVE_DLFCN_H
/* on Liunx this need to be set, otherwise RTLD_DEFAULT is not defined */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>
/* it *should* be defined in dlfcn.h, but if it's not, we take a guess */
#ifndef RTLD_DEFAULT
#define RTLD_DEFAULT ((void *) 0)
#endif
#endif

/* there is no dlsym() on Windows, implement it */
#ifdef _WIN32
#include <stdlib.h>
#include <windows.h>
#define HAS_DLSYM 1
#define RTLD_DEFAULT NULL
typedef void(*fn_t)(void);
static BOOL (WINAPI *EnumProcessModulesFn)(HANDLE, HMODULE*, DWORD, LPDWORD);

static HMODULE mods_buf[512]; /* we use a static buffer to avoid dynamic allocations */
static HMODULE *mods = mods_buf;
static int mods_alloc = sizeof(mods_buf);

static fn_t dlsym(void *whatever, const char *name) {
    FARPROC sym;
    HMODULE hModule = GetModuleHandle(NULL);
    /* can we resolve it directly? */
    if ((sym = GetProcAddress(hModule, name)))
	return (fn_t) sym; /* good */

    /* nope, so we have to walk loaded DLLs - annoying, for that we need PSAPI */
    /* FIXME: if we knew the library with the desired symbols, we could load it directly ...
       (on Windows 2008 we found all entries in msvcrt.dll - is that reliable?) */
    if (!EnumProcessModulesFn) { /* load from psapi.dll */
	HMODULE psapi = LoadLibraryA("psapi.dll");
	if (psapi)
	    EnumProcessModulesFn = (BOOL (WINAPI*)(HANDLE, HMODULE*, DWORD, LPDWORD)) GetProcAddress(psapi, "EnumProcessModules");
    }
    if (!EnumProcessModulesFn)
	Rf_error("Cannot find PSAPI.DLL");
    {
        DWORD n, i;
        if (EnumProcessModulesFn(GetCurrentProcess(), mods, mods_alloc, &n)) {
	    if (n > mods_alloc) {
		/* we have to allocate more */
		if (mods != mods_buf)
		    free(mods);
		mods_alloc = n + 64;
		mods = (HMODULE*) malloc(mods_alloc);
		if (!mods)
		    Rf_error("Unable to allocate memory for DLL modules");
		if (!EnumProcessModulesFn(GetCurrentProcess(), mods, mods_alloc, &n))
		    Rf_error("Cannot load DLL module list");
	    }
	    n /= sizeof(HMODULE);
	    for (i = 0; i < n; i++) {
#ifdef PRINT_DLL_SEARCH
		static char tb[512];
		*tb = 0;
		GetModuleFileNameA(mods[i], tb, sizeof(tb));
		Rprintf("> %s\n", tb);
		if ((sym = GetProcAddress(mods[i], name))) {
		    Rprintf("^-- %s found here\n", name);
		    return (fn_t) sym;
		}
#else
		if ((sym = GetProcAddress(mods[i], name)))
		    return (fn_t) sym;
#endif
	    }
	}
    }
    return 0;
}
#endif

/* if we can, let's get the symbols dynamically */
#ifdef HAS_DLSYM
typedef int(*rand_t)(void);
typedef void(*srand_t)(unsigned);
typedef long(*random_t)(void);
typedef void(*srandom_t)(unsigned);
#ifdef _WIN32
typedef errno_t (*rand_s_t)(unsigned int *);
static rand_s_t  fn_rand_s;
#endif
static rand_t    fn_rand;
static srand_t   fn_srand;
static random_t  fn_random;
static srandom_t fn_srandom;

static void load_rand(void) {
    if (!(fn_rand = (rand_t) dlsym(RTLD_DEFAULT, "rand")) ||
#ifdef _WIN32
	!(fn_rand_s = (rand_s_t) dlsym(RTLD_DEFAULT, "rand_s")) ||
#endif
#ifdef HAVE_SRANDOM
        !(fn_random = (random_t) dlsym(RTLD_DEFAULT, "random")) ||
        !(fn_srandom = (srandom_t) dlsym(RTLD_DEFAULT, "srandom")) ||
#endif
	!(fn_srand = (srand_t) dlsym(RTLD_DEFAULT, "srand")))
	Rf_error("Cannot find entry points for random number generators!");
}

int uuid_rand(void) {
    if (!fn_rand)
	load_rand();
#ifdef _WIN32
    /* on Windows there is no good source and no jrand, and relying on
       rand() is terribly bad for repeated calls, so we
       try to use rand_s() which is reportedly tied to crypto API */
    if (fn_rand_s) {
	unsigned int res;
	if (!fn_rand_s(&res))
	    return (int) res;
    }
#endif
    return fn_rand();
}

void uuid_srand(unsigned seed) {
    if (!fn_srand)
	load_rand();
    fn_srand(seed);
}

long uuid_random(void) {
    if (!fn_srand)
	load_rand();
    return fn_random();
}

void uuid_srandom(unsigned seed) {
    if (!fn_srand)
	load_rand();
    fn_srandom(seed);
}

#else

/* no way to find symbols, link them directly and deal with the warnings */
#include <stdlib.h>

int uuid_rand(void) { return rand(); }
void uuid_srand(unsigned seed) { srand(seed); }
long uuid_random(void) { return random(); }
void uuid_srandom(unsigned seed) { srandom(seed); }

#endif
