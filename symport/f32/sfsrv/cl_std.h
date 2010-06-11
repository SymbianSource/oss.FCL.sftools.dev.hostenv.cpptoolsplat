// Copyright (c) 1995-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// f32\sfsrv\cl_std.h
// 
//

#ifndef __TOOLS2__
#include <common.h>		
#include <message.h>
#endif
#include <f32file.h>
#ifndef __TOOLS2__
#include <f32ver.h>
#include <e32svr.h>
#include <F32plugin.h>
#else
const TInt KCountNeeded=KMinTInt;
#endif

enum TClientPanic
	{
	EDriveUnitBadDrive,
	EDriveUnitBadDriveText,
	EFindFileIllegalMode,
	EBadUidIndex,
	ERawDiskCannotClose,
	EFTextIllegalSeekMode,
	ENotImplemented,
	EFManNotActive,
	EFManActive,
	EFManNoActionSet,
	EFManUnknownAction,
	EFManCurrentEntryInvalid,
	EFManBadValueFromObserver,
	EFManBadNames,
	EFManRecursiveRename,
	EDirListError,
	EAddDirBadName,
	ELockLengthZero,
	EUnlockLengthZero,
	EPosNegative,
	ESizeNegative,
	EAttributesIllegal,
	EEntryArrayBadIndex,
	ECDirBadSortType,
	EParsePtrBadDescriptor0,
	EParsePtrBadDescriptor1,
	EParsePtrCAccessError,
	EBadLength,
	EDefaultPathCalled,
	ESetDefaultPathCalled,
	EFindFileIllegalAttribute,
	};
//
enum TClientFault
	{
	ENotifyChangeCancel,
	ESessionClose,
	ESubSessionClose
	};
//
GLREF_C void Panic(TClientPanic aPanic);
GLREF_C void Fault(TClientFault aFault);
//
