// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// e32test\buffer\t_char.cpp
// Overview:
// Test TChar, TCharF, TChaLC and TCharUC character classes.
// API Information:
// TChar, TCharF, TChaLC, TCharUC.
// Details:
// - For different characters 
// - Verify character's standard category information
// - CR1080 verification of standard category information for additional Indic characters
// - Verify character's bi-directional Unicode character category.
// - Check if character is printable.
// - Check if character is a graphical character.
// - Verify for Chinese, Japanese, Korean (CJK) notional width.
// - Verify that it is upper case, lower case form.
// - Verify character's title case.
// - Verify mirrored property of character.
// - Verify integer numeric property of character.
// - Verify character can be decomposed.
// - For different unicode strings, verify unicode collation
// - Compare data of two descriptors using the different collation methods and 
// check results are as expected.
// - Call the methods in the TChar, TCharF, TCharUC and TCharUC
// - For TChar, TCharF, TChaLC, TCharUC
// - Check the constructor and operators are as expected.
// - Check folded character is same when it is converted to folded uppercase.
// - Check folded and uppercase character is same when it is converted to 
// uppercase letter stripped of accents.
// - Convert lowercase, uppercase characters to it's lowercase form, check the conversion 
// is as expected, and lowercase characters are unchanged.
// - Convert lowercase, uppercase characters to it's uppercase form, check the conversion 
// is as expected, and uppercase characters are unchanged.
// Platforms/Drives/Compatibility:
// All 
// Assumptions/Requirement/Pre-requisites:
// Failures and causes:
// Base Port information:
// 
//

#include <e32test.h>
#include <collate.h>	  

#ifdef __VC32__
    // Solve compilation problem caused by non-English locale
    #pragma setlocale("english")
#endif

LOCAL_D RTest test(_L("T_CHAR"));

enum
	{
	Ydieresis=0x178,
	nobreakspace=160,
	exclamdown,
	cent,
	sterling,
	currency,
	yen,
	brokenbar,
	section,
	dieresis,
	copyright,
	ordfeminine,
	leftdoubleanglequote,
	logicalnot,
	softhyphen,
	registered,
	macron,
	degree,
	plusminus,
	twosuperior,
	threesuperior,
	acute,
	micro,
	pilcrow,
	dotaccent,
	cedilla,
	onesuperior,
	ordmasculine,
	rightdoubleanglequote,
	onequarter,
	onehalf,
	threequarters,
	questiondown,
	Agrave,
	Aacute,
	Acircumflex,
	Atilde,
	Adieresis,
	Aring,
	AE,
	Ccedilla,
	Egrave,
	Eacute,
	Ecircumflex,
	Edieresis,
	Igrave,
	Iacute,
	Icircumflex,
	Idieresis,
	Eth,
	Ntilde,
	Ograve,
	Oacute,
	Ocircumflex,
	Otilde,
	Odieresis,
	multiplication,
	Oslash,
	Ugrave,
	Uacute,
	Ucircumflex,
	Udieresis,
	Yacute,
	Thorn,
	germanbls,
	agrave,
	aacute,
	acircumflex,
	atilde,
	adieresis,
	aring,
	ae,
	ccedilla,
	egrave,
	eacute,
	ecircumflex,
	edieresis,
	igrave,
	iacute,
	icircumflex,
	idieresis,
	eth,
	ntilde,
	ograve,
	oacute,
	ocircumflex,
	otilde,
	odieresis,
	division,
	oslash,
	ugrave,
	uacute,
	ucircumflex,
	udieresis,
	yacute,
	thorn,
	ydieresis,
	Dzcaron = 0x01C5, 
	Lj = 0x01C8, 
	Udearesismacron = 0x01D5,
	Adotabove = 0x01E0,
	AEmacron = 0x01E2,
	aringacute = 0x01FB,
	scommabelow = 0x0219, 
	Otildemacron = 0x022C,
	Cstroke = 0x023B,
	doubleprime = 0x02BA, 
	lowleftarrow = 0x02FF,
	candrabindu = 0x0310, 
	doublerightwardsarrowbelow = 0x0362,
	latinsmalla = 0x0363	
	};

struct TCharDump
	{
	TUint iChar;
	};

/*EXPORT_C*/ void TChar::__DbgTest(void* pTCharDump) const
	{
	((TCharDump*)pTCharDump)->iChar=iChar;
	}

typedef enum {eTChar, eTCharF, eTCharC, eTCharLC, eTCharUC} TCharTypes;

template<class TCharType>	
class TestTChar
	{
public:
	void Test1();
	void Test2(TCharTypes);
	void Test3(TCharTypes);
	void Test4(TCharTypes);
	};

template<class TCharType>
GLDEF_C void TestTChar<TCharType>::Test1()
// Test that the methods are in the DLL
	{
	test.Start(_L("Test methods are in DLL - TChar"));
	TChar a('X');
	TUint TU=0;
	TChar b(TU);
	TChar c(b);

	c=TU;
	c=a;
	c-=TU;
	c+=TU;
	c-TU;
	c+TU;
	c=(TUint)(a);

	a.Fold();
	a.LowerCase();
	a.UpperCase();
	a.Eos();
	a.IsLower();
	a.IsUpper();
	a.IsAlpha();
	a.IsDigit();
	a.IsAlphaDigit();
	a.IsHexDigit();
	a.IsSpace();
	a.IsPunctuation();
	a.IsGraph();
	a.IsPrint();
//	a.SetChar(TU); protected

	test.Next(_L("Test methods are in DLL - TCharF"));	
	TCharF d(TU);
	TCharF e(a);
    e=d;
	e=TU;
	e=a;

	test.Next(_L("Test methods are in DLL - TCharLC"));	
	TCharLC h(TU);
	TCharLC i(a);
    h=i;
	h=TU;
	h=a;

	test.Next(_L("Test methods are in DLL - TCharUC"));	
	TCharUC j(TU);
	TCharUC k(a);
    k=j;
	k=TU;
	k=a;
	test.End();
	}
			

//*********************************
// Test the constructors
//*********************************
template<class TCharType>
GLDEF_C void TestTChar<TCharType>::Test2(TCharTypes CharType)
	{
	test.Start(_L("Test constructors"));
	TCharDump dump;
	TCharType b('x');
	b.__DbgTest(&dump);
	switch(CharType)
		{
		case eTChar:	
		case eTCharLC:
			test(dump.iChar=='x');
			break;
		case eTCharUC:
			test(dump.iChar=='X');
			break;
		case eTCharF:
		case eTCharC:
			test(dump.iChar == 'x');
		}
	test.End();
	}


//*********************************
// Test the operators
//*********************************
template<class TCharType>
GLDEF_C void TestTChar<TCharType>::Test3(TCharTypes CharType)
	{
	test.Start(_L("Test the operators")); 
	test.Next(_L("="));
	TCharDump dump;
	TUint TUx='x', TUX='X';
	TChar TCy='y', TCY='Y';
	TCharType aChar('m');

	aChar=TUx;
	aChar.__DbgTest(&dump);
	switch(CharType)
		{
		case eTChar:	
		case eTCharLC:	test(dump.iChar=='x'); break;
		case eTCharUC:	test(dump.iChar=='X'); break;
		case eTCharF:
		case eTCharC:	
			test(dump.iChar == 'x');
		}

	aChar=TUX; 
	aChar.__DbgTest(&dump);
	switch(CharType)
		{	
		case eTCharLC:	test(dump.iChar=='x'); break;
		case eTChar:
		case eTCharUC:	test(dump.iChar=='X'); break;
		case eTCharF:
		case eTCharC:	
			test(dump.iChar == 'x');
		}

	aChar=TCy;
	aChar.__DbgTest(&dump);
	switch(CharType)
		{
		case eTChar:	
		case eTCharLC:	test(dump.iChar=='y'); break;
		case eTCharUC:	test(dump.iChar=='Y'); break;
		case eTCharF:
		case eTCharC:	
			test(dump.iChar == 'y');
		}

	aChar=TCY;
	aChar.__DbgTest(&dump);
	switch(CharType)
		{	
		case eTCharLC:	test(dump.iChar=='y'); break;
		case eTChar:
		case eTCharUC:	test(dump.iChar=='Y'); break;
		case eTCharF:
		case eTCharC:	
			test(dump.iChar == 'y');
		}


	test.Next(_L("-=, +=, -, + and cast"));
	TCharType x(10);
	x-=5;
	x.__DbgTest(&dump);
	test(dump.iChar==5);

	x+=5;
	x.__DbgTest(&dump);
	test(dump.iChar==10);

	test((x-(TUint)5)==5);
	x.__DbgTest(&dump);
	test(dump.iChar==10);

	test((x+(TUint)5)==15);
	x.__DbgTest(&dump);
	test(dump.iChar==10);

	TChar z('a');
	TUint w=z;
	test(w==97);

	test.End();
	}


