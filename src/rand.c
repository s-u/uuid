/* this provides an isolation between system rand()/srand() and
   the R package (where possible) to avoid API warnings.
   It started as a small proof of concept, but got quite a bit
   out of hand due to Win32 not providing dl* POSIX API ..
*/
#include "config.h"

#include <R.h>

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif

/* there is no dlsym() on Windows, implement it */
#ifdef _WIN32
#include <windows.h>
#define HAS_DLSYM 1
#define RTLD_DEFAULT NULL
typedef void(*fn_t)();
static BOOL (WINAPI *EnumProcessModulesFn)(HANDLE, HMODULE*, DWORD, LPDWORD);
static fn_t dlsym(void *whatever, const char *name) {
    FARPROC sym;
    HMODULE hModule = GetModuleHandle(NULL);
    /* can we resolve it directly? */
    if ((sym = GetProcAddress(hModule, name)))
	return (fn_t) sym; /* good */

    /* nope, so we have to walk loaded DLLs - annoying, for that we need PSAPI */
    /* FIXME: if we knew the library with the desired symbols, we could load it directly ... */
    if (!EnumProcessModulesFn) { /* load from psapi.dll */
	HMODULE psapi = LoadLibraryA("psapi.dll");
	if (psapi)
	    EnumProcessModulesFn = (BOOL (WINAPI*)(HANDLE, HMODULE*, DWORD, LPDWORD)) GetProcAddress(psapi, "EnumProcessModules");
    }
    if (!EnumProcessModulesFn)
	Rf_error("Cannot find PSAPI.DLL");
    {
        static HMODULE mods[512]; /* we use a static buffer to avoid dynamic allocations */
        DWORD n, i;

        if (EnumProcessModulesFn(GetCurrentProcess(), mods, sizeof(mods), &n)) {
	    if (n > sizeof(mods))
		Rf_error("Too many DLL modules.");
	    n /= sizeof(HMODULE);
	    for (i = 0; i < n; i++)
		if ((sym = GetProcAddress(mods[i], name)))
		    return (fn_t) sym;
	}
    }
    return 0;
}
#endif

/* if we can, let's get the symbols dynamically */
#ifdef HAS_DLSYM
typedef int(*rand_t)();
typedef void(*srand_t)(unsigned);
static rand_t  fn_rand;
static srand_t fn_srand;

static void load_rand() {
    if (!(fn_rand = (rand_t) dlsym(RTLD_DEFAULT, "rand")) ||
	!(fn_srand = (srand_t) dlsym(RTLD_DEFAULT, "srand")))
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
