#ifndef UTIL_LINUX_RANDUTILS
#define UTIL_LINUX_RANDUTILS

/* map dynamic symbols */
#define srandom(x) uuid_srandom(x)
#define random()   uuid_random()

#ifdef HAVE_SRANDOM
#define srand(x)	uuid_srandom(x)
#define rand()		uuid_random()
#else
#define srand(x) uuid_srand(x)
#define rand()   uuid_rand
#endif

/* from rand.c */
extern int uuid_rand(void);
extern void uuid_srand(unsigned seed);
extern long uuid_random(void);
extern void uuid_srandom(unsigned seed);

/* rand() based */
extern int rand_get_number(int low_n, int high_n);

/* /dev/urandom based with fallback to rand() */
extern int random_get_fd(void);
extern int ul_random_get_bytes(void *buf, size_t nbytes);
extern const char *random_tell_source(void);

#endif
