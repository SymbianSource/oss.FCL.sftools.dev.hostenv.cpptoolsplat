// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32debug.h>
#include <e32base.h>
#include <e32hashtab.h>
#include <e32test.h>

#include "teststatic.h"
#include "testdll.h"

RTest test(_L("Simple tests"));

EXPORT_C void HashL()
	{
	TInt in = 0x12345678;
	TInt out = 0;
	out = DefaultHash::Integer(in);
	test(in != out);
	}

void DefectsL()
	{
	// DEF132826: symport User:Alloc family error checking
	test(User::Alloc(-10) == NULL);
	}

void doMainL()
	{
	CTestDll dll;

	test.Start(_L("Leaving"));
	TRAPD(err, TestStatic::LeaveL());
	RDebug::Print(_L("... Leave code from lib: %d\n"), err);
	TRAP(err, dll.LeaveL());
	RDebug::Print(_L("... Leave code from dll: %d\n"), err);
	test(err != KErrNone);

	test.Next(_L("Arrays"));
	test(TestStatic::ArrayL() == 3);
	test(dll.ArrayL() == 3);

	test.Next(_L("Descriptors"));
	TestStatic::DescriptorL();
	dll.DescriptorL();

	test.Next(_L("Hash"));
	HashL();

	test.Next(_L("Defects"));
	DefectsL();

	test.End();
	}

int E32Main()
	{
	RDebug::Print(_L("Called from E32Main\n"));

	__UHEAP_MARK;
	CTrapCleanup* theCleanup = CTrapCleanup::New();
	TRAPD(ret, doMainL());
	test(ret == KErrNone);
	test.Close();
	delete theCleanup;
	__UHEAP_MARKEND;
	return ret;
	}
