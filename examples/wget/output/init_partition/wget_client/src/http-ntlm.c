/* NTLM code.
   Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2015 Free
   Software Foundation, Inc.
   Contributed by Daniel Stenberg.

This file is part of GNU Wget.

GNU Wget is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

GNU Wget is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Wget.  If not, see <http://www.gnu.org/licenses/>.

Additional permission under GNU GPL version 3 section 7

If you modify this program, or any covered work, by linking or
combining it with the OpenSSL project's OpenSSL library (or a
modified version of that library), containing parts covered by the
terms of the OpenSSL or SSLeay licenses, the Free Software Foundation
grants you additional permission to convey the resulting work.
Corresponding Source for a non-source form of such a combination
shall include the source code for the parts of OpenSSL used as well
as that of the covered work.  */

#include "wget.h"

/* NTLM details:

   http://davenport.sourceforge.net/ntlm.html
   http://www.innovation.ch/java/ntlm.html

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "http-ntlm.h"

#include "wget_rpc_wrapper.h"

#ifdef HAVE_NETTLE
# include <nettle/md4.h>
# include <nettle/des.h>
#else
# include <openssl/des.h>
# include <openssl/md4.h>
# include <openssl/opensslv.h>

# if OPENSSL_VERSION_NUMBER < 0x00907001L
#  define DES_key_schedule des_key_schedule
#  define DES_cblock des_cblock
#  define DES_set_odd_parity des_set_odd_parity
#  define DES_set_key des_set_key
#  define DES_ecb_encrypt des_ecb_encrypt

/* This is how things were done in the old days */
#  define DESKEY(x) x
#  define DESKEYARG(x) x
# else
/* Modern version */
#  define DESKEYARG(x) *x
#  define DESKEY(x) &x
# endif

#endif

/* Define this to make the type-3 message include the NT response message */
#define USE_NTRESPONSES 1


/* Flag bits definitions available at on
   http://davenport.sourceforge.net/ntlm.html */

#define NTLMFLAG_NEGOTIATE_OEM                   (1<<1)
#define NTLMFLAG_NEGOTIATE_NTLM_KEY              (1<<9)

/*
  (*) = A "security buffer" is a triplet consisting of two shorts and one
  long:

  1. a 'short' containing the length of the buffer in bytes
  2. a 'short' containing the allocated space for the buffer in bytes
  3. a 'long' containing the offset to the start of the buffer from the
     beginning of the NTLM message, in bytes.
*/

/* return true on success, false otherwise */
bool ntlm_input(struct ntlmdata *ntlm, const char *header)
{
  if (0 != strncmp(header, "NTLM", 4))
    return 0;
  header += 4;
  while ((*header) && c_isspace(*header))
    header++;

  if (*header)
  {
    ssize_t size;
    char *buffer = (char *) alloca(strlen(header));
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Received a type-2 NTLM message.\n");
      }
    }
    while (0);
    size = base64_decode(header, buffer);
    if (size < 0)
      return 0;
    ntlm->state = NTLMSTATE_TYPE2;
    if (size >= 48)
      memcpy(ntlm->nonce, &buffer[24], 8);
  }
  else
  {
    if (ntlm->state >= NTLMSTATE_TYPE1)
    {
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Unexpected empty NTLM message.\n");
        }
      }
      while (0);
      return 0;
    }
    do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Empty NTLM message, starting transaction.\n");
      }
    }
    while (0);
    ntlm->state = NTLMSTATE_TYPE1;
  }
  return 1;
}



/*
 * Turns a 56 bit key into the 64 bit, odd parity key and sets the key.  The
 * key schedule ks is also set.
 */
#ifdef HAVE_NETTLE
static void
setup_des_key(unsigned char *key_56,
              struct des_ctx *des)
{
  unsigned char key[8];

