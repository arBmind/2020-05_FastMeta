/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#if !defined(FOLLY_MOBILE)
#if defined(__ANDROID__) || \
    (defined(__APPLE__) &&  \
     (TARGET_IPHONE_SIMULATOR || TARGET_OS_SIMULATOR || TARGET_OS_IPHONE))
#define FOLLY_MOBILE 1
#else
#define FOLLY_MOBILE 0
#endif
#endif // FOLLY_MOBILE

//#define FOLLY_HAVE_PTHREAD 0
//#define FOLLY_HAVE_PTHREAD_ATFORK 0

//#define FOLLY_HAVE_LIBGFLAGS 0
//#define FOLLY_UNUSUAL_GFLAGS_NAMESPACE 0
//#define FOLLY_GFLAGS_NAMESPACE gflags

//#define FOLLY_HAVE_LIBGLOG 0

//#define FOLLY_USE_JEMALLOC 0
//#define FOLLY_USE_LIBSTDCPP 0

#if __has_include(<features.h>)
#include <features.h>
#endif

//#define FOLLY_HAVE_MEMRCHR 1
//#define FOLLY_HAVE_ACCEPT4 0
//#define FOLLY_HAVE_GETRANDOM 0
//#define FOLLY_HAVE_PREADV 0
//#define FOLLY_HAVE_PWRITEV 0
//#define FOLLY_HAVE_CLOCK_GETTIME 0
//#define FOLLY_HAVE_PIPE2 0
//#define FOLLY_HAVE_SENDMMSG 0
//#define FOLLY_HAVE_RECVMMSG 0
//#define FOLLY_HAVE_OPENSSL_ASN1_TIME_DIFF 0

//#define FOLLY_HAVE_IFUNC 1
//#define FOLLY_HAVE_STD__IS_TRIVIALLY_COPYABLE 1
//#define FOLLY_HAVE_UNALIGNED_ACCESS 0
//#define FOLLY_HAVE_VLA 0
//#define FOLLY_HAVE_WEAK_SYMBOLS 0
//#define FOLLY_HAVE_LINUX_VDSO 0
//#define FOLLY_HAVE_MALLOC_USABLE_SIZE 1
//#define FOLLY_HAVE_INT128_T 0
//#define FOLLY_SUPPLY_MISSING_INT128_TRAITS 0
//#define FOLLY_HAVE_WCHAR_SUPPORT 1
//#define FOLLY_HAVE_EXTRANDOM_SFMT19937 0
//#define FOLLY_USE_LIBCPP 0
//#define HAVE_VSNPRINTF_ERRORS 1

//#define FOLLY_USE_SYMBOLIZER 0
//#define FOLLY_DEMANGLE_MAX_SYMBOL_SIZE 1024

//#define FOLLY_HAVE_SHADOW_LOCAL_WARNINGS 1

//#define FOLLY_HAVE_LIBLZ4 0
//#define FOLLY_HAVE_LIBLZMA 0
//#define FOLLY_HAVE_LIBSNAPPY 0
//#define FOLLY_HAVE_LIBZ 0
//#define FOLLY_HAVE_LIBZSTD 0
//#define FOLLY_HAVE_LIBBZ2 0

//#define FOLLY_LIBRARY_SANITIZE_ADDRESS 0

//#define FOLLY_SUPPORT_SHARED_LIBRARY 0