//////////////////////////////////////////////
// Test Fold, Fold(TChar::EFoldAccents), Collate, LowerCase, UpperCase
////////////////////////////////////////////
template<class TCharType>
GLDEF_C void TestTChar<TCharType>::Test4(TCharTypes CharType)
	{
	test.Start(_L("Fold(), Fold(TChar::EFoldAccents), Collate(), LowerCase() UpperCase() & Eos()"));
	TCharDump dump1, dump2;
	TCharType c(0);

	// Is the folded version the same as the folded uppercase version?
	test.Next(_L("Fold() 0-127"));
    TUint i;
	for(i=0; i<=127; i++)
		{
		c = i;
		c.Fold();
		c.__DbgTest(&dump1);
		c.UpperCase();
		c.Fold();
		c.__DbgTest(&dump2);
		test(dump1.iChar == dump2.iChar);
		}

	// Is the folded and uppercased version the same as an uppercase letter stripped of accents?
	test.Next(_L("Fold() >127"));
	for(i=128; i<=255; i++)
		{
		c=i;
		c.Fold();
		c.UpperCase();
		c.__DbgTest(&dump1);
		if(CharType==eTChar)
			{
			switch(i)
				{
			case ccedilla:	
			case Ccedilla:
                test(dump1.iChar=='C');
                break;

			case udieresis:	
			case ucircumflex:
			case ugrave:
			case uacute:		
			case Udieresis:
			case Ucircumflex:
			case Ugrave:
			case Uacute:
                test(dump1.iChar=='U');
                break;

			case ydieresis:
			case yacute:
			case Yacute:
                test(dump1.iChar=='Y');
                break;

			case ecircumflex:
			case edieresis:
			case egrave:
			case eacute:		
			case Ecircumflex:
			case Edieresis:
			case Egrave:
			case Eacute:
                test(dump1.iChar=='E');
                break;

			case acircumflex:
			case adieresis:
			case agrave:		
			case aacute:
			case Acircumflex: 
			case Adieresis:	
			case Agrave:			
			case Aacute:
			case atilde:
			case Atilde:
			case aring:		
			case Aring:
                test(dump1.iChar=='A');
                break;

			case idieresis:
			case icircumflex:
			case iacute:
			case igrave:			
			case Idieresis:
			case Icircumflex:
			case Iacute:
			case Igrave:
                test(dump1.iChar=='I');
                break;

			case ocircumflex:
			case odieresis:
			case ograve:
			case oacute:		
			case otilde:
			case Ocircumflex:
			case Odieresis:
			case Ograve:
			case Oacute:
			case Otilde:
                test(dump1.iChar=='O');
                break;

			case ntilde:
			case Ntilde:
                test(dump1.iChar=='N'); break;

			case oslash:
			case Oslash:

				test(dump1.iChar==Oslash); break;

			case ae:
			case AE:
				test(dump1.iChar==AE); break;

			case eth:
			case Eth:
                test(dump1.iChar==Eth); break;

			case thorn:
			case Thorn:
                test(dump1.iChar==Thorn);
                break;

			case nobreakspace:
				test(dump1.iChar == ' ');
				break;

			case micro:
				test(dump1.iChar == 0x039C);
				break;

			default:
                test(dump1.iChar==i); 
				}
			}
		}
	
	// In the following test section We test Fold(TChar::EFoldAccents) - This code tests this function for all the
	// set of unicode characters located between 0x0080 and 0x00FF which are already tested for Fold(). 
	// I added to this test 13 unicode character located between 0x01C5 and 0x0363.
	
	test.Next(_L("Fold(TChar::EFoldAccents) >127"));
	for(i=128; i<=255; i++)
		{
		c=i;
		c.Fold(TChar::EFoldAccents);
		c.__DbgTest(&dump1);
		if(CharType==eTChar)
			{
			switch(i)
				{
			case ccedilla:
				test(dump1.iChar=='c');
				break;
				
			case Ccedilla:
				test(dump1.iChar=='C');
				break;
				
			case acircumflex:
			case adieresis:
			case agrave:		
			case aacute:
			case atilde:
			case aring:
			case aringacute:			
            	test(dump1.iChar=='a');
            	break;
            	
			case Acircumflex: 
			case Adieresis:	
			case Agrave:			
			case Aacute:
			case Atilde:		
			case Aring:		
                test(dump1.iChar=='A');
                break;

			case udieresis:	
			case ucircumflex:
			case ugrave:
			case uacute:
				test(dump1.iChar=='u');
				break;
				
			case Udieresis:
			case Ucircumflex:
			case Ugrave:
			case Uacute:
                test(dump1.iChar=='U');
                break;

			case ydieresis:
			case yacute:
				test(dump1.iChar=='y');
				break;
            
			case Yacute:
                test(dump1.iChar=='Y');
                break;

			case ecircumflex:
			case edieresis:
			case egrave:
			case eacute:		
            	test(dump1.iChar=='e');
            	break;
            	
			case Ecircumflex:
			case Edieresis:
			case Egrave:
			case Eacute:
                test(dump1.iChar=='E');
                break;			
				
			case idieresis:
			case icircumflex:
			case iacute:
			case igrave:
				test(dump1.iChar=='i');
				break;
				
			case Idieresis:
			case Icircumflex:
			case Iacute:
			case Igrave:
                test(dump1.iChar=='I');
                break;

			case ocircumflex:
			case odieresis:
			case ograve:
			case oacute:		
			case otilde:
				test(dump1.iChar=='o');
				break;
			
			case Ocircumflex:
			case Odieresis:
			case Ograve:
			case Oacute:
			case Otilde:
                test(dump1.iChar=='O');
                break;

			case ntilde: test(dump1.iChar=='n'); break;
				
			case Ntilde: test(dump1.iChar=='N'); break;

			case oslash: test(dump1.iChar==oslash); break;
			case Oslash: test(dump1.iChar==Oslash); break;

			case ae: 	test(dump1.iChar==ae); break;
			case AE: 	test(dump1.iChar==AE); break;

			case eth:   test(dump1.iChar==eth); break;
			case Eth:   test(dump1.iChar==Eth); break;

			case thorn: test(dump1.iChar==thorn);break;
			case Thorn: test(dump1.iChar==Thorn);break;

			case micro:
				test(dump1.iChar == 0x00B5);
				break;
							
			default:
                test(dump1.iChar==i); 
				}
			}
		}
	
	TUint specialCasesTable [] = 
		{
		0x01C5, // Dzcaron
		0x01C8, // Lj
		0x01D5,//  Udearesismacron
		0x01E0,//  Adotabove
		0x01E2,//  AEmacron
		0x0219, // scommabelow
		0x022C,//  Otildemacron
		0x023B,//  Cstroke
		0x02BA, // doubleprime
		0x02FF,//  lowleftarrow
		0x0310, // candrabindu
		0x0362, // doublerightwardsarrowbelow
		0x0363 //  latinsmalla
		};
	    
	for (i=0;i<=12; i++)
		{
		c=specialCasesTable[i];
		c.Fold(TChar::EFoldAccents);
		c.__DbgTest(&dump1);
		if(CharType==eTChar)
			{
			switch(specialCasesTable[i])
				{
			case Dzcaron					:	test(dump1.iChar==Dzcaron);	break; 
			case Lj							:	test(dump1.iChar==Lj);		break; 
			case Udearesismacron			:	test(dump1.iChar=='U');		break; 
			case Adotabove					:	test(dump1.iChar=='A');		break;
			case AEmacron					:	test(dump1.iChar==AE);		break; 
			case scommabelow				:	test(dump1.iChar=='s');		break;
			case Otildemacron				:	test(dump1.iChar=='O');		break; 
			case Cstroke					:	test(dump1.iChar==Cstroke);			break; 
			case doubleprime				:	test(dump1.iChar==doubleprime);		break; 
			case lowleftarrow				:	test(dump1.iChar==lowleftarrow);	break; 
			case candrabindu				:	test(dump1.iChar==candrabindu);		break; 
			case doublerightwardsarrowbelow	:	test(dump1.iChar==doublerightwardsarrowbelow);	break; 
			case latinsmalla 				:	test(dump1.iChar==latinsmalla);		break; 
			default							:	test(dump1.iChar==specialCasesTable[i]); 
				}
			}
		}

	test.Next(_L("LowerCase() 0-127"));
	for(i=0;i<=127; i++)
		{ 
		c=i;
		if(c.IsUpper())
			{
			c.__DbgTest(&dump1);
			c.LowerCase();
			c.__DbgTest(&dump2);
			test(dump1.iChar==dump2.iChar-32);
			}
		else
			{
			c.__DbgTest(&dump1);
			c.LowerCase();
			c.__DbgTest(&dump2);
			test(dump1.iChar==dump2.iChar);
			}
		}


	test.Next(_L("LowerCase() >127"));
	for(i=128; i<=255; i++)
		{
		c=i;
		c.LowerCase();
		c.__DbgTest(&dump1);
		if(CharType==eTChar)
			{
			switch(i)
				{
			case Acircumflex:	test(dump1.iChar==acircumflex);	break;
			case Adieresis:		test(dump1.iChar==adieresis);	break;
			case Agrave:		test(dump1.iChar==agrave);		break;
			case Aacute:		test(dump1.iChar==aacute);		break;
			case Atilde:		test(dump1.iChar==atilde);		break;
			case Aring:			test(dump1.iChar==aring);		break;
			case AE:			test(dump1.iChar==ae);			break;
			case Ccedilla:		test(dump1.iChar==ccedilla); 	break;
			case Ecircumflex:	test(dump1.iChar==ecircumflex);	break;
			case Edieresis:		test(dump1.iChar==edieresis);	break;
			case Egrave:		test(dump1.iChar==egrave);		break;
			case Eacute:		test(dump1.iChar==eacute);		break;
			case Eth:			test(dump1.iChar==eth);			break;
			case Icircumflex:	test(dump1.iChar==icircumflex);	break;
			case Idieresis:		test(dump1.iChar==idieresis);	break;
			case Igrave:		test(dump1.iChar==igrave);		break;
			case Iacute:		test(dump1.iChar==iacute);		break;
			case Ntilde:		test(dump1.iChar==ntilde);		break;
			case Oslash:		test(dump1.iChar==oslash);		break;
			case Ocircumflex:	test(dump1.iChar==ocircumflex);	break;
			case Odieresis:		test(dump1.iChar==odieresis);	break;
			case Ograve:		test(dump1.iChar==ograve);		break;
			case Oacute:		test(dump1.iChar==oacute);		break;
			case Otilde:		test(dump1.iChar==otilde);		break;
			case Thorn:			test(dump1.iChar==thorn);		break;
			case Ucircumflex:	test(dump1.iChar==ucircumflex);	break;
			case Udieresis:		test(dump1.iChar==udieresis);	break;
			case Ugrave:		test(dump1.iChar==ugrave);		break;
			case Uacute:		test(dump1.iChar==uacute);		break;
			case Yacute:		test(dump1.iChar==yacute);		break;
			default: 			test(dump1.iChar==i); 		
				}
			}
		}


	test.Next(_L("UpperCase() 0-127"));
	for(i=0;i<=127; i++)
		{ 
		c=i;
		if(c.IsLower())
			{
			c.__DbgTest(&dump1);
			c.UpperCase();
			c.__DbgTest(&dump2);
			test(dump1.iChar==dump2.iChar+32);
			}
		else
			{
			c.__DbgTest(&dump1);
			c.UpperCase();
			c.__DbgTest(&dump2);
			test(dump1.iChar==dump2.iChar);
			}
		}


	test.Next(_L("UpperCase() >127"));
	for(i=128; i<=255; i++)
		{
		c=i;
		c.UpperCase();
		c.__DbgTest(&dump1);
		if(CharType==eTChar)
			{
			switch(i)	
				{
			case acircumflex:	test(dump1.iChar==Acircumflex);	break;
			case adieresis:		test(dump1.iChar==Adieresis);	break;
			case agrave:		test(dump1.iChar==Agrave);		break;
			case aacute:		test(dump1.iChar==Aacute);		break;
			case atilde:		test(dump1.iChar==Atilde);		break;
			case aring:			test(dump1.iChar==Aring);		break;
			case ae:			test(dump1.iChar==AE);			break;
			case ccedilla:		test(dump1.iChar==Ccedilla); 	break;
			case ecircumflex:	test(dump1.iChar==Ecircumflex);	break;
			case edieresis:		test(dump1.iChar==Edieresis);	break;
			case egrave:		test(dump1.iChar==Egrave);		break;
			case eacute:		test(dump1.iChar==Eacute);		break;
			case eth:			test(dump1.iChar==Eth);			break;
			case icircumflex:	test(dump1.iChar==Icircumflex);	break;
			case idieresis:		test(dump1.iChar==Idieresis);	break;
			case igrave:		test(dump1.iChar==Igrave);		break;
			case iacute:		test(dump1.iChar==Iacute);		break;
			case ntilde:		test(dump1.iChar==Ntilde);		break;
			case oslash:		test(dump1.iChar==Oslash);		break;
			case ocircumflex:	test(dump1.iChar==Ocircumflex);	break;
			case odieresis:		test(dump1.iChar==Odieresis);	break;
			case ograve:		test(dump1.iChar==Ograve);		break;
			case oacute:		test(dump1.iChar==Oacute);		break;
			case otilde:		test(dump1.iChar==Otilde);		break;
			case thorn:			test(dump1.iChar==Thorn);		break;
			case ucircumflex:	test(dump1.iChar==Ucircumflex);	break;
			case udieresis:		test(dump1.iChar==Udieresis);	break;
			case ugrave:		test(dump1.iChar==Ugrave);		break;
			case uacute:		test(dump1.iChar==Uacute);		break;
			case yacute:		test(dump1.iChar==Yacute);		break;
			case ydieresis:		test(dump1.iChar==Ydieresis);	break;
			case micro:			test(dump1.iChar==0x039C);		break;
			default: 			test(dump1.iChar==i); 		
				}
			}
		}
		   
	test.Next(_L("Eos()"));
	c=0;
	test(c.Eos()==TRUE);
	for(i=1;i<=255;i++)
		{
		c=i;
		test(c.Eos()==FALSE);
		}
	test.End();	
	}


