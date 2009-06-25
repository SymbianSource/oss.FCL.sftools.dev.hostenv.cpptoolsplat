// Copyright (c) 1995-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of the License "Symbian Foundation License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include "e32std.h"

EXPORT_C TAny* operator new(TUint aSize) __NO_THROW
//
// The global new operator.
//
	{
	return User::Alloc(aSize);
	}

EXPORT_C TAny* operator new[](TUint aSize) __NO_THROW
    {
    return User::Alloc(aSize);
    }

EXPORT_C TAny* operator new(TUint aSize, TUint aExtraSize) __NO_THROW
//
// Allocate the requested size plus the extra.
//
	{
	return  User::Alloc(aSize + aExtraSize);
	}

EXPORT_C void operator delete(TAny* aPtr) __NO_THROW
//
// The replacement delete operator.
//
	{
	User::Free(aPtr);
	}

EXPORT_C void operator delete[](TAny* aPtr) __NO_THROW
    {
    User::Free(aPtr);
    }
