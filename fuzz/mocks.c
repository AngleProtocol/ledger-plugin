#include <stdint.h>
#include <string.h>
#include "../src/angle_plugin.h"

// mocks
try_context_t *current_context = NULL;
try_context_t *try_context_get(void) {
    return current_context;
}
void os_longjmp(unsigned int exception) {
    longjmp(try_context_get()->jmp_buf, exception);
}

cx_err_t cx_hash_no_throw(cx_hash_t *hash, uint32_t mode, const uint8_t *in, size_t len, uint8_t *out, size_t out_len) { return 0;}

size_t cx_hash_get_size(const cx_hash_t *ctx) { return 32;}

cx_err_t cx_keccak_init_no_throw(cx_sha3_t *hash, size_t size) {return 0;}
/**
 * @brief Subtract two integers represented as byte arrays.
 *
 * @param[out] r   Buffer for the result.
 *
 * @param[in]  a   Pointer to the first integer.
 *
 * @param[in]  b   Pointer to the second integer.
 *
 * @param[in]  len Number of bytes taken into account for the subtraction.
 *
 * @return         Error code:
 *                 - CX_OK on success
 *                 - CX_NOT_UNLOCKED
 *                 - CX_INVALID_PARAMETER_SIZE
 *                 - CX_NOT_LOCKED
 *                 - CX_MEMORY_FULL
 *                 - CX_INVALID_PARAMETER
 */
cx_err_t cx_math_sub_no_throw(uint8_t *r, const uint8_t *a, const uint8_t *b, size_t len) {
    unsigned int va = 0;
    unsigned int vb = 0;
    for (int i=0; i < len; i++) {
        va += (a[i] << (i*8));
        vb += (b[i] << (i*8));
    }

    unsigned int vr = va - vb;

    for (int i=0; i < len; i++) {
        r[i] = (uint8_t)(vr >> (i*8));
    }

    return 0;
}

void *pic(void *linked_address) {
    return linked_address;
}

/*
 * Need functions from bsd/string.h statically for ClusterFuzzLite.
 * '_cups_strlcat()' - Safely concatenate two strings.
 */

size_t                  /* O - Length of string */
strlcat(char       *dst,        /* O - Destination string */
        const char *src,      /* I - Source string */
        size_t     size)      /* I - Size of destination string buffer */
{
    size_t    srclen;         /* Length of source string */
    size_t    dstlen;         /* Length of destination string */


    /*
  * Figure out how much room is left...
     */

    dstlen = strlen(dst);
    size   -= dstlen + 1;

    if (!size)
        return (dstlen);        /* No room, return immediately... */

    /*
  * Figure out how much room is needed...
     */

    srclen = strlen(src);

    /*
  * Copy the appropriate amount...
     */

    if (srclen > size)
        srclen = size;

    memcpy(dst + dstlen, src, srclen);
    dst[dstlen + srclen] = '\0';

    return (dstlen + srclen);
}
/*
 * '_cups_strlcpy()' - Safely copy two strings.
 */

size_t                  /* O - Length of string */
strlcpy(char       *dst,        /* O - Destination string */
        const char *src,      /* I - Source string */
        size_t      size)     /* I - Size of destination string buffer */
{
    size_t    srclen;         /* Length of source string */


    /*
  * Figure out how much room is needed...
     */

    size --;

    srclen = strlen(src);

    /*
  * Copy the appropriate amount...
     */

    if (srclen > size)
        srclen = size;

    memcpy(dst, src, srclen);
    dst[srclen] = '\0';

    return (srclen);
}