//*********************************
// Test the Isxxx functions
//*********************************
//
// Deleted as it can no longer use the C library
//
/*
template<class TCharType>
GLDEF_C void TestTChar<TCharType>::Test5(TCharTypes CharType)
	{
	test.Start(_L("Isxxxxx()"));
	test.End();
	}
*/

#ifndef _DEBUG
#pragma warning (disable :4710) //Function not expanded 
#pragma warning (disable :4700) //Local variable used without being initialized
#endif

/*
Put all the information about a character into a string. This can then be compared with
a previously verified string to test the character classification functions.

To keep the string short, only non-default information is appended, except for the first
attribute, the category.

The format is:

1. The two-letter Unicode category code: e.g., Lu for an uppercase letter, Nd for a digit, etc.
2. If the bidirectional category is not TChar::ELeftToRight, ' b=' then the one or two-letter code.
3. If the combining class is not 0, ' c=' then the combining class number in decimal.
4. If the uppercase form is different from the character code, ' u=' then the uppercase form in hex.
5. If the titlecase form is different from the character code, ' t=' then the titlecase form in hex.
6. If the lowercase form is different from the character code, ' l=' then the lowercase form in hex.
** REMOVED ** 7. If the folded form is different from the character code, ' f=' then the folded form in hex.
8. If the character has the mirrored property, ' m'.
9. If the character has a numeric value, ' n=' then the numeric value in decimal, or -2 for fractions.
10. If the CJK width is not neutral, ' w=' then 1 for halfwidth, 2 for fullwidth, 3 for narrow, 4 for wide
11. If there is a canonical decomposition, ' d=' then the characters of the decomposition in hex, comma-separated.
*/
void char_info_to_string(TDes16& info,TUint16 x)
	{
	TChar c(x);

	TChar::TCharInfo ci;
	c.GetInfo(ci);

	TBuf<16> cat;
	switch (ci.iCategory)
		{
		case TChar::ELuCategory: cat = _L("Lu"); break;
		case TChar::ELlCategory: cat = _L("Ll"); break;
		case TChar::ELtCategory: cat = _L("Lt"); break; 
		case TChar::ELoCategory: cat = _L("Lo"); break; 
		case TChar::ELmCategory: cat = _L("Lm"); break; 
		case TChar::EMnCategory: cat = _L("Mn"); break; 
		case TChar::EMcCategory: cat = _L("Mc"); break; 
		case TChar::ENdCategory: cat = _L("Nd"); break; 
		case TChar::ENlCategory: cat = _L("Nl"); break;
		case TChar::ENoCategory: cat = _L("No"); break; 
		case TChar::EPdCategory: cat = _L("Pd"); break; 
		case TChar::EPsCategory: cat = _L("Ps"); break; 
		case TChar::EPeCategory: cat = _L("Pe"); break; 
		case TChar::EPoCategory: cat = _L("Po"); break; 
		case TChar::EPiCategory: cat = _L("Pi"); break; 
		case TChar::EPfCategory: cat = _L("Pf"); break; 
		case TChar::ESmCategory: cat = _L("Sm"); break; 
		case TChar::EScCategory: cat = _L("Sc"); break; 
		case TChar::ESoCategory: cat = _L("So"); break; 
		case TChar::EZsCategory: cat = _L("Zs"); break; 
		case TChar::EZlCategory: cat = _L("Zl"); break; 
		case TChar::EZpCategory: cat = _L("Zp"); break; 
		case TChar::ECcCategory: cat = _L("Cc"); break; 
		case TChar::ECoCategory: cat = _L("Co"); break; 
		case TChar::ECnCategory: cat = _L("Cn"); break;
		default: User::Panic(_L("bad char category"),0); break; 
		}
	info.Append(cat);

	TBuf<16> bdcat;
	switch (ci.iBdCategory)
		{
		case TChar::ELeftToRight: bdcat=_L("L"); break;
		case TChar::ELeftToRightEmbedding: bdcat=_L("LRE"); break;
		case TChar::ELeftToRightOverride: bdcat=_L("LRO"); break;
		case TChar::ERightToLeft: bdcat=_L("R"); break;
		case TChar::ERightToLeftArabic: bdcat=_L("AL"); break; 
		case TChar::ERightToLeftEmbedding: bdcat=_L("RLE"); break;
		case TChar::ERightToLeftOverride: bdcat=_L("RLO"); break;
		case TChar::EPopDirectionalFormat: bdcat=_L("PDF"); break;
		case TChar::EEuropeanNumber: bdcat=_L("EN"); break; 
		case TChar::EEuropeanNumberSeparator: bdcat=_L("ES"); break;  
		case TChar::EEuropeanNumberTerminator: bdcat=_L("ET"); break;
		case TChar::EArabicNumber: bdcat=_L("AN"); break; 
		case TChar::ECommonNumberSeparator: bdcat=_L("CS"); break; 
		case TChar::ENonSpacingMark: bdcat=_L("NSM"); break;
		case TChar::EBoundaryNeutral: bdcat=_L("BN"); break; 
		case TChar::EParagraphSeparator: bdcat=_L("B"); break;
		case TChar::ESegmentSeparator: bdcat=_L("S"); break;
		case TChar::EWhitespace: bdcat=_L("WS"); break; 
		case TChar::EOtherNeutral: bdcat=_L("ON"); break; 
		default: User::Panic(_L("bad bd category"),0); break; 
		}
	if (ci.iBdCategory != TChar::ELeftToRight)
		{
		info.Append(_L(" b="));
		info.Append(bdcat);
		}

	if (ci.iCombiningClass != 0)
		{
		info.Append(_L(" c="));
		info.AppendNum(ci.iCombiningClass);
		}

	if (ci.iUpperCase != x)
		{
		info.Append(_L(" u="));
		info.AppendNum(ci.iUpperCase,EHex);
		}
		
	if (ci.iTitleCase != x)
		{
		info.Append(_L(" t="));
		info.AppendNum(ci.iTitleCase,EHex);
		}

	if (ci.iLowerCase != x)
		{
		info.Append(_L(" l="));
		info.AppendNum(ci.iLowerCase,EHex);
		}

	if (ci.iMirrored)
		info.Append(_L(" m"));

	if (ci.iNumericValue != -1)
		{
		info.Append(_L(" n="));
		info.AppendNum(ci.iNumericValue);
		}

	switch (c.GetCjkWidth())
		{
		case TChar::EHalfWidth: info.Append(_L(" w=1")); break;
		case TChar::EFullWidth: info.Append(_L(" w=2")); break;
		case TChar::ENarrow: info.Append(_L(" w=3")); break;
		case TChar::EWide: info.Append(_L(" w=4")); break;
		default: break;
		}

	TPtrC16 d;
	if (c.Decompose(d))
		{
		info.Append(_L(" d="));
		for (int i = 0; i < d.Length(); i++)
			{
			info.AppendNum((TUint)d[i],EHex);
			if (i < d.Length() - 1)
				info.Append(',');
			}
		}

	if (!c.IsPrint())
		info.Append(_L(" np"));
	if (!c.IsGraph())
		info.Append(_L(" ng"));
	}

