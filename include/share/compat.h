/* libFLAC - Free Lossless Audio Codec library
 * Copyright (C) 2012  Xiph.org Foundation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the Xiph.org Foundation nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* This is the prefered location of all CPP hackery to make $random_compiler
 * work like something approaching a C99 (or maybe more accurately GNU99)
 * compiler.
 *
 * It is assumed that this header will be included after "config.h".
 */

#ifndef FLAC__SHARE__COMPAT_H
#define FLAC__SHARE__COMPAT_H

#if defined _WIN32 && !defined __CYGWIN__
/* where MSVC puts unlink() */
# include <io.h>
#else
# include <unistd.h>
#endif

#if defined _MSC_VER || defined __BORLANDC__ || defined __MINGW32__
#include <sys/types.h> /* for off_t */
#define FLAC__off_t __int64 /* use this instead of off_t to fix the 2 GB limit */
#if !defined __MINGW32__
#define fseeko _fseeki64
#define ftello _ftelli64
#else /* MinGW */
#if !defined(HAVE_FSEEKO)
#define fseeko fseeko64
#define ftello ftello64
#endif
#endif
#elif defined __WATCOMC__
#define fseeko _fseeki64
#define ftello _ftelli64
#define FLAC__off_t long long
#elif defined(__EMX__) && !(defined(__INNOTEK_LIBC__) || defined(__KLIBC__))
#define fseeko fseek
#define ftello ftell
#define FLAC__off_t off_t
#elif defined(__MORPHOS__) || defined(__DJGPP__)
#define fseeko fseek
#define ftello ftell
#define FLAC__off_t off_t
#else
#define FLAC__off_t off_t
#endif

#if HAVE_INTTYPES_H
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#endif

#if defined(_MSC_VER)
#define strtoll _strtoi64
#define strtoull _strtoui64
#endif

#if defined(_MSC_VER)
#if _MSC_VER < 1500
/* Visual Studio 2008 has restrict. */
#define restrict __restrict
#endif
#ifndef __cplusplus
#define inline __inline
#endif
#endif

#if defined(__WATCOMC__)
# if (__WATCOMC__ >= 1250)
#  define restrict __restrict
# else
#  define restrict
# endif
#endif

/* adjust for compilers that can't understand using LLU suffix for uint64_t literals */
#ifdef _MSC_VER
#define FLAC__U64L(x) x
#else
#define FLAC__U64L(x) x##LLU
#endif

#if defined _MSC_VER || defined __BORLANDC__ || defined __MINGW32__ || defined __WATCOMC__ || defined __EMX__ || defined __DJGPP__
#define FLAC__STRNCASECMP strnicmp
#else
#define FLAC__STRNCASECMP strncasecmp
#endif

#if defined _MSC_VER || defined __MINGW32__ || defined __EMX__ || defined __WATCOMC__
#include <io.h> /* for _setmode(), chmod() */
#include <fcntl.h> /* for _O_BINARY */
#elif defined __DJGPP__
#include <io.h>
#include <fcntl.h>
#include <unistd.h>
#else
#include <unistd.h> /* for chown(), unlink() */
#endif

#if defined _MSC_VER || defined __BORLANDC__ || defined __MINGW32__
#if defined __BORLANDC__
#include <utime.h> /* for utime() */
#else
#include <sys/utime.h> /* for utime() */
#endif
#else
#include <sys/types.h> /* some flavors of BSD (like OS X) require this to get time_t */
#include <utime.h> /* for utime() */
#endif

#if defined _MSC_VER
#  if _MSC_VER >= 1600
/* Visual Studio 2010 has decent C99 support */
#    include <stdint.h>
#    define PRIu64 "llu"
#    define PRId64 "lld"
#    define PRIx64 "llx"
#  else
#    include <limits.h>
#    ifndef UINT32_MAX
#      define UINT32_MAX _UI32_MAX
#    endif
     typedef unsigned __int64 uint64_t;
     typedef unsigned __int32 uint32_t;
     typedef unsigned __int16 uint16_t;
     typedef unsigned __int8 uint8_t;
     typedef __int64 int64_t;
     typedef __int32 int32_t;
     typedef __int16 int16_t;
     typedef __int8  int8_t;
#    define PRIu64 "I64u"
#    define PRId64 "I64d"
#    define PRIx64 "I64x"
#  endif
#endif /* defined _MSC_VER */

#ifdef _WIN32
/* All char* strings are in UTF-8 format. Added to support Unicode files on Windows */
#include "share/win_utf8_io.h"

#define flac_printf printf_utf8
#define flac_fprintf fprintf_utf8
#define flac_vfprintf vfprintf_utf8
#define flac_fopen fopen_utf8
#define flac_chmod chmod_utf8
#define flac_utime utime_utf8
#define flac_unlink unlink_utf8
#define flac_rename rename_utf8
#define flac_stat _stat64_utf8

#else

#define flac_printf printf
#define flac_fprintf fprintf
#define flac_vfprintf vfprintf
#define flac_fopen fopen
#define flac_chmod chmod
#define flac_utime utime
#define flac_unlink unlink
#define flac_rename rename
#define flac_stat stat

#endif

#ifdef __WATCOMC__
#define flac_stat_s _stati64 /* stat struct */
#undef flac_stat
#define flac_stat   _stati64
#define flac_fstat _fstati64
# ifdef __NT__ /* for win_utf8_io.c : */
# define __stat64 _stati64
# define _wstat64 _wstati64
# define __utimbuf64 utimbuf
# define _wutime64 _wutime
# endif
#elif defined(_WIN32)
#define flac_stat_s __stat64 /* stat struct */
#define flac_fstat _fstat64
#else
#define flac_stat_s stat /* stat struct */
#define flac_fstat fstat
#endif


/* FLAC needs to compile and work correctly on systems with a norrmal ISO C99
 * snprintf as well as Microsoft Visual Studio which has an non-standards
 * conformant snprint_s function.
 *
 * This function wraps the MS version to behave more like the the ISO version.
 */
#ifdef __cplusplus
extern "C" {
#endif
int flac_snprintf(char *str, size_t size, const char *fmt, ...);
#ifdef __cplusplus
};
#endif

#endif /* FLAC__SHARE__COMPAT_H */