  key[0] = key_56[0];
  key[1] = ((key_56[0] << 7) & 0xFF) | (key_56[1] >> 1);
  key[2] = ((key_56[1] << 6) & 0xFF) | (key_56[2] >> 2);
  key[3] = ((key_56[2] << 5) & 0xFF) | (key_56[3] >> 3);
  key[4] = ((key_56[3] << 4) & 0xFF) | (key_56[4] >> 4);
  key[5] = ((key_56[4] << 3) & 0xFF) | (key_56[5] >> 5);
  key[6] = ((key_56[5] << 2) & 0xFF) | (key_56[6] >> 6);
  key[7] =  (key_56[6] << 1) & 0xFF;

  nettle_des_set_key(des, key);
}
#else
static void
setup_des_key(unsigned char *key_56,
              DES_key_schedule DESKEYARG(ks))
{
  DES_cblock key;

  key[0] = key_56[0];
  key[1] = ((key_56[0] << 7) & 0xFF) | (key_56[1] >> 1);
  key[2] = ((key_56[1] << 6) & 0xFF) | (key_56[2] >> 2);
  key[3] = ((key_56[2] << 5) & 0xFF) | (key_56[3] >> 3);
  key[4] = ((key_56[3] << 4) & 0xFF) | (key_56[4] >> 4);
  key[5] = ((key_56[4] << 3) & 0xFF) | (key_56[5] >> 5);
  key[6] = ((key_56[5] << 2) & 0xFF) | (key_56[6] >> 6);
  key[7] =  (key_56[6] << 1) & 0xFF;

  DES_set_odd_parity(&key);
  DES_set_key(&key, ks);
}
#endif

 /*
  * takes a 21 byte array and treats it as 3 56-bit DES keys. The
  * 8 byte plaintext is encrypted with each key and the resulting 24
  * bytes are stored in the results array.
  */
static void
calc_resp(unsigned char *keys, unsigned char *plaintext, unsigned char *results)
{
#ifdef HAVE_NETTLE
  struct des_ctx des;

  setup_des_key(keys, &des);
  nettle_des_encrypt(&des, 8, results, plaintext);

  setup_des_key(keys + 7, &des);
  nettle_des_encrypt(&des, 8, results + 8, plaintext);

  setup_des_key(keys + 14, &des);
  nettle_des_encrypt(&des, 8, results + 16, plaintext);
#else
  DES_key_schedule ks;

  setup_des_key(keys, DESKEY(ks));
  DES_ecb_encrypt((DES_cblock*) plaintext, (DES_cblock*) results,
                  DESKEY(ks), DES_ENCRYPT);

  setup_des_key(keys+7, DESKEY(ks));
  DES_ecb_encrypt((DES_cblock*) plaintext, (DES_cblock*) (results+8),
                  DESKEY(ks), DES_ENCRYPT);

  setup_des_key(keys+14, DESKEY(ks));
  DES_ecb_encrypt((DES_cblock*) plaintext, (DES_cblock*) (results+16),
                  DESKEY(ks), DES_ENCRYPT);
#endif
}

/*
 * Set up lanmanager and nt hashed passwords
 */