void test_unicode_char(TUint16 c,const TDesC& desired_info,const TDesC& name)
	{
	test.Next(name);
	TBuf<128> info;
	char_info_to_string(info,c);
//	test.Printf(_L("code=0x%x desired='%S' actual='%S'\n"),c,&desired_info,&info);
//	if (info != desired_info)
//		test.Printf(_L(" (mismatch!)\n"));
	test(info == desired_info);
	}

void test_unicode_chars()
	{
	test_unicode_char(0x0000,_L("Cc b=BN np ng"),_L("NULL"));
	test_unicode_char(0x000A,_L("Cc b=B np ng"),_L("linefeed"));
	test_unicode_char(0x0020,_L("Zs b=WS w=3 ng"),_L("space"));
	test_unicode_char(0x0028,_L("Ps b=ON m w=3"),_L("("));
	test_unicode_char(0x0038,_L("Nd b=EN n=8 w=3"),_L("8"));
	test_unicode_char(0x0040,_L("Po b=ON w=3"),_L("@"));
	test_unicode_char(0x0062,_L("Ll u=42 w=3"),_L("b"));
	test_unicode_char(0x0042,_L("Lu l=62 w=3"),_L("B"));
	test_unicode_char(0x0067,_L("Ll u=47 w=3"),_L("g"));
	test_unicode_char(0x0047,_L("Lu l=67 w=3"),_L("G"));
	test_unicode_char(0x0077,_L("Ll u=57 w=3"),_L("w"));
	test_unicode_char(0x0057,_L("Lu l=77 w=3"),_L("W"));
	test_unicode_char(0x00A0,_L("Zs b=CS ng"),_L("non-break space"));
	test_unicode_char(0x00BC,_L("No b=ON n=-2"),_L("1/4"));
	test_unicode_char(0x00DD,_L("Lu l=fd d=59,301"),_L("Y acute"));
	test_unicode_char(0x00FF,_L("Ll u=178 d=79,308"),_L("y umlaut"));
	test_unicode_char(0x0178,_L("Lu l=ff d=59,308"),_L("Y umlaut"));
	test_unicode_char(0x01C4,_L("Lu t=1c5 l=1c6"),_L("DZ caron"));
	test_unicode_char(0x01C5,_L("Lt u=1c4 l=1c6"),_L("Dz caron"));
	test_unicode_char(0x01C6,_L("Ll u=1c4 t=1c5"),_L("dz caron"));
	test_unicode_char(0x022A,_L("Lu l=22b d=4f,308,304"),_L("O with diaeresis and macron"));
	test_unicode_char(0x0267,_L("Ll"),_L("latin small letter heng with hook"));
	test_unicode_char(0x02b0,_L("Lm"),_L("modifier letter small h"));
	test_unicode_char(0x030a,_L("Mn b=NSM c=230"),_L("combining ring above"));
	test_unicode_char(0x03a8,_L("Lu l=3c8"),_L("Greek Psi"));
	test_unicode_char(0x03c8,_L("Ll u=3a8"),_L("Greek psi"));
	test_unicode_char(0x0401,_L("Lu l=451 d=415,308"),_L("Cyrillic IO"));
	test_unicode_char(0x0451,_L("Ll u=401 d=435,308"),_L("Cyrillic io"));
	test_unicode_char(0x04e0,_L("Lu l=4e1"),_L("Cyrillic Abkhasian Dze"));
	test_unicode_char(0x04e1,_L("Ll u=4e0"),_L("Cyrillic Abkhasian dze"));
	test_unicode_char(0x05A8,_L("Mn b=NSM c=230"),_L("Hebrew accent qadma"));
	test_unicode_char(0x05D2,_L("Lo b=R"),_L("Hebrew letter gimel"));
	test_unicode_char(0x05F0,_L("Lo b=R"),_L("Hebrew ligature Yiddish double vav"));
	test_unicode_char(0x0627,_L("Lo b=AL"),_L("Arabic letter alef"));
	test_unicode_char(0x0660,_L("Nd b=AN n=0"),_L("Arabic-Indic digit 0"));
	test_unicode_char(0x0669,_L("Nd b=AN n=9"),_L("Arabic-Indic digit 9"));
	test_unicode_char(0x06F4,_L("Nd b=EN n=4"),_L("Extended Arabic-Indic digit 4"));
	test_unicode_char(0x0B9C,_L("Lo"),_L("Tamil letter ja"));
	test_unicode_char(0x0BCD,_L("Mn b=NSM c=9"),_L("Tamil sign virama"));
	test_unicode_char(0x0BF0,_L("No n=10"),_L("Tamil number 10"));
	test_unicode_char(0x0BF1,_L("No n=100"),_L("Tamil number 100"));
	test_unicode_char(0x0BF2,_L("No n=1000"),_L("Tamil number 1000"));
	test_unicode_char(0x0E3F,_L("Sc b=ET"),_L("Thai currency symbol baht"));
	test_unicode_char(0x0ED5,_L("Nd n=5"),_L("Lao digit 5"));
	test_unicode_char(0x104E,_L("Po"),_L("Myanmar symbol aforementioned"));
	test_unicode_char(0x124B,_L("Lo"),_L("Ethiopic syllable QWAA"));
	test_unicode_char(0x137C,_L("No n=10000"),_L("Ethiopic number 10000"));
	test_unicode_char(0x217F,_L("Nl u=216f n=1000"),_L("small Roman numeral 1000"));
	test_unicode_char(0x2180,_L("Nl n=1000"),_L("Roman numeral 1000 CD"));
	test_unicode_char(0x2181,_L("Nl n=5000"),_L("Roman numeral 5000"));
	test_unicode_char(0x2182,_L("Nl n=10000"),_L("Roman numeral 10000"));
	test_unicode_char(0x2208,_L("Sm b=ON m"),_L("element of"));
	test_unicode_char(0x2248,_L("Sm b=ON m"),_L("almost equal to"));
	test_unicode_char(0x3000,_L("Zs b=WS w=4 ng"),_L("ideographic space"));
	test_unicode_char(0x3030,_L("Pd b=ON w=4"),_L("wavy dash"));
	test_unicode_char(0x305F,_L("Lo w=4"),_L("Hiragana ta"));
	test_unicode_char(0x3060,_L("Lo w=4 d=305f,3099"),_L("Hiragana da"));
	test_unicode_char(0x3099,_L("Mn b=NSM c=8 w=4"),_L("combining Katakana-Hiragana voiced sound mark"));
	test_unicode_char(0x30B3,_L("Lo w=4"),_L("Katakana ko"));
	test_unicode_char(0x30B4,_L("Lo w=4 d=30b3,3099"),_L("Katakana go"));
	test_unicode_char(0x51FF,_L("Lo w=4"),_L("CJK ideograph 51FF"));
	test_unicode_char(0x52FF,_L("Lo w=4"),_L("CJK ideograph 52FF"));
	test_unicode_char(0xCEFF,_L("Lo w=4"),_L("Hangul syllable CEFF"));
	test_unicode_char(0xFF17,_L("Nd b=EN n=7 w=2"),_L("fullwidth digit 7"));
	test_unicode_char(0xFF33,_L("Lu l=ff53 w=2"),_L("fullwidth S"));
	test_unicode_char(0xFF53,_L("Ll u=ff33 w=2"),_L("fullwidth s"));
	test_unicode_char(0xFF85,_L("Lo w=1"),_L("halfwidth Katakana na"));
	test_unicode_char(0xFFE1,_L("Sc b=ET w=2"),_L("fullwidth pound sign"));
	test_unicode_char(0xE000, _L("Co"), _L("End user private use character"));
	test_unicode_char(0xE3FF, _L("Co"), _L("End user private use character"));
	test_unicode_char(0xE400, _L("Co"), _L("Corporate private use character"));
	test_unicode_char(0xF6DA, _L("Co np ng"), _L("Corporate private use character - Java string marker"));
	test_unicode_char(0xF6DB, _L("Co"), _L("Corporate private use character - Replacement character"));
	test_unicode_char(0xF6DC, _L("Co np ng"), _L("Corporate private use character - switch to next"));
	test_unicode_char(0xF6DD, _L("Co np ng"), _L("Corporate private use character - switch to previous"));
	test_unicode_char(0xF6DB, _L("Co"), _L("Corporate private use character - Latin superscript y"));
	test_unicode_char(0xF6FF, _L("Co"), _L("Corporate private use character - Alarm bell"));
	test_unicode_char(0xF700, _L("Co np ng"), _L("Corporate private use character - Key code"));
	test_unicode_char(0xF8FF, _L("Co np ng"), _L("Corporate private use character - Key code"));
	
	// CR1080 - Testing additional Indic Chars
	// Test properties of new Indic characters added in CR1080, and also the properties of
	// existing characters immediately preceding and proceding the new characters, to test
	// for any regression.
	test_unicode_char(0x0903, _L("Mc"), _L("DEVANAGARI SIGN VISARGA"));  		// Existing
	test_unicode_char(0x0904, _L("Lo"), _L("DEVANAGARI LETTER SHORT A"));		// New
	test_unicode_char(0x0905, _L("Lo"), _L("DEVANAGARI LETTER A"));				// Existing
	
	test_unicode_char(0x0970, _L("Po"), _L("DEVANAGARI ABBREVIATION SIGN"));	// Existing
	test_unicode_char(0x097B, _L("Lo"), _L("DEVANAGARI LETTER GGA"));			// New
	test_unicode_char(0x097C, _L("Lo"), _L("DEVANAGARI LETTER JJA"));			// New
	test_unicode_char(0x097D, _L("Lo"), _L("DEVANAGARI LETTER GLOTTAL STOP"));	// New
	test_unicode_char(0x097E, _L("Lo"), _L("DEVANAGARI LETTER DDDA"));			// New
	test_unicode_char(0x097F, _L("Lo"), _L("DEVANAGARI LETTER BBA"));			// New
	test_unicode_char(0x0981, _L("Mn b=NSM"), _L("BENGALI SIGN CANDRABINDU"));	// Existing
	
	test_unicode_char(0x09BC, _L("Mn b=NSM c=7"), _L("BENGALI SIGN NUKTA"));	// Existing
	test_unicode_char(0x09BD, _L("Lo"), _L("BENGALI SIGN AVAGRAHA"));			// New
	test_unicode_char(0x09BE, _L("Mc"), _L("BENGALI VOWEL SIGN AA"));			// Existing
	
	test_unicode_char(0x09CD, _L("Mn b=NSM c=9"), _L("BENGALI SIGN VIRAMA"));	// Existing
	test_unicode_char(0x09CE, _L("Lo"), _L("BENGALI LETTER KHANDA TA"));		// New
	test_unicode_char(0x09D7, _L("Mc"), _L("BENGALI AU LENGTH MARK"));			// Existing

	test_unicode_char(0x09FA, _L("So"), _L("BENGALI ISSHAR"));					// Existing
	test_unicode_char(0x0A01, _L("Mn b=NSM"), _L("GURMUKHI SIGN ADAK BINDI"));	// New
	test_unicode_char(0x0A02, _L("Mn b=NSM"), _L("GURMUKHI SIGN BINDI"));		// Existing
	test_unicode_char(0x0A03, _L("Mc"), _L("GURMUKHI SIGN VISARGA"));			// New
	test_unicode_char(0x0A05, _L("Lo"), _L("GURMUKHI LETTER A"));				// Existing
	
	test_unicode_char(0x0A8B, _L("Lo"), _L("GUJARATI LETTER VOCALIC R"));		// Existing
	test_unicode_char(0x0A8C, _L("Lo"), _L("GUJARATI LETTER VOCALIC L"));		// New
	test_unicode_char(0x0A8D, _L("Lo"), _L("GUJARATI VOWEL CANDRA E"));			// Existing
	
	test_unicode_char(0x0AE0, _L("Lo"), _L("GUJARATI LETTER VOCALIC RR"));				// Existing
	test_unicode_char(0x0AE1, _L("Lo"), _L("GUJARATI LETTER VOCALIC LL"));				// New
	test_unicode_char(0x0AE2, _L("Mn b=NSM"), _L("GUJARATI VOWEL SIGN VOCALIC L"));		// New
	test_unicode_char(0x0AE3, _L("Mn b=NSM"), _L("GUJARATI VOWEL SIGN VOCALIC LL"));	// New
	test_unicode_char(0x0AE6, _L("Nd n=0"), _L("GUJARATI DIGIT ZERO")); 				// Existing
	
	test_unicode_char(0x0AEF, _L("Nd n=9"), _L("GUJARATI DIGIT NINE"));			// Existing
	test_unicode_char(0x0AF1, _L("Sc b=ET"), _L("GUJARATI RUPEE SIGN"));		// New
	test_unicode_char(0x0B01, _L("Mn b=NSM"), _L("ORIYA SIGN CANDRABINDU")); 	// Existing

	test_unicode_char(0x0B33, _L("Lo"), _L("ORIYA LETTER LLA"));				// Existing
	test_unicode_char(0x0B35, _L("Lo"), _L("ORIYA LETTER VA"));					// New
	test_unicode_char(0x0B36, _L("Lo"), _L("ORIYA LETTER SHA"));				// Existing

	test_unicode_char(0x0B70, _L("So"), _L("ORIYA ISSHAR"));					// Existing
	test_unicode_char(0x0B71, _L("Lo"), _L("ORIYA LETTER WA"));					// New
	test_unicode_char(0x0B82, _L("Mn b=NSM"), _L("TAMIL SIGN ANUSVARA")); 		// Existing

	test_unicode_char(0x0BB5, _L("Lo"), _L("TAMIL LETTER VA"));					// Existing
	test_unicode_char(0x0BB6, _L("Lo"), _L("TAMIL LETTER SHA"));				// New
	test_unicode_char(0x0BB7, _L("Lo"), _L("TAMIL LETTER SSA"));				// Existing

	test_unicode_char(0x0BD7, _L("Mc"), _L("TAMIL AU LENGTH MARK"));			// Existing
	test_unicode_char(0x0BE6, _L("Nd n=0"), _L("TAMIL DIGIT ZERO"));			// New
	test_unicode_char(0x0BE7, _L("Nd n=1"), _L("TAMIL DIGIT ZERO"));			// Existing

	test_unicode_char(0x0BF2, _L("No n=1000"), _L("TAMIL NUMBER ONE THOUSAND"));// Existing
	test_unicode_char(0x0BF3, _L("So b=ON"), _L("TAMIL DAY SIGN"));				// New
	test_unicode_char(0x0BF4, _L("So b=ON"), _L("TAMIL MONTH SIGN"));			// New
	test_unicode_char(0x0BF5, _L("So b=ON"), _L("TAMIL YEAR SIGN"));			// New
	test_unicode_char(0x0BF6, _L("So b=ON"), _L("TAMIL DEBIT SIGN"));			// New
	test_unicode_char(0x0BF7, _L("So b=ON"), _L("TAMIL CREDIT SIGN"));			// New
	test_unicode_char(0x0BF8, _L("So b=ON"), _L("TAMIL AS ABOVE SIGN"));		// New
	test_unicode_char(0x0BF9, _L("Sc b=ET"), _L("TAMIL RUPEE SIGN"));			// New
	test_unicode_char(0x0BFA, _L("So b=ON"), _L("TAMIL NUMBER SIGN"));			// New
	test_unicode_char(0x0C01, _L("Mc"), _L("TELUGU SIGN CANDRABINDU"));			// Existing
	
	test_unicode_char(0x0CB9, _L("Lo"), _L("KANNADA LETTER HA"));				// Existing
	test_unicode_char(0x0CBC, _L("Mn b=NSM c=7"), _L("KANNADA SIGN NUKTA"));	// New
	test_unicode_char(0x0CBD, _L("Lo"), _L("KANNADA SIGN AVAGRAHA"));			// New
	test_unicode_char(0x0CBE, _L("Mc"), _L("KANNADA VOWEL SIGN AA"));			// Existing

	test_unicode_char(0x0CE1, _L("Lo"), _L("KANNADA LETTER VOCALIC LL"));			// Existing
	test_unicode_char(0x0CE2, _L("Mn b=NSM"), _L("KANNADA VOWEL SIGN VOCALIC L"));	// New
	test_unicode_char(0x0CE3, _L("Mn b=NSM"), _L("KANNADA VOWEL SIGN VOCALIC LL"));	// New
	test_unicode_char(0x0CE6, _L("Nd n=0"), _L("KANNADA DIGIT ZERO"));				// Existing

	test_unicode_char(0x0CEF, _L("Nd n=9"), _L("KANNADA DIGIT NINE"));				// Existing
	test_unicode_char(0x0CF1, _L("So b=ON"), _L("KANNADA SIGN JIHVAMULIYA"));		// New
	test_unicode_char(0x0CF2, _L("So b=ON"), _L("KANNADA SIGN UPADHMANIYA"));		// New
	test_unicode_char(0x0D02, _L("Mc"), _L("MALAYALAM SIGN ANUSVARA"));				// Existing
	
	test_unicode_char(0x0B83, _L("Lo"), _L("TAMIL SIGN VISARGA"));					// Modified	
	test_unicode_char(0x0CBF, _L("Mn"), _L("KANNADA VOWEL SIGN I"));				// Modified
	test_unicode_char(0x0CC6, _L("Mn"), _L("KANNADA VOWEL SIGN E"));				// Modified
	}

