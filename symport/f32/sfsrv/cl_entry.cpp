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
// f32\sfsrv\cl_entry.cpp
// 
//

#include "cl_std.h"


/**
Default constructor.
*/
EXPORT_C TVolumeInfo::TVolumeInfo()
	{
    Mem::FillZ(this, sizeof(TVolumeInfo)); //-- zero-fill itself
    new(&iName)TBufC<KMaxFileName>;        //-- initialise broken descriptor 
    }




EXPORT_C TBool TEntry::IsReadOnly() const
/**
Tests whether the file or directory is read-only.

@return ETrue if entry is read-only, EFalse if not.

@see KEntryAttReadOnly
*/
	{
	return(iAtt&KEntryAttReadOnly);
	}




EXPORT_C TBool TEntry::IsHidden() const
/**
Tests whether the file or directory is hidden.

@return ETrue if entry is hidden, EFalse if not.

@see KEntryAttHidden
*/
	{

	return(iAtt&KEntryAttHidden);
	}




EXPORT_C TBool TEntry::IsSystem() const
/**
Tests whether the file or directory has the system attribute set.

@return ETrue if entry is a system entry, EFalse if not.

@see KEntryAttSystem
*/
	{

	return(iAtt&KEntryAttSystem);
	}




EXPORT_C TBool TEntry::IsDir() const
/**
Tests whether the entry is a directory.

@return ETrue if entry indicates a directory, EFalse if not.

@see KEntryAttDir
*/
	{

	return(iAtt&KEntryAttDir);
	}




EXPORT_C TBool TEntry::IsArchive() const
/**
Tests whether the file is an archive file.

@return ETrue if file is archive, EFalse if not.

@see KEntryAttArchive
*/
	{

	return(iAtt&KEntryAttArchive);
	}




EXPORT_C TEntryArray::TEntryArray()
	: iCount(0)
/**
Default constructor.

Initialises its count of contained TEntry objects to zero.
*/
	{}




EXPORT_C TInt TEntryArray::Count() const
/**
Gets the number of entries in the array.

@return The number of entries in the array.
*/
	{

	if (iCount==KCountNeeded)
		{
		const TEntry* pE=(const TEntry*)iBuf.Ptr();
		const TEntry* pEnd=PtrAdd(pE,iBuf.Length());
		TInt c=0;
		while (pE<pEnd)
			{
			c++;
			pE=PtrAdd(pE,Align4(EntrySize(*pE)));
			}
		TEntryArray& me=(TEntryArray& )(*this);
		me.iCount=c;
		me.iIndex=0;
		me.iPos=(const TEntry*)iBuf.Ptr();
		}
	return(iCount);
	}




EXPORT_C const TEntry& TEntryArray::operator[](TInt anIndex) const
/**
Gets the directory entry at the specified index.

@param anIndex Index of the entry within the array.
               This value is relative to zero.
               
@return On return contains the entry at the specified index.

@panic FSCLIENT 22 if anIndex is greater than or equal to the number
       of elements in the array.
*/
	{

	__ASSERT_ALWAYS(anIndex<Count(),Panic(EEntryArrayBadIndex));
	const TEntry* pE=iPos;
	TInt ix=iIndex;
	if (anIndex<ix)
		{
		ix=0;
		pE=(const TEntry*)iBuf.Ptr();
		}
	while (ix<anIndex)
		{
		pE=PtrAdd(pE,Align4(EntrySize(*pE)));
		ix++;
		}
	TEntryArray& me=(TEntryArray& )(*this);
	me.iIndex=ix;
	me.iPos=pE;
	return(*pE);
	}




EXPORT_C TEntry::TEntry()
/**
Default constructor.
*/
	{}




EXPORT_C TEntry::TEntry(const TEntry& aEntry)
/**
Copy constructor.

@param aEntry The TEntry object to be copied.
*/
	{
	Mem::Copy(this,&aEntry,EntrySize(aEntry));
	}




EXPORT_C TEntry& TEntry::operator=(const TEntry& aEntry)
/**
Assignment operator.

@param aEntry The TEntry object to be copied to this TEntry object.

@return A reference to this TEntry object.
*/
	{
	if(this!=&aEntry)
		Mem::Copy(this,&aEntry,EntrySize(aEntry));
	return(*this);
	}