static void
mkhash(const char *password,
       unsigned char *nonce,    /* 8 bytes */
       unsigned char *lmresp    /* must fit 0x18 bytes */
#ifdef USE_NTRESPONSES
       , unsigned char *ntresp  /* must fit 0x18 bytes */
#endif
  )
{
  unsigned char lmbuffer[21];
#ifdef USE_NTRESPONSES
  unsigned char ntbuffer[21];
#endif
  unsigned char *pw;
  static const unsigned char magic[] = {
    0x4B, 0x47, 0x53, 0x21, 0x40, 0x23, 0x24, 0x25
  };
  size_t i, len = strlen(password);

  /* make it fit at least 14 bytes */
  pw = (unsigned char *) alloca (len < 7 ? 14 : len * 2);

  if (len > 14)
    len = 14;

  for (i=0; i<len; i++)
    pw[i] = (unsigned char) c_toupper (password[i]);

  for (; i<14; i++)
    pw[i] = 0;

  {
    /* create LanManager hashed password */
#ifdef HAVE_NETTLE
    struct des_ctx des;

    setup_des_key(pw, &des);
    nettle_des_encrypt(&des, 8, lmbuffer, magic);

    setup_des_key(pw + 7, &des);
    nettle_des_encrypt(&des, 8, lmbuffer + 8, magic);
#else
    DES_key_schedule ks;

    setup_des_key(pw, DESKEY(ks));
    DES_ecb_encrypt((DES_cblock *)magic, (DES_cblock *)lmbuffer,
                    DESKEY(ks), DES_ENCRYPT);

    setup_des_key(pw+7, DESKEY(ks));
    DES_ecb_encrypt((DES_cblock *)magic, (DES_cblock *)(lmbuffer+8),
                    DESKEY(ks), DES_ENCRYPT);
#endif

    memset(lmbuffer+16, 0, 5);
  }
  /* create LM responses */
  calc_resp(lmbuffer, nonce, lmresp);

#ifdef USE_NTRESPONSES
  {
#ifdef HAVE_NETTLE
    struct md4_ctx MD4;
#else
    MD4_CTX MD4;
#endif

    len = strlen(password);

    for (i=0; i<len; i++) {
      pw[2*i]   = (unsigned char) password[i];
      pw[2*i+1] = 0;
    }

#ifdef HAVE_NETTLE
    nettle_md4_init(&MD4);
    nettle_md4_update(&MD4, (unsigned) (2 * len), pw);
    nettle_md4_digest(&MD4, MD4_DIGEST_SIZE, ntbuffer);
#else
    /* create NT hashed password */
    MD4_Init(&MD4);
    MD4_Update(&MD4, pw, 2*len);
    MD4_Final(ntbuffer, &MD4);
#endif

    memset(ntbuffer+16, 0, 5);
  }

  calc_resp(ntbuffer, nonce, ntresp);
#endif
}

#define SHORTPAIR(x) (char) ((x) & 0xff), (char) ((x) >> 8)
#define LONGQUARTET(x) ((x) & 0xff), (((x) >> 8)&0xff), \
  (((x) >>16)&0xff), ((x)>>24)