void test_unicode_collation(const TDesC& x,const TDesC& y,TInt desired_order,
							TInt level = 3,const TCollationMethod* method = NULL)
	{
	TInt order = x.CompareC(y,level,method);

	// Normalise order to -1, 0 or 1.
	if (order < 0)
		order = -1;
	else if (order > 0)
		order = 1;

	test(order == desired_order);
	}

static const TUint32 TheSwedishKey[] = 
	{
	0x8f60109,0x8f70109,0x8f80109,0x8f60121,0x8f70121,0x8f80121,0x8dd0109,0x8dd0121,
	0x8c50121,0x8c50109,
	};

static const TUint32 TheSwedishIndex[] = 
	{
	0x570008,0x770009,
	};

static const TUint16 TheSwedishStringElement[] = 
	{
	0x2,0x61,0x30a,0x2,0x61,0x308,0x2,0x6f,
	0x308,0x2,0x41,0x30a,0x2,0x41,0x308,0x2,
	0x4f,0x308,0x2,0x75,0x308,0x2,0x55,0x308,
	};

static const TUint32 TheSwedishStringIndex[] = 
	{
	0xc0004,0x90003,0xf0005,0x150007,0x30001,0x0,0x60002,0x120006,
	};

static const TCollationKeyTable TheSwedishTable = 
	{ TheSwedishKey, TheSwedishIndex, 2, TheSwedishStringElement, TheSwedishStringIndex, 8 };

