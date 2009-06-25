// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

#include "e32base.h"

extern int gArgc;
extern char **gArgv;

// The exec class implements low level functionality needed by the TOOLS2 port
// These are normally kernel executive calls
class Exec
	{
	static TInt TimeNow(TInt64&, TInt&);
	static TInt TimeNowSecure(TInt64&, TInt&);
	static void DebugPrint(TAny* aPtr, TInt);
	static TUint32 MathRandom();
	static void ProcessCommandLine(TInt, TDes8&);
	static TInt ProcessCommandLineLength(TInt);
	static TUint TickCount();
	static TUint32 NTickCount();
	static void After(TInt, TRequestStatus&);
	static void AfterHighRes(TInt, TRequestStatus&);
	};

// ***
// after
//

void Exec::After(TInt aInterval, TRequestStatus&)
	{
#ifdef _WIN32
	Sleep(aInterval/1000);
#else
	sleep(aInterval/1000000);
#endif
	}

void Exec::AfterHighRes(TInt aInterval, TRequestStatus&)
	{
#ifdef _WIN32
	Sleep(aInterval/1000);
#else
	sleep(aInterval/1000000);
#endif
	}

// ***
// tick
//
TUint Exec::TickCount()
	{
	return (TUint)clock();
	}

TUint32 Exec::NTickCount()
	{
	return (TUint32)clock();
	}

// ***
// command line
//
void Exec::ProcessCommandLine(TInt, TDes8& aCmd)
	{
	TPtr16 aCommand16((TUint16*)aCmd.Ptr(),aCmd.MaxLength()>>1);
	for(TInt i = 1; i < gArgc; i++)
		{
		if (i > 0)
			aCommand16.Append(' ');

		for(TUint s = 0; s < strlen(gArgv[i]); s++)
			{
			aCommand16.Append(gArgv[i][s]);
			}
		}
	aCmd.SetLength(aCommand16.Length()<<1);
	}

TInt Exec::ProcessCommandLineLength(TInt)
	{
	TInt len = 0;
	for(TInt i = 1; i < gArgc; i++)
		{
		if (i > 0)
			len++;
		len += strlen(gArgv[i]);
		}
	return len;
	}

// ***
// debug
//
void Exec::DebugPrint(TAny* aPtr, TInt)
	{
	TPtr8* p = (TPtr8*)aPtr;
	for(TInt i = 0; i < p->Length(); i++)
		putchar((*p)[i]);

	}

// ***
// rand
//
TUint32 Exec::MathRandom()
	{
	return rand();
	}

// ***
// time
//
TInt Exec::TimeNow(TInt64& aUniversalTime, TInt& aUniversalTimeOffset)
	{
	aUniversalTimeOffset = 0;

	time_t t = time(NULL);
    struct tm *tm = gmtime(&t);
    TTime tt = TDateTime(1900 + tm->tm_year, TMonth(tm->tm_mon), tm->tm_mday - 1, tm->tm_hour, tm->tm_min, tm->tm_sec, 0);
    aUniversalTime = tt.Int64();

    return KErrNone;
	}

TInt Exec::TimeNowSecure(TInt64& aUniversalTime, TInt& aUniversalTimeOffset)
	{
	return Exec::TimeNow(aUniversalTime, aUniversalTimeOffset);
	}
