// Copyright (c) 1996-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// f32\inc\f32file.inl
// 
//




// Class TEntry
inline const TUid& TEntry::operator[](TInt anIndex) const
/**
Gets any one of the file's three UIDs.

@param anIndex Identifies the UID required. This can be zero, one or 
               two. Specifiying any other value raises a panic.

@return On return, contains the requested UID.

@see TUidType
*/
    {return(iType[anIndex]);}




inline TBool TEntry::IsUidPresent(TUid aUid) const
/**
Tests whether the specified UID matches any of the UIDs in the UID type.

@param aUid The UID to be checked.

@return True if the specified UID is present, false otherwise.

@see TUidType::IsPresent
@see TUidType
*/
    {return(iType.IsPresent(aUid));}




inline TBool TEntry::IsTypeValid() const
/**
Test whether the file has a valid UID.

@return True if the entry has a valid UID, false otherwise. 

@see TUidType::IsValid  
@see TUidType
*/
    {return(iType.IsValid());}




inline TUid TEntry::MostDerivedUid() const
/**
Gets the most derived (i.e. the most specific) UID.

@return The entry's most derived UID.

@see TUidType::MostDerived
@see TUidType
*/
    {return(iType.MostDerived());}




//	Returns the entire size of the TEntry...
inline TInt EntrySize(const TEntry& anEntry)
	{return(sizeof(TUint)+sizeof(TInt)+sizeof(TTime)+sizeof(TInt)+sizeof(TUidType)+anEntry.iName.Size());}




// Class TFindFile
inline const TDesC& TFindFile::File() const
/**
Gets the full file specification of a file which was found by a successful 
call to any of the search variants that do not accept wildcards.

The file specification includes drive, path and filename.

Notes:

1. When called after a successful search using wildcards the only valid 
   components of the retrieved file specification are drive letter and 
   directory.
   
@return The full path and filename.
*/
	{return iFile.FullName();}



// Class CDirScan
inline RFs& CDirScan::Fs()
	{return(*iFs);}




// Class TDriveUnit
inline TDriveUnit::operator TInt() const
/**
Converts the drive unit to an integer value.
*/
	{return(iDrive);}




inline TInt ValidateMatchMask( TUint aMask)
/**
Validates the mask used to match drive attributes.
*/
	{
	const TUint matchedFlags= aMask & KDriveAttMatchedFlags;  //KDriveAttMatchedFlags = 0xFFF
	const TUint matchedAtt = aMask & KDriveAttMatchedAtt;	 //KDriveAttMatchedAtt = 0x0FFF0000
	
	switch(matchedAtt)
		{
		case KDriveAttExclude:
			return matchedFlags==0?KErrArgument:KErrNone;
		case KDriveAttExclusive :
			return matchedFlags==0?KErrArgument:KErrNone;
		case KDriveAttExclude | KDriveAttExclusive:
			return matchedFlags==0?KErrArgument:KErrNone;
		case KDriveAttAll:
			return matchedFlags==0?KErrNone:KErrArgument;
		case 0:
			return KErrNone;
		default:
			return KErrArgument;										
		}
	}	

inline void RFs::Close(){}