static const TCollationMethod TheSwedishMethod =
	{ 0, NULL, &TheSwedishTable, 0 };

static const TCollationMethod TheSwapCaseMethod =
	{ 0, NULL, NULL, TCollationMethod::ESwapCase };

static const TCollationMethod TheFoldCaseMethod =
	{ 0, NULL, NULL, TCollationMethod::EFoldCase };

static const TUint32 TheKanaKey[] = 
	{
	0xc949,0xca09,0x2010f,0x20113,0x4010b,0x4010f,0x5010b,0x5010f,
	0x8010b,0x8010f,0xa010f,0xa014b,0xc010b,0xc010f,0xe010b,0xe010f,
	0xe013f,0x10010b,0x10010f,0x12010b,0x12010f,0x14c90b,0x14c90f,0x16ca0b,
	0x16ca0f,0x17010b,0x18030f,0x1a060b,0x1a060f,0x1c160f,0x1e0d0b,0x1e0d0f,
	0x20290f,0x22020b,0x22020f,0x24010d,0x28010d,0x2c010d,0x2e010d,0x30010d,
	0x32010d,0x34010d,0x36010b,0x36010f,0x38010f,0x3a010f,0x3c0109,0x3c010d,
	0x3e0109,0x3e010d,0x40010d,0x400109,0x42010d,0x440109,0x44010d,0x46010d,
	0x48010d,0x4a010f,0x4c010f,0x4e010b,0x4f010b,0x50010b,0x50010f,0x52010b,
	0x52010f,0x54010b,0x56010b,0x58010b,0x58010f,0x5a010b,0x5a010f,0x5c010b,
	0x5c010f,0x5e010b,0x5e010f,0x60010b,0x62010b,0x64010b,0x66010b,0x68010f,
	0x68010b,0x6a010f,0x6a010b,0x6c010b,0x6e010b,0x6f010b,0x70010b,0x72010b,
	0x72010f,0x74010b,0x74010f,0x76010f,0x78010f,0x7a010f,0x7c010b,0x7c010f,
	0x80010b,0x80010f,0x82010b,0x82010f,0x84010f,0x86010f,0x88010f,0x8a010f,
	0x8c010b,0x8e010f,0x90010f,0x92010f,0x94010d,0x960109,0x96010d,0x980109,
	0x98010d,0x9a010d,0x9c0109,0x9c010d,0x9e010b,0x9e010f,0x100010b,0x100010f,
	0x102010b,0x102010f,0x104010b,0x104010f,0x106010b,0x106010f,0x107010f,0x108010f,
	0x109010f,0x10a010f,0x10b010f,0x10c010f,0x10d010f,0x10e010f,0x10f010f,0x110010f,
	0x111010f,0x112010f,0x113010f,0x114010f,0x115010f,0x116010f,0x117010f,0x118010f,
	0x119010f,0x11a010f,0x11b010f,0x11c010f,0x11d010f,0x11e010f,0x11f010f,0x120010f,
	0x121010f,0x122010f,0x123010f,0x124010f,0x125010f,0x126010f,0x127010f,0x128010f,
	0x129010f,0x12a010f,0x12b010f,0x12c010f,0x12d010f,0x12e010f,0x12f010f,0x130010f,
	0x131010f,0x132010f,0x133010f,0x134010f,0x135010f,0x136010f,0x137010f,0x138010f,
	0x139010f,0x13b010f,0x13c010d,0x13d010d,0x13e010f,0x13f010f,0x140010f,0x141010f,
	0x142010f,0x143010d,0x144010d,0x145010d,0x146010d,0x147010d,0x148010d,0x149010d,
	0x14a010d,0x14b010d,0x14c010d,0x14d010d,0x14e010d,0x14f010d,0x150010d,0x151010d,
	0x152010d,0x153010d,0x154010d,0x155010d,0x156010d,0x157010d,0x158010d,0x159010d,
	0x15a010d,0x15b010d,0x15c010d,0x15d010d,0x15e010d,0x15f010d,0x160010f,0x161010f,
	0x162010f,0x163010f,0x164010f,0x165010f,0x166010f,0x167010f,0x168010f,0x169010f,
	0x16a010f,0x16b010f,0x16c010f,0x16d010f,0x16e010f,0x16f010f,0x170010f,0x171010f,
	0x172010f,0x174010f,0x175010f,0x176010f,0x177010f,0x178010f,0x179010f,0x17a010d,
	0x17b010d,0x17c010d,0x17d0119,0x17e0119,0x17f0119,0x1800119,0x1810119,0x182010d,
	0x183010d,0x184010d,0x185010d,0x186010d,0x187010d,0x189010f,0x18a010f,0x18b010f,
	0x18c010f,0x18d0111,0x18e0111,0x18f0111,0x1900111,0x1910111,0x1920111,0x1930111,
	0x1940111,0x1950111,0x1960111,0x197010f,0x198010f,0x199010b,0x199010f,0x6c50109,
	0x6c60109,0x6c70109,0x6c80109,0x6c90109,0x6ca0109,0x6cb0109,0x6cc0109,0x6cd0109,
	0x6ce0109,0x6cf0109,0x6cf010d,0x6cf0121,0x6cf0125,0x6e30109,0x6e3010d,0x6e30121,
	0x6e30125,0x6fb0109,0x6fb010d,0x6fb0121,0x6fb0125,0x7070109,0x707010d,0x7070121,
	0x7070125,0x72f0109,0x72f010d,0x72f0121,0x72f0125,0x75b0109,0x75b010d,0x75b0121,
	0x75b0125,0x7630109,0x763010d,0x7630121,0x7630125,0x7830109,0x783010d,0x7830121,
	0x7830125,0x79d0109,0x79d010d,0x79d0121,0x79d0125,0x7b10109,0x7b1010d,0x7b10121,
	0x7b10125,0x7c10109,0x7c1010d,0x7c10121,0x7c10125,0x7cd0109,0x7cd010d,0x7cd0121,
	0x7cd0125,0x7f50109,0x7f5010d,0x7f50121,0x7f50125,0x7fd0109,0x7fd010d,0x7fd0121,
	0x7fd0125,0x8150109,0x815010d,0x8150121,0x8150125,0x82d0109,0x82d010d,0x82d0121,
	0x82d0125,0x8390109,0x839010d,0x8390121,0x8390125,0x8450109,0x845010d,0x8450121,
	0x8450125,0x86d0109,0x86d010d,0x86d0121,0x86d0125,0x8890109,0x889010d,0x8890121,
	0x8890125,0x8ad0109,0x8ad010d,0x8ad0121,0x8ad0125,0x8c50109,0x8c5010d,0x8c50121,
	0x8c50125,0x8d10109,0x8d1010d,0x8d10121,0x8d10125,0x8d90109,0x8d9010d,0x8d90121,
	0x8d90125,0x8dd0109,0x8dd010d,0x8dd0121,0x8dd0125,0x8e90109,0x8e9010d,0x8e90121,
	0x8e90125,0x10200139,0x10200135,0x10200149,0x10200141,0x10200145,0x1020013d,0x10260139,
	0x10260135,0x10260149,0x10260141,0x10260145,0x1026013d,0x102c0139,0x102c0135,0x102c0149,
	0x102c0141,0x102c0145,0x102c013d,0x10330139,0x10330135,0x10330149,0x10330141,0x10330145,
	0x1033013d,0x10390139,0x10390135,0x10390149,0x10390141,0x10390145,0x1039013d,0x103f0135,
	0x103f0145,0x103f013d,0x10440135,0x10440145,0x1044013d,0x10490135,0x10490145,0x1049013d,
	0x104e0135,0x104e0145,0x104e013d,0x10530135,0x10530145,0x1053013d,0x10580135,0x10580145,
	0x1058013d,0x105d0135,0x105d0145,0x105d013d,0x10620135,0x10620145,0x1062013d,0x10670135,
	0x10670145,0x1067013d,0x106c0135,0x106c0145,0x106c013d,0x10710135,0x10710145,0x1071013d,
	0x10760135,0x10760145,0x1076013d,0x107b0139,0x107b0135,0x107b0149,0x107b0141,0x107b0145,
	0x107b013d,0x10830135,0x10830145,0x1083013d,0x10880135,0x10880145,0x1088013d,0x108d0135,
	0x108d0145,0x108d013d,0x10900135,0x10900145,0x1090013d,0x10930135,0x10930145,0x1093013d,
	0x10960135,0x10960145,0x1096013d,0x10990135,0x10990145,0x1099013d,0x109c0135,0x109c0145,
	0x109c013d,0x10a30135,0x10a30145,0x10a3013d,0x10aa0135,0x10aa0145,0x10aa013d,0x10b10135,
	0x10b10145,0x10b1013d,0x10b80135,0x10b80145,0x10b8013d,0x10bf0135,0x10bf0145,0x10bf013d,
	0x10c20135,0x10c20145,0x10c2013d,0x10c50135,0x10c50145,0x10c5013d,0x10c80135,0x10c80145,
	0x10c8013d,0x10cb0135,0x10cb0145,0x10cb013d,0x10ce0139,0x10ce0135,0x10ce0149,0x10ce0141,
	0x10ce0145,0x10ce013d,0x10d40139,0x10d40135,0x10d40149,0x10d40141,0x10d40145,0x10d4013d,
	0x10da0139,0x10da0135,0x10da0149,0x10da0141,0x10da0145,0x10da013d,0x10e00135,0x10e00145,
	0x10e0013d,0x10e30135,0x10e30145,0x10e3013d,0x10e60135,0x10e60145,0x10e6013d,0x10e90135,
	0x10e90145,0x10e9013d,0x10ec0135,0x10ec0145,0x10ec013d,0x10ef0139,0x10ef0135,0x10ef0149,
	0x10ef0145,0x10ef013d,0x10f40135,0x10f40145,0x10f60135,0x10f60145,0x10f80135,0x10f80145,
	0x10f8013d,0x10fb0135,0x10fb0145,0x10fb013d,
	};

