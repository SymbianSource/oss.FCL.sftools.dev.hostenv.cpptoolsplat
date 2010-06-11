// Copyright (c) 1996-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// f32\sfsrv\cl_find.cpp
// 
//

#include "cl_std.h"

EXPORT_C TFindFile::TFindFile(RFs& aFs)
	: iFs(&aFs), iPathPos(0), iCurrentDrive(0), iMode(-1), iMatchMask(0)
/**
Constructor taking a file server session.

@param aFs File server session.
*/
	{
	
	iFile.Set(_L(""),NULL,NULL);
	}

TInt TFindFile::DoFindByPath(const TDesC& aFileName,const TDesC* aPath)
//
// Look for a file in each directory in the path
// Make initial check for aFileName in the current directory
//
	{
	TUint att;
	
#ifdef _DEBUG_LOGGING
	RDebug::Print(_L("DEBUG: FindByPath> aFileName=%S id=%x\n"), &aFileName);
#endif
	
	// At the moment this is just looking for an exact match for the file
	// or does the file exist in the current directory
	iFile.Set(aFileName, NULL, NULL);
	if (iFs->Att(iFile.FullName(), att) == KErrNone)
		return KErrNone;
	
	iFile.Set(iFile.NameAndExt(), NULL, NULL);
	return iFs->Att(iFile.FullName(), att);
	}

EXPORT_C TInt TFindFile::FindByPath(const TDesC& aFileName,const TDesC* aPath)
/**
Searches for a file/directory in one or more directories in the path.

The search ends when the file/directory is found, or when every directory
specified in the path list has been unsuccessfully searched.

Notes:
	
1. For the equivalent search using wildcards, use FindWildByPath().

2. After a file has been found, use TFindFile::File() to get the fully qualified path of the file. To
   search for the next occurrence, use TFindFile::Find().

@param aFileName The filename to search for. If this specifies a directory as well
                 as a filename, then that directory is searched first.
@param aPath     A list of directories to be searched. Paths in this list must
                 be separated by a semicolon character, but a semicolon is not
                 required after the	final path. The directories are searched in
                 the order in which they occur in the list. If a path in
                 the list contains a drive letter, that drive alone is searched.
                 If a path contains no drive letter, the function searches for
                 the file in that directory on every available drive in turn,
                 beginning with drive Y:, in descending alphabetical order
                 and ending with drive Z:.When path is empty then session path 
                 will be used for the search.

@return KErrNone, if the filename was found;
        KErrNotFound, if the filename was not found.
        KErrArgument, if the filename is empty. 

@see TFindFile::FindWildByPath
@see TFindFile::File
@see TFindFile::Find
*/
	{

	iDir=NULL;
	return(DoFindByPath(aFileName,aPath));
	}
