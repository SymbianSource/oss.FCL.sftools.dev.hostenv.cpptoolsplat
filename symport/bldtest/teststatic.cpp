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

#include <e32base.h>
#include <e32std.h>
#include <e32debug.h>

#include "teststatic.h"

EXPORT_C void TestStatic::LeaveL()
	{
	User::Leave(-666);
	}

EXPORT_C TInt TestStatic::ArrayL()
	{
	RArray<TInt> array;
	array.Append(1);
	array.Append(2);
	array.Append(3);

	TInt count = array.Count();
	array.Close();
	return count;
	}

EXPORT_C void TestStatic::DescriptorL()
	{
	TBuf<256> des1;
	TBuf<256> des2(_L("... TBuf descriptors are okay\n"));
	des1.Format(_L("%S"), &des2);	
	RDebug::Print(des1);
	
	HBufC8* hbuf1 = HBufC8::NewLC(76);
	hbuf1->Des().Copy(_L8("... Heap descriptors are okay\n"));
	des1.Copy(*hbuf1);
	RDebug::Print(des1);

	des1.Zero();
	des1.AppendNum(TReal(3.14), TRealFormat());
	RDebug::Print(_L("... The value of PI=%S\n"), &des1);
	
	des1.Zero();
	des1.AppendFormat(_L("... More accurate value of PI=%e\n"), TReal(3.14159));
	RDebug::Print(des1);

	CleanupStack::PopAndDestroy(hbuf1);
	}