static const TUint32 TheKanaIndex[] = 
	{
	0x210013,0x22010d,0x230076,0x24006f,0x250074,0x260078,0x270019,0x28003d,
	0x29003f,0x2a007a,0x2b0057,0x2c0006,0x2d0059,0x2e0008,0x2f002e,0x30010f,
	0x310110,0x320111,0x330112,0x340113,0x350114,0x360115,0x370116,0x380117,
	0x390118,0x3a000c,0x3b000e,0x3c0060,0x3d005e,0x3e0062,0x3f0011,0x40007c,
	0x41011b,0x42011f,0x430123,0x440127,0x45012b,0x46012f,0x470133,0x480137,
	0x49013b,0x4a013f,0x4b0143,0x4c0147,0x4d014b,0x4e014f,0x4f0153,0x500157,
	0x51015b,0x52015f,0x530163,0x540167,0x55016b,0x56016f,0x570173,0x580177,
	0x59017b,0x5a017f,0x5b0043,0x5c0030,0x5d0045,0x5e001e,0x5f0021,0x60001b,
	0x610119,0x62011d,0x630121,0x640125,0x650129,0x66012d,0x670131,0x680135,
	0x690139,0x6a013d,0x6b0141,0x6c0145,0x6d0149,0x6e014d,0x6f0151,0x700155,
	0x710159,0x72015d,0x730161,0x740165,0x750169,0x76016d,0x770171,0x780175,
	0x790179,0x7a017d,0x7b0047,0x7c0035,0x7d0049,0x7e0033,0xa20071,0xa30072,
	0xa5006d,0xa7007e,0xa8001d,0xb00069,0xb1005b,0xb4001a,0xb600b7,0xd7005c,
	0xf7005d,0x2010002d,0x2015002c,0x20180039,0x2019003a,0x201c003b,0x201d003c,0x202000b5,
	0x202100b6,0x20250038,0x20260037,0x203000b1,0x2032006a,0x2033006b,0x203b008c,0x2103006c,
	0x211600ef,0x212100f1,0x216000cc,0x216100cd,0x216200ce,0x216300cf,0x216400d0,0x216500d1,
	0x216600d2,0x216700d3,0x216800d4,0x216900d5,0x21700101,0x21710102,0x21720103,0x21730104,
	0x21740105,0x21750106,0x21760107,0x21770108,0x21780109,0x2179010a,0x2190008f,0x21910090,
	0x2192008e,0x21930091,0x21d2009e,0x21d4009f,0x220000a0,0x220200a5,0x220300a1,0x220700a6,
	0x22080093,0x220b0094,0x221100fe,0x2212005a,0x221a00ab,0x221d00ad,0x221e0066,0x221f00ff,
	0x222000a2,0x22250034,0x2227009b,0x2228009c,0x2229009a,0x222a0099,0x222b00af,0x222c00b0,
	0x222e00fd,0x22340066,0x223500ae,0x223d00ac,0x225200a8,0x226100a7,0x22660064,0x22670065,
	0x226a00a9,0x226b00aa,0x22820097,0x22830098,0x22860095,0x22870096,0x22a500a3,0x22bf0100,
	0x231200a4,0x246000b9,0x246100ba,0x246200bb,0x246300bc,0x246400bd,0x246500be,0x246600bf,
	0x246700c0,0x246800c1,0x246900c2,0x246a00c3,0x246b00c3,0x246c00c4,0x246d00c5,0x246e00c6,
	0x246f00c7,0x247000c8,0x247100c9,0x247200ca,0x247300cb,0x25a00087,0x25a10086,0x25b20089,
	0x25b30088,0x25bc008b,0x25bd008a,0x25c60085,0x25c70084,0x25cb0081,0x25ce0083,0x25cf0082,
	0x25ef00b8,0x26050080,0x2606007f,0x26400068,0x26420067,0x266a00b4,0x266d00b3,0x266f00b2,
	0x30010003,0x30020005,0x30030025,0x30050027,0x30060028,0x30070029,0x3008004b,0x3009004c,
	0x300a004d,0x300b004e,0x300c004f,0x300d0051,0x300e0053,0x300f0054,0x30100055,0x30110056,
	0x3012008d,0x30130092,0x30140041,0x30150042,0x301c0032,0x301d00ed,0x301f00ee,0x30410181,
	0x30420182,0x30430187,0x30440188,0x3045018d,0x3046018e,0x30470193,0x30480194,0x30490199,
	0x304a019a,0x304b019f,0x304d01a2,0x304f01a5,0x305101a8,0x305301ab,0x305501ae,0x305701b1,
	0x305901b4,0x305b01b7,0x305d01ba,0x305f01bd,0x306101c0,0x306301c3,0x306401c4,0x306601c9,
	0x306801cc,0x306a01cf,0x306b01d2,0x306c01d5,0x306d01d8,0x306e01db,0x306f01de,0x307201e1,
	0x307501e4,0x307801e7,0x307b01ea,0x307e01ed,0x307f01f0,0x308001f3,0x308101f6,0x308201f9,
	0x308301fc,0x308401fd,0x30850202,0x30860203,0x30870208,0x30880209,0x3089020e,0x308a0211,
	0x308b0214,0x308c0217,0x308d021a,0x308e021d,0x308f021e,0x30900222,0x30910224,0x30920226,
	0x30930229,0x30990000,0x309a0001,0x309b0016,0x309c0018,0x309d0024,0x30a10183,0x30a20185,
	0x30a30189,0x30a4018b,0x30a5018f,0x30a60191,0x30a70195,0x30a80197,0x30a9019b,0x30aa019d,
	0x30ab01a0,0x30ad01a3,0x30af01a6,0x30b101a9,0x30b301ac,0x30b501af,0x30b701b2,0x30b901b5,
	0x30bb01b8,0x30bd01bb,0x30bf01be,0x30c101c1,0x30c301c5,0x30c401c7,0x30c601ca,0x30c801cd,
	0x30ca01d0,0x30cb01d3,0x30cc01d6,0x30cd01d9,0x30ce01dc,0x30cf01df,0x30d201e2,0x30d501e5,
	0x30d801e8,0x30db01eb,0x30de01ee,0x30df01f1,0x30e001f4,0x30e101f7,0x30e201fa,0x30e301fe,
	0x30e40200,0x30e50204,0x30e60206,0x30e7020a,0x30e8020c,0x30e9020f,0x30ea0212,0x30eb0215,
	0x30ec0218,0x30ed021b,0x30ee021f,0x30ef0220,0x30f00223,0x30f10225,0x30f20227,0x30f3022a,
	0x30fb000a,0x30fc002b,0x30fd0023,0x323100f7,0x323200f8,0x323900f9,0x32a400f2,0x32a500f3,
	0x32a600f4,0x32a700f5,0x32a800f6,0x330300dc,0x330d00e0,0x331400d7,0x331800da,0x332200d8,
	0x332300e2,0x332600e1,0x332700db,0x332b00e3,0x333600dd,0x333b00e5,0x334900d6,0x334a00e4,
	0x334d00d9,0x335100de,0x335700df,0x337b00ec,0x337c00fc,0x337d00fb,0x337e00fa,0x338f00e9,
	0x339c00e6,0x339d00e7,0x339e00e8,0x33a100eb,0x33c400ea,0x33cd00f0,0x4edd0026,0xfe540010,
	0xff010014,0xff02010e,0xff030077,0xff040070,0xff050075,0xff060079,0xff07010c,0xff08003e,
	0xff090040,0xff0a007b,0xff0b0058,0xff0c0007,0xff0d005a,0xff0e0009,0xff0f002f,0xff1a000d,
	0xff1b000f,0xff1c0061,0xff1d005f,0xff1e0063,0xff1f0012,0xff20007d,0xff21011c,0xff220120,
	0xff230124,0xff240128,0xff25012c,0xff260130,0xff270134,0xff280138,0xff29013c,0xff2a0140,
	0xff2b0144,0xff2c0148,0xff2d014c,0xff2e0150,0xff2f0154,0xff300158,0xff31015c,0xff320160,
	0xff330164,0xff340168,0xff35016c,0xff360170,0xff370174,0xff380178,0xff39017c,0xff3a0180,
	0xff3b0044,0xff3c0031,0xff3d0046,0xff3e001f,0xff3f0022,0xff40001c,0xff41011a,0xff42011e,
	0xff430122,0xff440126,0xff45012a,0xff46012e,0xff470132,0xff480136,0xff49013a,0xff4a013e,
	0xff4b0142,0xff4c0146,0xff4d014a,0xff4e014e,0xff4f0152,0xff500156,0xff51015a,0xff52015e,
	0xff530162,0xff540166,0xff55016a,0xff56016e,0xff570172,0xff580176,0xff59017a,0xff5a017e,
	0xff5b0048,0xff5c0036,0xff5d004a,0xff5e0032,0xff610004,0xff620050,0xff630052,0xff640002,
	0xff65000b,0xff660228,0xff670184,0xff68018a,0xff690190,0xff6a0196,0xff6b019c,0xff6c01ff,
	0xff6d0205,0xff6e020b,0xff6f01c6,0xff70002a,0xff710186,0xff72018c,0xff730192,0xff740198,
	0xff75019e,0xff7601a1,0xff7701a4,0xff7801a7,0xff7901aa,0xff7a01ad,0xff7b01b0,0xff7c01b3,
	0xff7d01b6,0xff7e01b9,0xff7f01bc,0xff8001bf,0xff8101c2,0xff8201c8,0xff8301cb,0xff8401ce,
	0xff8501d1,0xff8601d4,0xff8701d7,0xff8801da,0xff8901dd,0xff8a01e0,0xff8b01e3,0xff8c01e6,
	0xff8d01e9,0xff8e01ec,0xff8f01ef,0xff9001f2,0xff9101f5,0xff9201f8,0xff9301fb,0xff940201,
	0xff950207,0xff96020d,0xff970210,0xff980213,0xff990216,0xff9a0219,0xff9b021c,0xff9c0221,
	0xff9d022b,0xff9e0015,0xff9f0017,0xffe00071,0xffe10073,0xffe2009d,0xffe30020,0xffe4010b,
	0xffe5006e,
	};

