#ifndef _test_h_
#define _test_h_

#include <stdio.h>
#include <assert.h>

#define TEST_ASSERT(name, expr) \
        if (expr) { \
                printf("%-60s  Ok\n", name); \
        } else { \
                printf("%-60s  Failure\n", name); \
                printf("%s:%d assert `"#expr"` failed\n", __FILE__, __LINE__); \
        } \
        fflush(stdout);

#endif

