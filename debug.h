#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifndef NDEBUG
FILE* debug_get_stream();

#define dprintf(...) fprintf(debug_get_stream(), __VA_ARGS__)
#else
#define dprintf(...) ;
#endif

#endif