/*
Collation keys for the Hiragana and Katakana syllabaries, built by
running coltab on \loce32\japanese\source\japanese_basekeys.txt after deleting all CJK characters.
*/
static const TCollationKeyTable TheKanaTable = 
	{ TheKanaKey, TheKanaIndex, 561, NULL, NULL, 0 };

static const TCollationMethod TheKanaMethod =
	{ 0, NULL, &TheKanaTable, 0 };

static const TCollationMethod TheSwapKanaMethod =
	{ 0, NULL, &TheKanaTable, TCollationMethod::ESwapKana };

static const TUint32 TheQazKey[] = 
	{
	0x6d00121,0x6e40121,
	};

static const TUint32 TheQazIndex[] = 
	{
	0
	};

static const TUint16 TheQazStringElement[] = 
	{
	0x2,0x53,0x45,0x3,0x53,0x45,0x58,
	};

static const TUint32 TheQazStringIndex[] = 
	{
	0x0,0x30001,
	};

static const TCollationKeyTable TheQazTable = 
	{ TheQazKey, TheQazIndex, 0, TheQazStringElement, TheQazStringIndex, 2 };

// A bizarre collation method that sorts 'SE' between A and B and 'SEX' between B and C
static const TCollationMethod TheQazMethod = 
	{ 0, NULL, &TheQazTable, 0 };

// A collation method to sort accents starting at the end of the string, as in France
static const TCollationMethod TheAccentsBackwardsMethod = 
	{ 0, NULL, NULL, TCollationMethod::EAccentsBackwards };

void test_unicode_collations()
	{
	/*
	Note: the curious method of getting values greater than 255 into the strings is needed
	because GCC doesn't handle the \x notation properly: it zeroes the high byte, so
	L"\x2011" puts the value 0x0011, not 0x2011, into the string.
	*/
	TBuf<32> x, y;

	test.Next(_L("collation"));
	test_unicode_collation(_L("SEZ"),_L("BZ"),-1,3,&TheQazMethod); // test 2-character combination 'SE'
	test_unicode_collation(_L("SEXY"),_L("CY"),-1,3,&TheQazMethod); // test 3-character combination 'SEX'
	test_unicode_collation(_L("Graham"),_L("Graham"),0);		// equality
	test_unicode_collation(_L("something"),_L(""),1);			// something after nothing
	test_unicode_collation(_L("inform"),_L("information"),-1);	// prefixes come first
	test_unicode_collation(_L("unicode"),_L("consortium"),1);	// standard alphabetic ordering: lowercase
	test_unicode_collation(_L("AARON"),_L("AARDVARK"),1);		// standard alphabetic ordering: uppercase
	test_unicode_collation(_L("AARON"),_L("AARDVARK"),1,0);		// alphabetic ordering is significant at level 0
	test_unicode_collation(_L("hello"),_L("Hello"),-1,3);		// lowercase before uppercase
	test_unicode_collation(_L("hello"),_L("Hello"),-1,3,&TheSwapKanaMethod);// swapping kana ordering doesn't affect case
	test_unicode_collation(_L("hello"),_L("Hello"),1,3,&TheSwapCaseMethod); // uppercase before lowercase if ESwapCase used
	test_unicode_collation(_L("hello"),_L("Hello"),0,2,&TheFoldCaseMethod); // uppercase = lowercase if EFoldCase used at level 2
	test_unicode_collation(_L("hello"),_L("Hello"),-1,2);		// case is significant at level 2
	test_unicode_collation(_L("hello"),_L("Hello"),0,1);		// ignore case at level 1
	test_unicode_collation(_L("hello"),_L("[H E L L O !]"),0,1);// ignore punctuation and case at level 1
	test_unicode_collation(_L("1234"),_L("abc"),-1);			// digits before letters
	x = _L("week day"); x[4] = 0x2011;							// (week-day with non-breaking hyphen)
	test_unicode_collation(x,_L("weekend"),-1);					// ignore punctuation if letters differ; d < e
	test_unicode_collation(x,_L("week-day"),1);					// compare punctuation if otherwise equal
	test_unicode_collation(x,_L("week-day"),0,2);				// ignore difference between hyphens at level 2
	test_unicode_collation(x,_L("Week-day"),-1);				// case difference outweighs punctuation ordering
	y = _L("a ngstro m"); y[1] = 0x30a; y[8] = 0x308;			// (a<ring>ngstro<umlaut>m with piece accents)
	test_unicode_collation(_L("\xe5ngstr\xf6m"),y,0);			// base+accent = precomposed (a<ring>ngstro<umlaut>m)
	test_unicode_collation(_L("Liege"),_L("Li\xe8ge"),-1);		// unaccented before accented...
	test_unicode_collation(_L("Liege"),_L("Li\xe8ge"),-1,1);	// accents are significant at level 1
	test_unicode_collation(_L("Liege"),_L("li\xe8ge"),-1);		// accentuation outweighs case
	test_unicode_collation(_L("Liege"),_L("li\xe8ge"),0,0);		// ignore accents and case at level 0
	test_unicode_collation(_L("\xe5ngstr\xf6m"),_L("zoo"),-1);	// a-ring before z by default
	test_unicode_collation(_L("\xe5ngstr\xf6m"),_L("zoo"),1,3,&TheSwedishMethod);	// a-ring after z in Sweden
	test_unicode_collation(_L("Antidisestablishmentarian"),_L("antidisestablishmentariac"),1);
																// long strings that don't have identical prefixes
	x = _L("\x304b\x306a"); y = _L("\x30ab\x30ca");				// 'kana' in Hiragana and Katakana
	test_unicode_collation(x,y,-1,3,&TheKanaMethod);			// Hiragana before Katakana 
	test_unicode_collation(x,y,1,3,&TheSwapKanaMethod);			// Katakana before Hiragana if ESwapKana used
	x = _L("p\xe9"L"ch\xe9"); y = _L("p\xea"L"che");			// x = pe<acute>che<acute>, y = pe<circumflex>che
	test_unicode_collation(x,y,-1,3);							// normal order; accents compared from start
	test_unicode_collation(x,y,1,3,&TheAccentsBackwardsMethod);	// reverse order; accents compared from end
	test_unicode_collation(_L("aaAA"),_L("aAaA"),0,3,&TheFoldCaseMethod); // case folding should work even at level 3
	test_unicode_collation(_L("\x2113"),_L("L"),1,3,&TheFoldCaseMethod); // script l and L should not be equal;
																// script l before L because L folds to lower case,
																// which sorts after secondary key type for script l
	test_unicode_collation(_L("\x3c2"),_L("\x3c3"),-1,3); // Greek final and non-final sigma
	test_unicode_collation(_L("\x3c2"),_L("\x3c3"),0,3,&TheFoldCaseMethod); // Greek final and non-final sigma
	test_unicode_collation(_L("\xee0"),_L("\x4e00"),1,3);		// undefined character after CJK character not in table
	test_unicode_collation(_L("\xee0"),_L("\xef0"),-1,3);		// undefined characters must differ
	test_unicode_collation(_L("\xee0\xee1"),_L("\xef0"),-1,3);	// undefined characters in Unicode order, not string length
	test_unicode_collation(_L("\xd800\xdc00"),_L("\xceff"),1,3);	// character U+10000 (using surrogates) 
																// after Hangul character not in table
	}

GLDEF_C TInt E32Main(void)
    {

	test.Title();
	test.Start(_L("TCHAR TESTING"));

	test_unicode_chars();
	test_unicode_collations();

	test.Next(_L("TChar"));
	TestTChar<TChar> t;
	t.Test1();
	t.Test2(eTChar);
	t.Test3(eTChar);
	t.Test4(eTChar);

	test.Next(_L("TCharF"));
	TestTChar<TCharF> f;
	f.Test2(eTCharF);
	f.Test3(eTCharF);
	f.Test4(eTCharF);

	test.Next(_L("TCharLC"));
	TestTChar<TCharLC> lc;
	lc.Test2(eTCharLC);
	lc.Test3(eTCharLC);
	lc.Test4(eTCharLC);

	test.Next(_L("TCharUC"));
  	TestTChar<TCharUC> uc;
	uc.Test2(eTCharUC);
	uc.Test3(eTCharUC);
	uc.Test4(eTCharUC);

	test.End();
	return(KErrNone);
    }

#pragma warning ( default :4700)
#pragma warning ( default :4710) 
