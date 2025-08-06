#ifndef __khrplatform_h_
#define __khrplatform_h_

/*
** Copyright (c) 2008-2009 The Khronos Group Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
**
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/

/* Platform-specific types and definitions.
 *
 * This file contains the platform-specific types and definitions for the
 * Khronos APIs.
 */

#if defined(__SCITECH_SNAP__) && !defined(KHRONOS_STATIC)
#   define KHRONOS_STATIC 1
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_APICALL
 *-------------------------------------------------------------------------
 * This precedes the return type of the function in the function declaration.
 */
#if defined(KHRONOS_STATIC)
    /* If the preprocessor constant KHRONOS_STATIC is defined, make the
     * header compatible with static linking. */
#   define KHRONOS_APICALL
#elif defined(_WIN32)
#   define KHRONOS_APICALL __declspec(dllimport)
#elif defined (__SYMBIAN32__)
#   if defined(KHRONOS_APICALL_EXPORT)
#       define KHRONOS_APICALL EXPORT_C
#   else
#       define KHRONOS_APICALL IMPORT_C
#   endif
#elif defined(__ANDROID__)
#   define KHRONOS_APICALL __attribute__((visibility("default")))
#else
#   define KHRONOS_APICALL
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_APIENTRY
 *-------------------------------------------------------------------------
 * This follows the parameter list in the function declaration.
 */
#if defined(_WIN32) && !defined(_WIN32_WCE) && !defined(__SCITECH_SNAP__)
    /* Win32 but not WinCE */
#   define KHRONOS_APIENTRY __stdcall
#else
#   define KHRONOS_APIENTRY
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_APIENTRYP
 *-------------------------------------------------------------------------
 * This follows the parameter list in the function declaration.
 */
#define KHRONOS_APIENTRYP KHRONOS_APIENTRY*

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_CALLBACK
 *-------------------------------------------------------------------------
 * This follows the parameter list in the function declaration.
 */
#define KHRONOS_CALLBACK KHRONOS_APIENTRY

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_CALLBACKRYP
 *-------------------------------------------------------------------------
 * This follows the parameter list in the function declaration.
 */
#define KHRONOS_CALLBACKRYP KHRONOS_CALLBACK*

/*-------------------------------------------------------------------------
 * Basic type definitions
 *-----------------------------------------------------------------------*/
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || (defined(__GNUC__) && defined(__STDC_VERSION__))
    /*
     * Using <stdint.h>
     */
#include <stdint.h>
typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;
#elif defined(__VMS ) || defined(__sgi)
    /*
     * Using <inttypes.h>
     */
#include <inttypes.h>
typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;
#elif defined(_WIN32) && !defined(__SCITECH_SNAP__)
    /*
     * Win32
     */
typedef __int32                 khronos_int32_t;
typedef unsigned __int32        khronos_uint32_t;
typedef __int64                 khronos_int64_t;
typedef unsigned __int64        khronos_uint64_t;
#else
    /*
     * Default to standard types
     */
typedef signed int              khronos_int32_t;
typedef unsigned int            khronos_uint32_t;
typedef signed long long int    khronos_int64_t;
typedef unsigned long long int  khronos_uint64_t;
#endif

#if defined(__SCITECH_SNAP__) && !defined(KHRONOS_STATIC)
#   define KHRONOS_STATIC 1
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_APICALL
 *-------------------------------------------------------------------------
 * This precedes the return type of the function in the function declaration.
 */
#if defined(KHRONOS_STATIC)
    /* If the preprocessor constant KHRONOS_STATIC is defined, make the
     * header compatible with static linking. */
#   define KHRONOS_APICALL
#elif defined(_WIN32)
#   define KHRONOS_APICALL __declspec(dllimport)
#elif defined (__SYMBIAN32__)
#   if defined(KHRONOS_APICALL_EXPORT)
#       define KHRONOS_APICALL EXPORT_C
#   else
#       define KHRONOS_APICALL IMPORT_C
#   endif
#elif defined(__ANDROID__)
#   define KHRONOS_APICALL __attribute__((visibility("default")))
#else
#   define KHRONOS_APICALL
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_APIENTRY
 *-------------------------------------------------------------------------
 * This follows the parameter list in the function declaration.
 */
#if defined(_WIN32) && !defined(_WIN32_WCE) && !defined(__SCITECH_SNAP__)
    /* Win32 but not WinCE */
#   define KHRONOS_APIENTRY __stdcall
#else
#   define KHRONOS_APIENTRY
#endif

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_APIENTRYP
 *-------------------------------------------------------------------------
 * This follows the parameter list in the function declaration.
 */
#define KHRONOS_APIENTRYP KHRONOS_APIENTRY*

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_CALLBACK
 *-------------------------------------------------------------------------
 * This follows the parameter list in the function declaration.
 */
#define KHRONOS_CALLBACK KHRONOS_APIENTRY

/*-------------------------------------------------------------------------
 * Definition of KHRONOS_CALLBACKRYP
 *-------------------------------------------------------------------------
 * This follows the parameter list in the function declaration.
 */
#define KHRONOS_CALLBACKRYP KHRONOS_CALLBACK*

/*-------------------------------------------------------------------------
 * Basic type definitions
 *-----------------------------------------------------------------------*/
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || (defined(__GNUC__) && defined(__STDC_VERSION__))
    /*
     * Using <stdint.h>
     */
#include <stdint.h>
typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;
#elif defined(__VMS ) || defined(__sgi)
    /*
     * Using <inttypes.h>
     */
#include <inttypes.h>
typedef int32_t                 khronos_int32_t;
typedef uint32_t                khronos_uint32_t;
typedef int64_t                 khronos_int64_t;
typedef uint64_t                khronos_uint64_t;
#elif defined(_WIN32) && !defined(__SCITECH_SNAP__)
    /*
     * Win32
     */
typedef __int32                 khronos_int32_t;
typedef unsigned __int32        khronos_uint32_t;
typedef __int64                 khronos_int64_t;
typedef unsigned __int64        khronos_uint64_t;
#else
    /*
     * Default to standard types
     */
typedef signed int              khronos_int32_t;
typedef unsigned int            khronos_uint32_t;
typedef signed long long int    khronos_int64_t;
typedef unsigned long long int  khronos_uint64_t;
#endif

#endif /* __khrplatform_h_ */ 