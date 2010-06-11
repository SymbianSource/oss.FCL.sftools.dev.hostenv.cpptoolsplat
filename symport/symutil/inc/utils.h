// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#include <e32std.h>
#include <time.h>

/**
 * Creates a NULL terminated string from aSource and stores into aTarget.
 */
extern const TInt CreateNullTerminatedString(TDes8& aTarget, const TDesC8& aSource);

/**
 * Creates a NULL terminated string from aSource and stores into aTarget.
 * The 16 to 8 bit conversion is performed via TDes16::Collapse
 */
extern const TInt CreateNullTerminatedString(TDes8& aTarget, const TDesC& aSource);

/**
 * Creates a NULL terminated string from aSource and stores into aTarget.
 */
extern const TInt CreateNullTerminatedString(TDes& aTarget, const TDesC& aSource);


extern const void TTime_to_time_t(TTime& aSource, time_t& aTarget) ;

extern const void TTime_to_tm(TTime& aTime, struct tm& aTarget ) ;

extern const void time_t_to_TTime(time_t& aSource, TTime& aTarget) ;
