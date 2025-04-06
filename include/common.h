#ifndef COMMON_H_
#define COMMON_H_

#include <stdlib.h>

#define error(...)                                        \
    do {                                                  \
        fprintf (stderr, "[%s:%d] ", __FILE__, __LINE__); \
        fprintf (stderr, __VA_ARGS__);                    \
        fprintf (stderr, "\n");                           \
        exit (EXIT_FAILURE);                              \
    } while (0);

#endif
