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
// e32\euser\maths\um_dtor.cpp
// 
//

#include "um_std.h"


void TLex8::Scndig(TInt &aSig,TInt &aExp,TInt64 &aDl)
//
// Scans a decimal digit field and accumulates the value to a TInt64 at aDl
// Used before decimal point - do not drop trailing zeros.
//
	{

	FOREVER
		{
		if (iNext>=iEnd)
			break;
		TChar c=Peek();
		if (!c.IsDigit())
			break;
		else
			c=Get();
		if (aSig<KMaxPrecision+2)		
			{
			aDl *= 10;				// Multiply accumulator by 10
			aDl+=((TUint)c)-'0';	// Add current digit
			aSig++;
			}
		else
			aExp++;
		}														  
	}

void TLex8::ScndigAfterPoint(TInt &aSig,TInt64 &aDl)
//
// Scans a decimal digit field and accumulates the value to a TInt64 at aDl
// Used after decimal point - drops trailing zeros.
//
// Could be improved with change to header file!!
	{
	TInt trailing=0;	// no of trailing zeros
	TInt leading=0;		// no of leading zeros
	TInt n;
	TChar c;
	
	if (aDl==0)		
		//	if no significance before decimal point need extra digit to lead to full significance
		leading++;	

	FOREVER
		{
		if (iNext>=iEnd)
			break;
		c=Peek();
		if (!c.IsDigit())
			break;
		else
			{
			c=Get();
			if (c=='0')
				{
				if (aDl!=0)		// possible trailing zeros
					trailing++;	
				else			// if aDl==0 multiplying by 10 and adding 0 has no effect  
					{
					leading++;
					aSig++;		// leading zeros have significance
					}
				}	
			else if ((aSig<KMaxPrecision+2+leading && !trailing) || (trailing && aSig+trailing+1<KMaxPrecision+2))
				{
				// first multiply, taking account of preceeding zeros
				for (n=trailing; n>=0; n--)
					{
					aDl *= 10;			// Multiply accumulator by 10
					}
				// now add current digit
				aDl+=((TUint)c)-'0';
				// now update significant digits used
				aSig+=trailing+1;
				trailing=0;
				}
			}
		}
	}	

void TLex16::Scndig(TInt &aSig,TInt &aExp,TInt64 &aDl)
//
// Scans a decimal digit field and accumulates the value to a TInt64 at aDl
//
	{

	FOREVER
		{
		TChar c=Peek();
		if (!c.IsDigit())
			break;
		else
			c=Get();
		if (aSig<KMaxPrecision+2)	
			{
			aDl *= 10;				// Multiply accumulator by 10
			aDl+=((TUint)c)-'0';	// Add current digit
			aSig++;
			}
		else
			aExp++;
		}														  
	}

EXPORT_C TInt TLex8::Val(TReal32& aVal)
//
// Convert a 32 bit real.
//
	{
	TRealX x;
	TInt r=Val(x);
	if (r==KErrNone)
		r=x.GetTReal(aVal);
	return r;
	}

EXPORT_C TInt TLex8::Val(TReal32& aVal, TChar aPoint)
//
// Convert a 32 bit real.
//
	{
	TRealX x;
	TInt r=Val(x,aPoint);
	if (r==KErrNone)
		r=x.GetTReal(aVal);
	return r;
	}

EXPORT_C TInt TLex8::Val(TReal64& aVal)
//
// Convert a 64 bit real.
//
	{
	TRealX x;
	TInt r=Val(x);
	if (r==KErrNone)
		r=x.GetTReal(aVal);
	return r;
	}

EXPORT_C TInt TLex8::Val(TReal64& aVal, TChar aPoint)
//
// Convert a 64 bit real.
//
	{
	TRealX x;
	TInt r=Val(x,aPoint);
	if (r==KErrNone)
		r=x.GetTReal(aVal);
	return r;
	}

TInt TLex8::Val(TRealX& aVal)
//
// Convert an extended real. Use the locale decimal point.
//
	{
	TLocale locale;
	return(Val(aVal,locale.DecimalSeparator()));
	}

