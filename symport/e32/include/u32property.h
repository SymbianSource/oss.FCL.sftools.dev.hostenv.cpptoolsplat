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
// \e32\include\u32property.h
// 
//

#ifndef __U32PROPERTY_H__
#define __U32PROPERTY_H__

#include <e32cmn.h>

/**
@internalComponent
*/
class TPropertyInfo
	{
public:
	TUint					iAttr;
	TUint16					iSize;
	RProperty::TType		iType;
	TSecurityPolicy			iReadPolicy;
	TSecurityPolicy			iWritePolicy;
	};

#endif
