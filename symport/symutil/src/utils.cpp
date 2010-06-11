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

#include <utils.h>
#include <time.h>
/**
 * Creates a NULL terminated string from aSource and stores into aTarget.
 */
const TInt CreateNullTerminatedString(TDes8& aTarget, const TDesC8& aSource)
	{
	aTarget.Append(aSource);
	aTarget.Append('\0');
	return KErrNone;
	}

/**
 * Creates a NULL terminated string from aSource and stores into aTarget.
 * The 16 to 8 bit conversion is performed via TDes16::Collapse
 */
const TInt CreateNullTerminatedString(TDes8& aTarget, const TDesC& aSource)
	{
	aTarget.Copy(aSource);
	aTarget.Append('\0');
	return KErrNone;
	}

/**
 * Creates a NULL terminated string from aSource and stores into aTarget.
 */
const TInt CreateNullTerminatedString(TDes& aTarget, const TDesC& aSource)
	{
	aTarget.Append(aSource);
	aTarget.Append('\0');
	return KErrNone;
	}

const void TTime_to_time_t(TTime& aSource, time_t& aTarget)
	{
	TDateTime dateTime = aSource.DateTime();
	struct tm stm;

	stm.tm_year = dateTime.Year() - 1900;
	stm.tm_mon  = (int)dateTime.Month();
	stm.tm_mday = 1 + dateTime.Day();
	stm.tm_hour = dateTime.Hour();
	stm.tm_min = dateTime.Minute();
	stm.tm_sec = dateTime.Second();

	aTarget = mktime(&stm);
	}

const void TTime_to_tm(TTime& aSource, tm& aTarget )
	{
	TDateTime dateTime = aSource.DateTime();
	aTarget.tm_year = dateTime.Year() - 1900;
	aTarget.tm_mon  = (int)dateTime.Month();
	aTarget.tm_mday = 1 + dateTime.Day();
	aTarget.tm_hour = dateTime.Hour();
	aTarget.tm_min = dateTime.Minute();
	aTarget.tm_sec = dateTime.Second();
	}

const void time_t_to_TTime(time_t& aSource, TTime& aTarget)
	{
	struct tm* ptm = localtime( &aSource );

	TDateTime datetime (
			ptm->tm_year +1900,
			(TMonth)(EJanuary + ptm->tm_mon),
			ptm->tm_mday - 1,
			ptm->tm_hour,
			ptm->tm_min,
			ptm->tm_sec, 0
			);
	TTime tmp(datetime);
	aTarget = tmp;
	}

//end of file
