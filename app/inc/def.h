#ifndef DEF_H
#define DEF_H

#include "mcal.h"
#include <stdint.h>

typedef uint32_t counter_t;

typedef enum
{
    OP_SUCCESS = 0,
    OP_FAILED = -1
} result_t;

typedef enum
{
    false = 0u,
    true = 1u
} bool_t;

#endif
