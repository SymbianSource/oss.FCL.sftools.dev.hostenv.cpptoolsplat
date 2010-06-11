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

#ifndef _TESTDLL_H__
#define _TESTDLL_H__

class CTestDll : public CBase
	{
public:
	IMPORT_C CTestDll();
	IMPORT_C ~CTestDll();
public:
	IMPORT_C void LeaveL();
	IMPORT_C TInt ArrayL();
	IMPORT_C void DescriptorL();
	};

#endif
