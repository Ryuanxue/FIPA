
/*
 * An internal implementation, based on Alexander Peslyak's
 * public domain implementation:
 * http://openwall.info/wiki/people/solar/software/public-domain-source-code/md5
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_md5.h>


static const u_char *ngx_md5_body(ngx_md5_t *ctx, const u_char *data,
    size_t size);








/*
 * The basic MD5 functions.
 *
 * F and G are optimized compared to their RFC 1321 definitions for
 * architectures that lack an AND-NOT instruction, just like in
 * Colin Plumb's implementation.
 */

#define F(x, y, z)  ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)  ((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z)  ((x) ^ (y) ^ (z))
#define I(x, y, z)  ((y) ^ ((x) | ~(z)))

/*
 * The MD5 transformation for all four rounds.
 */

#define STEP(f, a, b, c, d, x, t, s)                                          \
    (a) += f((b), (c), (d)) + (x) + (t);                                      \
    (a) = (((a) << (s)) | (((a) & 0xffffffff) >> (32 - (s))));                \
    (a) += (b)

/*
 * SET() reads 4 input bytes in little-endian byte order and stores them
 * in a properly aligned word in host byte order.
 *
 * The check for little-endian architectures that tolerate unaligned
 * memory accesses is just an optimization.  Nothing will break if it
 * does not work.
 */

#if (NGX_HAVE_LITTLE_ENDIAN && NGX_HAVE_NONALIGNED)

#define SET(n)      (*(uint32_t *) &p[n * 4])
#define GET(n)      (*(uint32_t *) &p[n * 4])

#else

#define SET(n)                                                                \
    (block[n] =                                                               \
    (uint32_t) p[n * 4] |                                                     \
    ((uint32_t) p[n * 4 + 1] << 8) |                                          \
    ((uint32_t) p[n * 4 + 2] << 16) |                                         \
    ((uint32_t) p[n * 4 + 3] << 24))

#define GET(n)      block[n]

#endif


/*
 * This processes one or more 64-byte data blocks, but does not update
 * the bit counters.  There are no alignment requirements.
 */

