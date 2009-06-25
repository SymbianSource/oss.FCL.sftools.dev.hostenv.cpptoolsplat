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

RTest test(_L("Simple tests"));

EXPORT_C void HashL()
	{
	TInt in = 0x12345678;
	TInt out = 0;
	out = DefaultHash::Integer(in);
	test(in != out);
	}

void doMainL()
	{
	test.Start(_L("Leaving"));
	TRAPD(err, TestStatic::LeaveL());
	RDebug::Print(_L("... Leave code: %d\n"), err);
	test(err != KErrNone);

	test.Next(_L("Arrays"));
	test(TestStatic::ArrayL() == 3);

	test.Next(_L("Descriptors"));
	TestStatic::DescriptorL();

	test.Next(_L("Hash"));
	HashL();

	test.End();
	}

// Note - no E32Main!
int main(int argc, char *argv[])
	{
	RDebug::Print(_L("Called from main\n"));

	__UHEAP_MARK;
	CTrapCleanup* theCleanup = CTrapCleanup::New();
	TRAPD(ret, doMainL());
	test(ret == KErrNone);
	test.Close();
	delete theCleanup;
	__UHEAP_MARKEND;
	return ret;
	}

