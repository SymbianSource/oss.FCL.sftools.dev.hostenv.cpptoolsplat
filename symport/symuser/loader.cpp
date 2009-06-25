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
//

#include <stdio.h>

#include <e32ldr.h>
#include <u32std.h>
#include <e32std.h>

#ifndef __linux__
#include <windows.h>
#include <winbase.h>

// Windows version of the function
TInt RLoader::LoadLocale(const TDesC& aLocaleDllName, TLibraryFunction* aExportList)
	{
	typedef int (__cdecl *MYPROC)(LPWSTR);
	MYPROC myFunc;
	TBuf<256> nameBuf;
	nameBuf.Copy(aLocaleDllName);
	nameBuf.ZeroTerminate();

	iHandle = (TAny*)LoadLibraryEx((const WCHAR*)(nameBuf.Ptr()), NULL, DONT_RESOLVE_DLL_REFERENCES|LOAD_WITH_ALTERED_SEARCH_PATH);
	if ( iHandle == NULL )
		{
		DWORD err = GetLastError();
		printf("Error loading locale: Win system error %lu", (unsigned long)err);
		return KErrGeneral;
		}

	for ( TInt i = 1 ; i < KNumLocaleExports ; i ++ )
		{
		myFunc = (MYPROC) GetProcAddress((HMODULE)iHandle, (LPCSTR)i);
		if ( myFunc != NULL )
			{
			aExportList[i] = (TLibraryFunction)myFunc;
			}
		else
			{
			DWORD err = GetLastError();
			printf("Error loading locale function: Win system error %lu", (unsigned long)err);
			FreeLibrary((HMODULE)iHandle);
			return KErrGeneral;
			}
		}
	return KErrNone;
	}

void RLoader::Free()
	{
	if (iHandle != NULL)
		{
		FreeLibrary((HMODULE)iHandle);
		iHandle = NULL;
		}
	}

#else

#include <dlfcn.h>

// Mangled function names
const char funcnames [22][60] =
	{
	"Dummy",
	"_ZN4Locl9AmPmTableEv",
	"_ZN4Locl7CharSetEv",
	"_ZN4Locl9CollTableEv",
	"_ZN4Locl14CurrencySymbolEv",
	"_ZN4Locl15DateSuffixTableEv",
	"_ZN4Locl11DayAbbTableEv",
	"_ZN4Locl8DayTableEv",
	"_ZN4Locl9FoldTableEv",
	"_ZN4Locl8LanguageEv",
	"_ZN4Locl10LocaleDataEP11SLocaleData",
	"_ZN4Locl10LowerTableEv",
	"_ZN4Locl13MonthAbbTableEv",
	"_ZN4Locl10MonthTableEv",
	"_ZN4Locl8MsgTableEv",
	"_ZN4Locl9TypeTableEv",
	"_ZN4Locl7UniCodeEv",
	"_ZN4Locl10UpperTableEv",
	"_ZN4Locl19ShortDateFormatSpecEv",
	"_ZN4Locl18LongDateFormatSpecEv",
	"_ZN4Locl19FatUtilityFunctionsEv",
	};

// Linux version of the function
TInt RLoader::LoadLocale(const TDesC& aLocaleDllName, TLibraryFunction* aExportList)
	{
	TBuf8<256> nameBuf;
	nameBuf.Copy(aLocaleDllName);
	const char* name = reinterpret_cast<const char*>(nameBuf.PtrZ());

	iHandle = dlopen(name, RTLD_NOW);
	TLibraryFunction myFunc;

	if(iHandle == NULL) {
		printf("Error loading locale: %s\n", dlerror());
		return KErrGeneral;
	}

	for ( TInt i = 1 ; i < KNumLocaleExports ; i ++ )
		{
		myFunc = (TLibraryFunction) dlsym(iHandle, funcnames[i]);
		if ( myFunc != NULL )
			{
			aExportList[i] = (TLibraryFunction)myFunc;
			}
		else
			{
			printf("Error loading locale function: %s\n", dlerror());
			dlclose(iHandle);
			return KErrGeneral;
			}
		}
	return KErrNone;
	}

void RLoader::Free()
	{
	if ( iHandle != NULL )
		{
		dlclose(iHandle);
		iHandle = NULL;
		}
	}

#endif

