#ifndef WAD18_COMMON_H
#define WAD18_COMMON_H

#include <stdint.h>
#include <string.h>
#include <math.h>

typedef int64_t wad18_t;

/* PURE WAD18: 10^18 scale for exact fixed-point arithmetic */
#define WAD18_SCALE       1000000000000000000LL
#define WAD18_MAX         9223372036854775807LL
#define WAD18_MIN         -9223372036854775807LL

/* Exact integer arithmetic - no floating point */
#define WAD18_MUL_SCALED(a, b) (((a) * (b)) / WAD18_SCALE)
#define WAD18_DIV_SCALED(a, b) (((a) * WAD18_SCALE) / (b))

/* Helper functions - purely integer, decidable by inspection */
static inline wad18_t wad18_clamp(wad18_t val, wad18_t min, wad18_t max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

static inline wad18_t wad18_abs(wad18_t val) {
    return (val < 0) ? -val : val;
}

static inline wad18_t wad18_max(wad18_t a, wad18_t b) {
    return (a > b) ? a : b;
}

static inline wad18_t wad18_min(wad18_t a, wad18_t b) {
    return (a < b) ? a : b;
}

#endif