/* this is for creating ntlm header output */
char *ntlm_output(struct ntlmdata *ntlm, const char *user, const char *passwd, bool *ready)
{
  const char *domain = "";
  const char *host = "";
  size_t domlen = strlen(domain);
  size_t hostlen = strlen(host);
  size_t hostoff;
  size_t domoff;
  size_t size;
  char *base64;
  char ntlmbuf[256];
  char *output = 0;
  *ready = 0;
  if (!user)
    user = "";
  if (!passwd)
    passwd = "";
  switch (ntlm->state)
  {
    case NTLMSTATE_TYPE1:

    case NTLMSTATE_NONE:

    case NTLMSTATE_LAST:
      hostoff = 32;
      domoff = hostoff + hostlen;
      do
    {
      if (__builtin_expect(get_opt_debug_wrapper(), 0))
      {
        debug_logprintf("Creating a type-1 NTLM message.\n");
      }
    }
    while (0);
      snprintf(ntlmbuf, sizeof(ntlmbuf), "NTLMSSP%c\x01%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%s%s", 0, 0, 0, 0, ((1 << 1) | (1 << 9)) & 0xff, (((1 << 1) | (1 << 9)) >> 8) & 0xff, (((1 << 1) | (1 << 9)) >> 16) & 0xff, ((1 << 1) | (1 << 9)) >> 24, (char) (domlen & 0xff), (char) (domlen >> 8), (char) (domlen & 0xff), (char) (domlen >> 8), (char) (domoff & 0xff), (char) (domoff >> 8), 0, 0, (char) (hostlen & 0xff), (char) (hostlen >> 8), (char) (hostlen & 0xff), (char) (hostlen >> 8), (char) (hostoff & 0xff), (char) (hostoff >> 8), 0, 0, host, domain);
      size = (32 + hostlen) + domlen;
      base64 = (char *) alloca((4 * ((size + 2) / 3)) + 1);
      base64_encode(ntlmbuf, size, base64);
      output = concat_strings("NTLM ", base64, (char *) 0);
      break;

    case NTLMSTATE_TYPE2:
    {
      size_t lmrespoff;
      size_t ntrespoff;
      size_t useroff;
      unsigned char lmresp[0x18];
      unsigned char ntresp[0x18];
      const char *usr;
      size_t userlen;
      do
      {
        if (__builtin_expect(get_opt_debug_wrapper(), 0))
        {
          debug_logprintf("Creating a type-3 NTLM message.\n");
        }
      }
      while (0);
      usr = strchr(user, '\\');
      if (!usr)
        usr = strchr(user, '/');
      if (usr)
      {
        domain = user;
        domlen = (size_t) (usr - domain);
        usr++;
      }
      else
        usr = user;
      userlen = strlen(usr);
      mkhash(passwd, &ntlm->nonce[0], lmresp, ntresp);
      domoff = 64;
      useroff = domoff + domlen;
      hostoff = useroff + userlen;
      lmrespoff = hostoff + hostlen;
      ntrespoff = lmrespoff + 0x18;
      snprintf(ntlmbuf, sizeof(ntlmbuf), "NTLMSSP%c\x03%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\xff\xff%c%c\x01\x82%c%c", 0, 0, 0, 0, (char) (0x18 & 0xff), (char) (0x18 >> 8), (char) (0x18 & 0xff), (char) (0x18 >> 8), (char) (lmrespoff & 0xff), (char) (lmrespoff >> 8), 0x0, 0x0, (char) (0x18 & 0xff), (char) (0x18 >> 8), (char) (0x18 & 0xff), (char) (0x18 >> 8), (char) (ntrespoff & 0xff), (char) (ntrespoff >> 8), 0x0, 0x0, (char) (domlen & 0xff), (char) (domlen >> 8), (char) (domlen & 0xff), (char) (domlen >> 8), (char) (domoff & 0xff), (char) (domoff >> 8), 0x0, 0x0, (char) (userlen & 0xff), (char) (userlen >> 8), (char) (userlen & 0xff), (char) (userlen >> 8), (char) (useroff & 0xff), (char) (useroff >> 8), 0x0, 0x0, (char) (hostlen & 0xff), (char) (hostlen >> 8), (char) (hostlen & 0xff), (char) (hostlen >> 8), (char) (hostoff & 0xff), (char) (hostoff >> 8), 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0);
      size = 64;
      ntlmbuf[62] = (ntlmbuf[63] = 0);
      if (((size + userlen) + domlen) >= (sizeof(ntlmbuf)))
        return 0;
      memcpy(&ntlmbuf[size], domain, domlen);
      size += domlen;
      memcpy(&ntlmbuf[size], usr, userlen);
      size += userlen;
      if (size < ((sizeof(ntlmbuf)) - 0x18))
      {
        memcpy(&ntlmbuf[size], lmresp, 0x18);
        size += 0x18;
      }
      if (size < ((sizeof(ntlmbuf)) - 0x18))
      {
        memcpy(&ntlmbuf[size], ntresp, 0x18);
        size += 0x18;
      }
      ntlmbuf[56] = (char) (size & 0xff);
      ntlmbuf[57] = (char) (size >> 8);
      base64 = (char *) alloca((4 * ((size + 2) / 3)) + 1);
      base64_encode(ntlmbuf, size, base64);
      output = concat_strings("NTLM ", base64, (char *) 0);
      ntlm->state = NTLMSTATE_TYPE3;
      *ready = 1;
    }
      break;

    case NTLMSTATE_TYPE3:
      *ready = 1;
      output = 0;
      break;

  }

  return output;
}


