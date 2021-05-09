#ifndef _MULIB_MACROS_H
#define _MULIB_MACROS_H

#define UNUSED(__expr) ((void)(__expr))

#define MIN(__x, __y) ((__x) < (__y) ? (__x) : (__y))

#define MAX(__x, __y) ((__x) > (__y) ? (__x) : (__y))

#define CLAMP(__v, __lower, __upper) (MAX(MIN((__v), (__upper)), (__lower)))

#define FLATTEN __attribute__((flatten))

#define ALWAYS_INLINE __attribute__((always_inline))

// Align the nearest _lower_ aligned value
// ex: 8 with align = 8 -> 8
// ex: 9 with align = 8 -> 16
// ex: 7 with align = 8 -> 0
#define ALIGN_DOWN(__addr, __align) ((__addr) & ~((__align)-1))

// Align the nearest _upper_ aligned value
// ex: 8 with align = 8 -> 8
// ex: 9 with align = 8 -> 16
// ex: 7 with align = 8 -> 8
#define ALIGN_UP(__addr, __align) (((__addr) + (__align)-1) & ~((__align)-1))

#define ARRAY_LENGTH(__array) (sizeof(__array) / sizeof(__array[0]))

#define NO_RETURN __attribute__((noreturn))

#define PACKED __attribute__((packed))

#define ALIGNED(__align) __attribute__((aligned(__align)))

#endif /* !_CUTEC_TOKEN_H */
