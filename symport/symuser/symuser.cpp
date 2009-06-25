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
// main.cpp
// 
//

#include <stdlib.h>
#include <time.h>

#include "e32base.h"
#include "us_data.h"
#include "e32const.h"

// Globals
TLocalThreadData gThreadData;
int gArgc = 0;
char **gArgv = NULL;

// ***
// LocalThreadData
//
TLocalThreadData* LocalThreadData()
	{
	return &gThreadData;
	}

// ***
// ForceKeyFunction
// This is the key function that forces the class impedimenta to be get exported
//
#if (defined(__TOOLS2__) && defined(__linux__))
EXPORT_C void XLeaveException::ForceKeyFunction()
	{
	}
#endif

// ***
// main
//
EXPORT_C void SymInit(int argc, char *argv[])
	{
	gArgc = argc;
	gArgv = argv;
	srand(time(NULL));
	}

const TInt KIrrelevantInt = 1;

EXPORT_C TThreadId RThread::Id() const{ return KIrrelevantInt;}

EXPORT_C void RThread::SetPriority(TThreadPriority aPriority) const{}

EXPORT_C TThreadPriority RThread::Priority() const{ return EPriorityNormal; }

//end of file

