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

#include <e32std.h>

const TInt KMaxErrorStringLength = 100;
static const TText8 KErrorMessages[][KMaxErrorStringLength+1]=
	{
#define ERRORSTRING(s) #s
#include "errorstrings.h"
#undef ERRORSTRING
	};


void TranslateError(TInt aErrorCode, TDes8& aTarget)
	{
	aErrorCode = - aErrorCode;
	if ( aErrorCode > 46 )
		{
		_LIT8(KNoTranslationAvailable, "");
		aTarget.Copy(KNoTranslationAvailable);
		aTarget.Append(' ');
		aTarget.Append('-');
		aTarget.AppendNum(aErrorCode);
		}
	else if ( aErrorCode >= 0 )
		{
		aTarget.Copy(KErrorMessages[aErrorCode]);
		}
	else
		{
		_LIT8(KNoTranslationAvailable, "");
		aTarget.Copy(KNoTranslationAvailable);
		aTarget.Append(' ');
		aTarget.AppendNum(aErrorCode);
		}
	}

// end of file