TInt TLex8::Val(TRealX& aVal, TChar aPoint)
//
// Convert an extended real.
//
	{

	TLexMark8 start(iNext);
	if (iNext>=iEnd)
		return(KErrGeneral);
	TInt64 n(0);
	TBool minus=EFalse;
	if (Peek()=='-')
		{
		Inc();
		minus=ETrue;
		}
	else if (Peek()=='+')
		Inc();
	TInt digflg=Peek().IsDigit();
	while (Peek()=='0')		// Skip leading zeros
		Inc();
	TInt nsig=0;
	TInt nskip=0;
	Scndig(nsig,nskip,n);
	TInt nint=nsig;
	TInt nfract=0;
	if (Peek()==aPoint)
		{
		Inc();
		if (!digflg)
			digflg=Peek().IsDigit();
		ScndigAfterPoint(nsig,n);	// skip trailing zeros
		nfract=nsig-nint;
		}
	if (!digflg)
		{
		UnGetToMark(start);
		return(KErrGeneral);	// Not a number
		}
	TInt nexp=0;
	TInt r;
	if (Peek()=='E' || Peek()=='e')
		{
		TLexMark8 rollback(iNext);
		Inc();
		r=Val(nexp);
		if (r!=KErrNone)
			{
			if (r==KErrOverflow)
				{
				aVal.SetInfinite(minus);
				return r;
				}
			else
				{
				//it wasn't a number after the 'e', so rollback to the 'e'
				UnGetToMark(rollback);
				}
			}
		}

	if (n == 0)
		{
		aVal.SetZero();
		return KErrNone;
		}

	aVal=minus?-n:n;
	nexp+=nskip-nfract;
	r=Math::MultPow10X(aVal,nexp);
	return r;
	}

EXPORT_C TInt TLex16::Val(TReal32& aVal)
//
// Convert a 32 bit real.
//
	{
	TRealX x;
	TInt r=Val(x);
	if (r==KErrNone)
		r=x.GetTReal(aVal);
	return r;
	}

EXPORT_C TInt TLex16::Val(TReal32& aVal, TChar aPoint)
//
// Convert a 32 bit real.
//
	{
	TRealX x;
	TInt r=Val(x,aPoint);
	if (r==KErrNone)
		r=x.GetTReal(aVal);
	return r;
	}

EXPORT_C TInt TLex16::Val(TReal64& aVal)
//
// Convert a 64 bit real.
//
	{
	TRealX x;
	TInt r=Val(x);
	if (r==KErrNone)
		r=x.GetTReal(aVal);
	return r;
	}

EXPORT_C TInt TLex16::Val(TReal64& aVal, TChar aPoint)
//
// Convert a 64 bit real.
//
	{
	TRealX x;
	TInt r=Val(x,aPoint);
	if (r==KErrNone)
		r=x.GetTReal(aVal);
	return r;
	}

TInt TLex16::Val(TRealX& aVal)
//
// Convert an extended real. Use the locale decimal point.
//
	{
	TLocale locale;
	return(Val(aVal,locale.DecimalSeparator()));
	}

TInt TLex16::Val(TRealX& aVal, TChar aPoint)
//
// Convert a 64 bit real
//
	{
	
	HBufC8 *temp=HBufC8::New(iEnd-iNext);
	if (temp==NULL)
		return(KErrNoMemory);
	TPtr8 tdes(temp->Des());

//	for (TUint8 *p=(TUint8*)iNext; p<(TUint8*)iEnd; p+=2)
//		tdes.Append(*p);

	for (const TText* p = (TText*)iNext; p < (TText*)iEnd; p++)
		{
		TChar c = *p;
		if (c == aPoint)
			c = '.';
		else if (c == '.')
			c = ' ';
		else if (c > 255)
			c = ' ';
		tdes.Append((TUint8)c);
		}
	aPoint = '.';

	TLex8 lex(tdes);
	lex.Mark();
	TInt r=lex.Val(aVal,aPoint);
	User::Free(temp);
	if (r==KErrNone)
		Inc(lex.TokenLength());
	return r;
	}
