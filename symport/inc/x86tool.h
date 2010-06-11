// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of the License "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// This is the preinclude file for SYMPORT
// 
//

/**
 @file
 @publishedAll
 @released
*/

// compiler and STLport things first 
#define _STLP_THREADS
#define _STLP_DESIGNATED_DLL

// Pick up relevant macros under __GCC32__, since __GCC32__ is not a valid macro for TOOLS2

#define __NO_CLASS_CONSTS__
#define __NORETURN__  __attribute__ ((noreturn))

#ifdef __GCCV3__
#define __NORETURN_TERMINATOR()
#else
#define __NORETURN_TERMINATOR()		abort()
#endif

#define IMPORT_C
#define IMPORT_D
#ifdef _WIN32
#define EXPORT_C __declspec(dllexport)
#define EXPORT_D __declspec(dllexport)
#else
#define EXPORT_C 
#define EXPORT_D
#endif

#define NONSHARABLE_CLASS(x) class x
#define NONSHARABLE_STRUCT(x) struct x

#ifndef __TOOLS2__
#define __NO_THROW
#else
#include <new>
#ifndef _WIN32
#include <exception>
#endif
#define __NO_THROW throw(std::bad_alloc)
#endif

typedef long long Int64;
typedef unsigned long long Uint64;
#define	I64LIT(x)	x##LL
#define	UI64LIT(x)	x##ULL
#define TEMPLATE_SPECIALIZATION template<>
#define __TText_defined
#ifdef __TOOLS2__
#ifndef _WIN32
#include <stddef.h>  // Linux definition of wchar_t
#endif
#endif

typedef wchar_t __TText;

// Symbian things next ///////////////////////////////////////////////////////

#ifdef __PRODUCT_INCLUDE__
#include __PRODUCT_INCLUDE__
#endif

// Do not use inline new in e32cmn.h
#define __PLACEMENT_NEW_INLINE
#define __PLACEMENT_VEC_NEW_INLINE
// avoid e32tools/filesystem/include/mingw.inl nonsense
#ifdef _WIN32
#define _MINGW_INL
#endif

// Varargs
typedef __builtin_va_list va_list;
#define va_start(v, l) __builtin_va_start(v, l)
#define va_arg(v, l) __builtin_va_arg(v, l)
#define va_end(v) __builtin_va_end(v)
#define VA_LIST va_list
#define _VA_LIST_DEFINED //To deal with stdarg.h
#define __VA_LIST_defined //To deal with e32def.h
#define VA_START(ap,pn) va_start(ap, pn)
#define VA_ARG(ap,type) va_arg(ap,type)
#define VA_END(ap)      va_end(ap)

#ifdef __TOOLS2__
#ifndef _WIN32
	#define asm(EXPR) __asm(".intel_syntax noprefix\n\t" EXPR)
#else
	#define asm __asm
#endif
#endif


// the end of the pre-include

