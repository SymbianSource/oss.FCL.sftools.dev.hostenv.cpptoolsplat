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
// f32\sfsrv\cl_cli.cpp
// 
//

#include <f32file.h>
#include <e32debug.h>

#include "cl_std.h"

#ifdef _WIN32
#include <cstdio>
#include <io.h>
#include <sys/stat.h>
#include <string>
#else
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include <stdlib.h>
#endif

// ***
// Map a Posix error to a Symbian error code
//
TInt mapErrSwitch(TInt aErr)
	{
	switch (aErr)
		{
	case ENOENT:
		return KErrNotFound;
	case EACCES:
	case EPIPE:
	case EROFS:
		return KErrAccessDenied;
	case EINVAL:
	case EISDIR:
	case ENOTDIR:
		return KErrArgument;
	case ENOTEMPTY:
	case EBUSY:
		return KErrInUse;
	case EEXIST:
		return KErrAlreadyExists;
	case EBADF:
		return KErrBadHandle;
	case ENOMEM:
		return KErrNoMemory;
	case EFBIG:
		return KErrTooBig;
	default:
		return -aErr;
		}
	return aErr;
	}

// ***
// Map a Posix error to a Symbian error code
//
TInt mapErr(TInt aErr)
	{
	TInt symErr = mapErrSwitch(aErr);

#ifdef _DEBUG_LOGGING
	if (aErr != KErrNone)
		RDebug::Print(_L("DEBUG: mapErr> %d => %d\n"), aErr, symErr);
#endif
	return symErr;
	}

#ifdef _WIN32
const char KNativeSlashChar = '\\';
#else
const char KNativeSlashChar = '/';
#endif

// ***
// Is the character a Symbian path separator?
//
TBool IsPathSep(TChar aChar)
	{
	return aChar == TChar('\\');
	}

// ***
// Perform posix filename conversions
//
const char* PosixFilename(const TDesC &aIn, TDes8& aOut)
	{
	aOut.Zero();
	for(TInt pos = 0; pos < aIn.Length(); pos++)
		{
		TChar c(aIn[pos]);

		// Magic for using epocroot
		if (pos == 0 && aIn.Length() >= 7 && IsPathSep(aIn[0]) && aIn.Mid(1, 6).CompareF(_L("epoc32")) == 0)
			{
			char* epocroot = getenv("EPOCROOT");
			if (epocroot)
				{
				aOut.Append(TPtrC8(reinterpret_cast<const TUint8*>(epocroot), strlen(epocroot)));

				// If the last character of EPOCROOT is not a slash then we have to add one
				if (aOut[aOut.Length() - 1] != KNativeSlashChar)
					aOut.Append(KNativeSlashChar);

				continue;
				}
			}

#ifndef _WIN32
		// Drive letters ignored - go back home!
		if (pos == 0 && aIn[1] == ':' && IsPathSep(aIn[2]))
			{
			char* home = getenv("HOME");
			if (home)
				aOut.Append(TPtrC8(reinterpret_cast<const TUint8*>(home), strlen(home)));
			pos++;
			continue;
			}

		// Convert backslashes to forward slashes
		if (c == '\\')
			{
			aOut.Append('/');
			continue;
			}

		// Lower case everything
		c = c.GetLowerCase();
#endif
		aOut.Append(c);
		}

	const char* ret = reinterpret_cast<const char*>(aOut.PtrZ());
#ifdef _DEBUG_LOGGING
    RDebug::Printf("DEBUG: PosixFilename> %s\n", ret);
#endif
	return ret;
	}
TBuf8<0x200> gFilenameBuf;
const char* PosixFilename(const TDesC &aIn)
	{
	// Return the result
	return PosixFilename(aIn, gFilenameBuf);
	}

EXPORT_C TBool RFs::IsValidDrive(TInt aDrive)
/**
Tests whether the specified drive number is valid.

A valid drive number is any number between 0 and (KMaxDrives-1) inclusive,
or the specific value KDefaultDrive (implying the session default drive).

@param aDrive The drive number.
			
@return True if the drive is valid; false if not.				

@see TDriveNumber 				
*/
	{
	return((aDrive>=0 && aDrive<KMaxDrives) || aDrive==KDefaultDrive);
	}

EXPORT_C TInt RFs::CharToDrive(TChar aChar,TInt& aDrive)
/**
Maps a drive character to a drive number.

The drive character must be in the range A to Z or a to z. For example, drive A (or a)
corresponds to zero, drive B (or b) corresponds to 1 etc. For the drive number
enumeration, see TDriveNumber.

@param aChar  The drive character.
@param aDrive On return, contains the drive number.

@return KErrNone, if successful;
        KErrArgument, if the drive character is not in the range A to Z or a to z.
        
@see TDriveNumber        
*/
	{

	aChar.UpperCase();
	if (aChar>='A' && aChar<='Z')
		{
		aDrive=(TInt)aChar-'A';
		return(KErrNone);
		}
	return(KErrArgument);
	}

EXPORT_C TInt RFs::DriveToChar(TInt aDrive,TChar& aChar)
/**
Maps a drive number to the corresponding character.

The drive number must be in the range 0 to (KMaxDrives-1). For example, drive
number zero (EDriveA) corresponds to drive A, one (EDriveB)
corresponds to drive B. For the drive number enumeration, see TDriveNumber.

The drive number can also be KDefaultDrive, implying the default drive. In this
case the current drive is taken and converted.

@param aDrive The drive number.
@param aChar  On return, contains the drive character.

@return KErrNone, if successful;
        KErrArgument, if the drive number is invalid;
        otherwise one of the other system-wide error codes.
*/
	{

	if (aDrive==KDefaultDrive)
		{

#ifdef _WIN32
		RFs fs;
		TFileName path;
		TInt r=fs.Connect();
		if (r!=KErrNone)
			return(r);
		r=fs.SessionPath(path);
		fs.Close();
		if (r!=KErrNone)
			return(r);

		aChar=path[0];
		return(KErrNone);

#else

		aChar='C';

		return KErrNone;

#endif
		}
	if (!IsValidDrive(aDrive))
		return(KErrArgument);
	aChar=aDrive+'A';
	return(KErrNone);
	}

/** 
Obtain the system drive number.
 
The System Drive is a defined drive on the device which is:
 - Read/Writeable
 - Internal: Always available and not removable from the device
 - Non-Volatile (e.g. Flash memory, battery-backed RAM)
 - Only Accessible via Rfs (e.g. not available via USB mass storage)
     
The System drive is utilised as:
 - Storage for Persistent settings from system and application software
 - Storage for Localisation resources
 - A Default Drive for user data
 - A Target Drive for Software installations

It the system drive is not set previously (see RFs::SetSystemDrive) EDriveC is returned by default.
 
@see RFs::GetSystemDriveChar
@see RFs::SetSystemDrive   
@see TDriveNumber
@return TDriveNumber contains the drive number of the system drive.
 */
EXPORT_C TDriveNumber RFs::GetSystemDrive()
	{
	return EDriveC;
	}

/**
This is a wrapper around GetSystemDrive() function. It returns the character corresponding to the system drive.

@parameter aDriveChar On return, contains the system drive character
@return KErrNone if successful, otherwise one of the other system-wide error codes
@see RFs::GetSystemDrive
*/
EXPORT_C TChar RFs::GetSystemDriveChar()
	{
	return 'A' + GetSystemDrive();
	}

EXPORT_C TInt RFs::Connect(TInt)
/**
Connects a client to the file server.

To end the file server session, use Close().

@param aMessageSlots The number of message slots required. The default value of
				     KFileServerDefaultMessageSlots indicates that message
				     slots will be acquired dynamically from the system
				     wide pool. Override this value with a fixed number, if
				     a fixed number of slots are to be allocated to the session.
				     If overriding, note that the number of message slots
				     represents the number of operations, such as reads
				     and writes, that can be outstanding at once;
				     always remember to provide a spare slot for
				     the cancel operation.

@return KErrNone, if successful, otherwise one of the other system-wide
        error codes.
*/
	{
	return KErrNone;
	}

EXPORT_C TInt RFs::Drive(TDriveInfo& anInfo, TInt aDrive) const
/**
Gets information about a drive and the medium mounted on it.

Note that Volume() can also be used to give information about the drive and
the volume mounted on it. These two functions are separate because, while
the characteristics of a drive cannot change, those of a
volume can, by mounting different media, reformatting etc.
			 
@param anInfo On return, contains information describing the drive
			  and the medium mounted on it. The value of TDriveInfo::iType
			  shows whether the drive contains media.
@param aDrive The drive for which information is requested.
              Specify KDefaultDrive for the session default drive.
			  Specify a drive in the range EDriveA to EDriveZ for drives
			  A to Z respectively.

@return       KErrNone, if successful, otherwise one of the other
              system-wide error codes.
			 
@see RFs::Volume
*/
	{
	if (aDrive != EDriveC && aDrive != KDefaultDrive)
		return KErrBadName;
	
	anInfo.iType = EMediaHardDisk;
	anInfo.iBattery = EBatNotSupported;
	anInfo.iDriveAtt = KDriveAttLocal|KDriveAttInternal;
	anInfo.iMediaAtt = 0;
	
	return KErrNone;
	}

EXPORT_C TInt RFs::SessionPath(TDes& aPath) const
/**
Gets the session path.

When a client connects to the file server, its session path is initialised to
the system default path. The session path of an existing client can only be
changed by this function.

@param aPath On return, contains the session path, including a trailing
             backslash.

@return KErrNone if successful, otherwise one of the other
        system-wide error codes.
*/
	{

#ifdef _DEBUG_LOGGING

    RDebug::Print(_L("DEBUG: SessionPath\n"), &aPath);

#endif


	TBuf8<KMaxFileName + 1> copy;
	copy.Copy(aPath.Left(KMaxFileName));

	// As the system for the current work directory
	const char* path = reinterpret_cast<const char*>(copy.PtrZ());
	if (getcwd(const_cast<char*>(path), copy.MaxLength() - 1) == NULL)
		return mapErr(errno);
	
	// Get the length of the data
	TInt len = strlen(path);	
	copy.SetLength(len);

	// We need one extra character of space for the backslash on the end
	if (len == KMaxFileName)
		return KErrOverflow;

	// Make sure we use backslash rather than forward 
	while(len--)
		{
		if (copy[len] == '/')
			copy[len] = '\\';
		}
	
	// Copy the data and append a back slash, as Symbian expects it
	aPath.Copy(copy);
	aPath.Append(TChar('\\'));
	
	return KErrNone;
	}

EXPORT_C TInt RFs::SetSessionPath(const TDesC& aPath)
/**
Sets the session path for the current file server client.

When the client first connects to the file server, its session path
is initialised to the system default path.

Note that the session path is text-only. It does not cause any locking.
Thus, although the path must be syntactically correct, its components
do not need to be valid at the time the path is set, and any component may be
deleted, removed or unmounted while the path is set.

On TOOLS2 this function modifies the session path for ALL RFs sessions.

@param aPath The new session path. Consists of a drive and path. Normally, a
             drive should be specified, but if not, the drive specified in
             the existing session path is preserved. If a file is specified,
             then the function fails and returns an error code. Therefore,
             the final component in the path must have a trailing backslash
             to indicate that it is a directory. All components of the
             path must be syntactically correct, for example, wildcard
             characters and double backslashes are not allowed in any
             part of it.

@return KErrNone if successful, otherwise one of the other
        system-wide error codes.

@capability Dependent If aPath is /Sys then AllFiles capability is required.
@capability Dependent If aPath begins with /Private and does not match this process' SID
					  then AllFiles capability is required.

*/
	{

#ifdef _DEBUG_LOGGING

    RDebug::Print(_L("DEBUG: SetSessionPath> path=%S\n"), &aPath);

#endif


	const char* name = PosixFilename(aPath);
	if (chdir(name) != 0)
		return mapErr(errno);
	return KErrNone;
	}

/**
Makes a directory.

It should be a sub-directory of an existing	directory and its name should be
unique within its parent directory, otherwise the function returns error code KErrAlreadyExists.
				
Note that if a filename is specified in the argument, it is	ignored.
Therefore, there should be a trailing backslash after the final
directory name in the argument to indicate that it is a directory, not a filename. 

For example, following code will create directory "C:\\DIR1\\"
   
@code
fs.MkDir(_L("C:\\DIR1\\"));
@endcode

The last line in the following example will result in KErrAlreadyExists because "DIR2" doesn't have a trailing backslash, 
therefore is considered as a file name and discarded. Directory "C:\\DIR1\\" has already been created.

@code
fs.MkDir(_L("C:\\DIR1\\"));     // shall create DIR1 in the root directory
fs.MkDir(_L("C:\\DIR1\\DIR2")); // No trailing backslash, fails with KErrAlreadyExists
@endcode

This example will always fail because "DIR1" doesn't have a trailing backslash and discarded while the root
directory always exists. 

@code
fs.MkDir(_L("C:\\DIR1"));  // No trailing backslash, will always fail with KErrAlreadyExists
@endcode

Note, the following case

@code
fs.MkDir(_L("C:\\example.txt\\"));	// would normally create a directory "c:\\example.txt\\" with KErrNone
@endcode
 
But if there is a file named "example.txt", which exists at the same location, KErrAccessDenied is returned.    

Note also that because this method can return an error code (eg. because
the disk is full) before checking whether the path already exists, it
is not appropriate to use it just to work out whether a path exists or not.

See MkDirAll(), which may also create intermediate directories.

@param aPath The name of the new directory. Any path components which are
             not specified here will be taken from the session path.

@return KErrNone if successful, otherwise one of the other
        system-wide error codes. Even if another error code is returned,
		(for example, if the disk is full) it is still possible that the 
		path may already exist.

@capability Dependent If aPath is /Sys then Tcb capability is required.
@capability Dependent If aPath begins with /Private and does not match this process' SID
					  then AllFiles capability is required.
@capability Dependent If aPath is /Resource then Tcb capability is required.
        
@see RFs::MkDirAll       
*/
EXPORT_C TInt RFs::MkDir(const TDesC& aPath)
	{
#ifdef _DEBUG_LOGGING

    RDebug::Print(_L("DEBUG: MkDir> path=%S\n"), &aPath);

#endif



	// Skip the file on the end
	TInt len = aPath.Length();
	while(len > 0 && !IsPathSep(aPath[len-1]))
		len--;
	TPtrC path(aPath.Left(len));

	// Check if it exists already
	TUint att;
	if (RFs::Att(path, att) == KErrNone)
		{
		if (att&KEntryAttDir)
			return KErrAlreadyExists;
		else
			// A file exists with the same name
			return KErrAccessDenied;
		}
	const char* name = PosixFilename(path);

#ifdef _WIN32
	if (mkdir(name) != 0)
#else
	if (mkdir(name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
#endif
		return mapErr(errno);
	return KErrNone;
	}

/**
Makes one or more directories.

Any valid path component specified in aPath which does not already exist is
created as a directory.
 
Note that if a filename is specified in the argument, it is	ignored.
Therefore, there should be a trailing backslash after the final
directory name in the argument to indicate that it is a directory, not a
filename.

See also notes on RFs::MkDir() about trailing backslashes in directory names.

Note also that because this method can return an error code (eg. because
the disk is full) before checking whether the path already exists, it
is not appropriate to use it just to work out whether a path exists or not.
		
See MkDir(), which creates only a single new directory.

@param aPath The path name specifiying the directory or directories to
             create. If the function completes successfully, this path
             identifies a valid	directory. Any path components which are not
             specified here are taken from the session path.

@return KErrNone if successful, otherwise one of the other
        system-wide error codes. Even if another error code is returned,
		(for example, if the disk is full) it is still possible that the 
		path may already exist. 


@capability Dependent If aPath is /Sys then Tcb capability is required.
@capability Dependent If aPath begins with /Private and does not match this process' SID
					  then AllFiles capability is required.
@capability Dependent If aPath is /Resource then Tcb capability is required.

@see RFs::MkDir
*/
EXPORT_C TInt RFs::MkDirAll(const TDesC& aPath)
	{
#ifdef _DEBUG_LOGGING
	RDebug::Print(_L("DEBUG: MkDirAll> path=%S\n"), &aPath);
#endif
	
	// Skip the file on the end
	TInt len = aPath.Length();
	while(len > 0 && !IsPathSep(aPath[len-1]))
		len--;
	TPtrC path(aPath.Left(len));

	// Check if it exists already
	TUint att;
	if (RFs::Att(path, att) == KErrNone)
		{
		if (att&KEntryAttDir)
			return KErrAlreadyExists;
		else
			// A file exists with the same name
			return KErrAccessDenied;
		}

	// Skip drive letter
	TInt start = 1;
	if (len >= 3 && path[1] == TChar(':') && IsPathSep(path[2]))
		{
		start = 3;
		}

	for (TInt pos = start; pos < len; pos++)
		{
		if (IsPathSep(path[pos]))
			{
			TPtrC parent(path.Ptr(), pos + 1);
			
			TUint att;
			TInt err = RFs::Att(parent, att);
			if (err == KErrNotFound)
				{
				TInt err = RFs::MkDir(parent);
#ifdef _DEBUG_LOGGING
				RDebug::Print(_L("DEBUG: MkDirAll> parent=%S err=%d\n"), &parent, err);
#endif
				if (err != KErrNone)
					return err;
				}
			else if (!(att & KEntryAttDir))
				{
				// Path element is not a directory
				return KErrAccessDenied;
				}
			}
		}
	return KErrNone;
	}

EXPORT_C TInt RFs::RmDir(const TDesC& aPath)
/**
Removes a directory.

The directory must be empty and cannot be the root directory. 

Note that if a filename is specified in the argument, it is
ignored. 

For example, following code will result in directory "C:\\SRC\\" being removed as long as 
it is empty, the existance of "ENTRY" will not be checked:

@code
fs.RmDir(_L("C:\\SRC\\ENTRY"));
@endcode

Similarly, following code will try to remove "C:\\SRC\\" instead of "C:\\SRC\DIR\\":
@code
fs.RmDir(_L("C:\\SRC\\DIR"));
@endcode

Therefore, there should be a trailing backslash after the final
directory name in the argument to indicate that it is a directory, not a
filename.

See class CFileMan for information on deleting a
non-empty directory and all of its contents.
				
@param aPath The path name of the directory to be removed. Any path	components
             which are not specified here are taken from the session path. Only
             the lowest-level directory identified is removed.

@return KErrNone, if successful;
        KErrInUse, if trying to remove a non-empty directory or root directory;
        otherwise, one of the other system-wide error codes.
              
@capability Dependent If aPath is /Sys then Tcb capability is required.
@capability Dependent If aPath begins with /Private and does not match this process' SID
					  then AllFiles capability is required.
@capability Dependent If aPath is /Resource then Tcb capability is required.

@see CFileMan
*/
	{

#ifdef _DEBUG_LOGGING

    RDebug::Print(_L("DEBUG: RmDir> path=%S\n"), &aPath);

#endif


	// Skip the file on the end
	TInt len = aPath.Length();
	while(len > 0 && !IsPathSep(aPath[len-1]))
		len--;
	TPtrC path(aPath.Left(len));

	// Return error on an attempt to delete root!
	if (len == 0 || (len <= 2 && IsPathSep(path[0])) || (len >= 3 && len < 5 && path[1] == TChar(':') && IsPathSep(path[2])))
		return KErrInUse;

	// Return an error if the directory doesn't exist or isn't a directory
	TUint att;
	if (RFs::Att(path, att) != KErrNone || !(att&KEntryAttDir))
		return KErrNotFound;

	const char* name = PosixFilename(path);
	if (rmdir(name) != 0)
		return mapErr(errno);
	return KErrNone;
	}

EXPORT_C TInt RFs::Parse(const TDesC& aName, TParse& aParse) const
/**
Parses a filename specification.

Parsing is done with wildcard resolution, using the session path as
the default. You can then use TParse's getter functions to extract individual
components of the resulting name. All the path components that are included
in aName are put into the resulting	filename. Any components that are still
missing are taken from the session path.

Specifying:

@code
TParse fp;
@endcode
@code
fs.Parse(name,fp);
@endcode

is equivalent to 

@code
TParse fp;
@endcode
@code
fp.Set(name,NULL,&fs.SessionPath());
@endcode

Note that the function does not check for illegal characters, or for
illegal path components in either of the paths specified.

@param aName  The file name to be parsed, using the session path to provide
              the missing components.
@param aParse A TParse objct that provides functions for
              extracting individual components of the resulting file name.

@return KErrNone if successful, otherwise one of the other
        system-wide error codes.
*/
	{
	TFileName session_path;
	TInt r = SessionPath(session_path);
	if (r==KErrNone)
		r = aParse.Set(aName, NULL, &session_path);
	return r;
	}

EXPORT_C TInt RFs::Parse(const TDesC& aName,const TDesC& aRelated,TParse& aParse) const
/**
Parses a filename specification, specifying related file path components.

Parsing is done with wildcard resolution, using the session path as
the default. You can then use TParse's getter functions to extract individual
components of the resulting name. All the path components that are included
in aName are put into the resulting	filename. Any missing components are taken
from the optional aRelated argument, which has the next order of precedence.
Finally, any components that are still missing are taken from the session path.

Specifying:

@code
TParse fp;
@endcode
@code
fs.Parse(name,related,fp);
@endcode

is equivalent to 

@code
TParse fp;
@endcode
@code
fp.Set(name,related,&fs.SessionPath());
@endcode

Note that the function does not check for illegal characters, or for
illegal path components in any of the paths specified.

@param aName    The file name to be parsed, using the session path and the
                related path to provide the missing components.
@param aRelated The related file specification.
@param aParse   A TParse objct that provides functions for
                extracting individual components of the resulting file name.

@return KErrNone if successful, otherwise one of the other
        system-wide error codes.
*/
	{
	TFileName session_path;
	TInt r = SessionPath(session_path);
	if (r==KErrNone)
		r = aParse.Set(aName, &aRelated, &session_path);
	return r;
	}

EXPORT_C TInt RFs::Delete(const TDesC& aName)
/**
Deletes a single file.

Wildcards are not allowed in either the	file name or the extension,
otherwise an error is returned.

Note that the file must be closed and must not be read-only.
Hidden files can be deleted but system files cannot.

See class CFileMan for information on deleting multiple files.
		  
@param aName The name of the file to be deleted. Any path components which
             are not specified here will be taken from the session path.

@return KErrNone if successful, otherwise one of the other
        system-wide error codes.

@capability Dependent If aName is /Sys then Tcb capability is required.
@capability Dependent If aName begins with /Private and does not match this process' SID
					  then AllFiles capability is required.
@capability Dependent If aName is /Resource then Tcb capability is required.
        
@see CFileMan        
*/
	{

#ifdef _DEBUG_LOGGING

    RDebug::Print(_L("DEBUG: Delete> name=%S\n"), &aName);

#endif



	TUint att;

	TInt err = Att(aName, att);

	if (err != KErrNone)

		return err;



	// Don't allow deletion of directories

	if (att & KEntryAttDir)

		return KErrAccessDenied;


	const char* name = PosixFilename(aName);
	if (remove(name) != 0)
		return mapErr(errno);
	return KErrNone;
	}

EXPORT_C TInt RFs::Rename(const TDesC& anOldName, const TDesC& aNewName)
/**
Renames a single file or directory.

It can also be used to move a file or directory by specifying different
destination and source directories.	If so, the destination and source
directories must be on the same drive. If a	directory is moved, then
the directory structure beneath it is also	moved.

If a directory specified by aNewName is different from one specified
by anOldName, then the file or directory is	moved to the new directory.
The file or directory cannot be moved to another device by this means,
either explicitly (by another drive	specified in the name) or implicitly
(because the directory has been mapped to another device with SetSubst().

The function fails and returns an error code in the following
circumstances:

1. If either the old or new name includes wildcards.

2. If a file or directory with the new name already exists in
   the target directory. Overwriting is not permitted.

3. If file anOldName does not exist, or is open.

Read-only, system and hidden files may be renamed. The renamed
file's attributes are preserved.

Note that when this function is operating on directories, a	trailing backslash
is not required after the final directory name in either anOldName or aNewName.

See class CFileMan for information on renaming multiple files.
		  				
@param anOldName File or directory to be renamed. Any path components which are
                 not specified here will be taken from the session path.
@param aNewName  Path specifying the new name for the file or directory and/or
				 its new parent directory. All directories specified in this path
				 must exist.
				 Any path components which are not specified here will be taken
				 from the session path.

@return KErrNone if successful, otherwise one of the other
        system-wide error codes.

@capability Dependent If either anOldName or aNewName is /Sys then Tcb capability is required.
@capability Dependent If either anOldName or aNewName begins with /Private and does not match
					  this process' SID then AllFiles capability is required.
@capability Dependent If either anOldName or aNewName is /Resource then Tcb capability is required.
        
@see CFileMan        
*/
	{
#ifdef _DEBUG_LOGGING

    RDebug::Print(_L("DEBUG: Rename> old=%S new=%S\n"), &anOldName, &aNewName);

#endif



	// Lose path separator on the end
	TInt len = anOldName.Length();
	while(len > 0 && IsPathSep(anOldName[len-1]))
		len--;

	TBuf8<KMaxFileName + 1> copyold;
	const char* oldname = PosixFilename(anOldName, copyold);


	// If new file exists already - return an error

	TUint newAtt;

	if (Att(aNewName, newAtt)== KErrNone)

		return KErrAlreadyExists;


	// Lose path separator on the end
	len = aNewName.Length();
	while(len > 0 && IsPathSep(aNewName[len-1]))
		len--;

	TBuf8<KMaxFileName + 1> copynew;
	const char* newname = PosixFilename(aNewName, copynew);

	if (rename(oldname, newname) != 0)
		return mapErr(errno);
	return KErrNone;
	}

EXPORT_C TInt RFs::Replace(const TDesC& anOldName, const TDesC& aNewName)
/**
Replaces a single file with another.

This function does not support the use of wildcards. Unlike Rename(), it only
applies to files.

This function operates as follows:

1. if the aNewName file does not exist, it is created.

2. anOldName's contents, attributes and the date and time of its last
   modification are copied to file aNewName, overwriting any existing contents
   and attribute details.

3. anOldName is deleted.
				 
anOldName may be hidden, read-only or a system file. However,
neither anOldName, nor, if it exists, aNewName, can be open;
aNewName must not be read-only.
Both files must be on the same drive.

@param anOldName The file to be replaced. Must exist and must be closed. It is
                 deleted by this function.
@param aNewName  The file to replace anOldName. Does not need to exist, but if
                 it does exist, it must be closed. If it exists, its name
                 remains unchanged but its contents, attributes and the date
                 and time of its last modification are replaced by those
                 of anOldName.
                 If it does not exist, it will be created and is assigned
                 the contents and attributes of anOldName. Must not be followed
                 by a trailing backslash.

@return KErrNone, if successful;
        KErrAccessDenied, if an attempt is made to replace a directory;
        otherwise one of the other system-wide error codes. 

@capability Dependent If either anOldName or aNewName is /Sys then Tcb capability is required.
@capability Dependent If either anOldName or aNewName begins with /Private and does not match
					  this process' SID then AllFiles capability is required.
@capability Dependent If either anOldName or aNewName is /Resource then Tcb capability is required.

*/
	{
#ifdef _DEBUG_LOGGING

    RDebug::Print(_L("DEBUG: Replace> old=%S new=%S\n"), &anOldName, &aNewName);

#endif



	TInt err = RFs::Delete(aNewName);
	if (err<KErrNone && err!=KErrNotFound)
		return err;
	return RFs::Rename(anOldName, aNewName);
	}

EXPORT_C TInt RFs::Att(const TDesC& aName, TUint& aAttValue) const
/**
Gets a file's attributes.

@param aName The filename. Any path components which are not specified here
             will be taken from the session path.
@param aVal  On return, the individual bits within the byte indicate which
             attributes have been set. For more information see KEntryAttNormal
	         and the other file/directory attributes.

@return KErrNone if successful, otherwise one of the other
        system-wide error codes.

@capability Dependent If aName contains /sys/ then AllFiles capability is required.
@capability Dependent If aName contains /Private/ and does not match
					  this process' SID then AllFiles capability is required.
        
@see KEntryAttNormal
*/
	{
#ifdef _DEBUG_LOGGING
	RDebug::Print(_L("DEBUG: Att> aName=%S\n"), &aName);
#endif
	
	TBuf<KMaxFileName + 1> copy;
	copy.Copy(aName.Left(KMaxFileName));

	// Get rid of trailing backslash or forward slash
	if (copy.Length() > 0 && IsPathSep(copy[copy.Length() - 1]))
		copy.SetLength(copy.Length() - 1);
	
	// It seems you can't retrieve the attributes of root
	if (copy.Length() == 0 || (copy.Length() == 2 && copy[1] == ':'))
		return KErrBadName;

	const char* name = PosixFilename(copy);
	
	struct stat sb;
	if(stat(name, &sb) != 0)
		return mapErr(errno);
	
	aAttValue = 0;
    if (S_ISREG(sb.st_mode))
    	aAttValue = KEntryAttNormal;
    else if(S_ISDIR(sb.st_mode))
    	aAttValue |= KEntryAttDir;

	// Bit Value Specifier File          Directory
	// 0   1     x         executable    can search directory
	// 1   2     w         write access  can modify (delete/add files)
	// 2   4     r         read access   read access

	// Test readonly flag - no write access
    if (!(sb.st_mode & 0222))
    	aAttValue |= KEntryAttReadOnly;
    
    return KErrNone;
	}

EXPORT_C TInt RFs::SetAtt(const TDesC& aName, TUint aSetAttMask, TUint aClearAttMask)
/**
Sets or clears the attributes of a single file or directory.

The function uses two bitmasks. The first bitmask specifies the	attributes
to be set; the second specifies the attributes to be cleared.

An attempt to set or clear the KEntryAttDir, KEntryAttVolume or KEntryAttRemote
attributes have no effect.

@param aName          File or directory name. Any path components which are not
                      specified here will be taken from the session path. Must
                      not include wildcard characters. The file must be closed.
@param aSetAttMask    Bitmask indicating the attributes to be set.
@param aClearAttMask  Bitmask indicating the attributes to be cleared. For more
				      information, see KEntryAttNormal and the other file or
				      directory attributes.

@return KErrNone if successful, otherwise one of the other
        system-wide error codes.

@panic FSCLIENT 21 if any attribute appears in both bitmasks.


@capability Dependent If aName is /Sys then Tcb capability is required.
@capability Dependent If aName begins with /Private and does not match
					  this process' SID then AllFiles capability is required.
@capability Dependent If aName is /Resource then Tcb capability is required.
	
@see RFs::SetEntry

*/
	{

#ifdef _DEBUG_LOGGING

	RDebug::Print(_L("DEBUG: SetAtt> aName=%S set=%x clear=%x\n"), &aName, aSetAttMask, aClearAttMask);

#endif


	const char* name = PosixFilename(aName);
	struct stat sb;
	if(stat(name, &sb) != 0)
		return mapErr(errno);
	
	// Clear readonly flag - write access
    if (aClearAttMask&KEntryAttReadOnly)
	    sb.st_mode |= 0222;

    // Set readonly flag - no write access
    if (aSetAttMask&KEntryAttReadOnly)
    	sb.st_mode &= ~0222;
    
	return mapErr(chmod(name, sb.st_mode));
	}

EXPORT_C TInt RFs::Modified(const TDesC& aName,TTime& aTime) const
/**
Gets the last modification date and time of a file or a directory,
in UTC.

If there has been no modification, the function gets the date and
time of the file or directory's creation.

@param aName File or directory name.
@param aTime On return, contains the date and time of the file or
             directory's last modification in universal time.

@return KErrNone if successful, otherwise one of the other
        system-wide error codes.

@capability Dependent If aName contains /sys/ then AllFiles capability is required.
@capability Dependent If aName contains /Private/ and does not match
					  this process' SID then AllFiles capability is required.

*/
	{

	TEntry e;
	TInt r=Entry(aName,e);
	if (r==KErrNone)
		aTime=e.iModified;
	return(r);
	}

TInt ReadUid(const char* aName,TEntry& anEntry)
//
// Read the entry uid if present
//
	{
    FILE* hFile = fopen(aName, "rb");
    if (!hFile)
    	return KErrGeneral;
    
	//  First check to see if the first sixteen bytes form a valid UID
	TBuf8<sizeof(TCheckedUid)> checkedUidBuf;
	checkedUidBuf.SetLength(sizeof(TCheckedUid));
	TInt ret = fread(&checkedUidBuf[0], sizeof(TCheckedUid), 1, hFile);
	fclose(hFile);
	if (ret != 1)
		return KErrCorrupt;
	
	TCheckedUid checkedUid(checkedUidBuf);
	if(checkedUid.UidType()!=TUidType(TUid::Null(),TUid::Null(),TUid::Null()))
		anEntry.iType=checkedUid.UidType();

	return KErrNone;
	}

EXPORT_C TInt RFs::Entry(const TDesC& aName, TEntry& anEntry) const
/**
Gets the entry details for a file or directory.

This information includes UID information.

@param aName   Name of file or directory.
@param anEntry On return, contains the entry details for the file or directory. TEntry::iModified contains UTC date and time.

@return KErrNone if successful, otherwise one of the other
        system-wide error codes.

@capability Dependent If aName contains "\\Sys\\" and includes an additional file or directory then AllFiles capability 
					  is required. For example, the paths "c:\\sys" and "c:\\sys\\" will always be readable, whereas
					  the path "c:\\sys\\abc\\" will only be readable with AllFiles capability.

@capability Dependent If aName contains \\Private\\ and includes an additional file, or a directory which does not match
					  this process' SID, then AllFiles capability is required. For example, the paths "c:\\private" and 
					  "c:\\private\\" will always be readable, whereas the path "c:\\private\\<n>\\" will only be 
					  readable with AllFiles capability or if <n> matches the process' SID.
*/
	{

#ifdef _DEBUG_LOGGING

	RDebug::Print(_L("DEBUG: Entry> aName=%S\n"), &aName);

#endif


	TInt err = RFs::Att(aName, anEntry.iAtt);
	if (err < KErrNone)
		return err;

	struct stat sb;
	TBuf<KMaxFileName + 1> copy;
	copy.Copy(aName.Left(KMaxFileName));

	// Get rid of trailing backslash 
	if (copy.Length() > 0 && IsPathSep(copy[copy.Length() - 1]))
		copy.SetLength(copy.Length() - 1);

	const char* name = PosixFilename(copy);
	if(stat(name, &sb) < 0)
		return KErrNotFound;
	

	// On Symbian directories have zero size
	anEntry.iSize = (anEntry.iAtt&KEntryAttDir) ? 0 : sb.st_size;

    anEntry.iType = TUidType(TUid::Null(), TUid::Null(), TUid::Null());
    ReadUid(name, anEntry);
    
	struct tm *tm = gmtime(&sb.st_mtime);
	anEntry.iModified = TDateTime(1900 + tm->tm_year, TMonth(tm->tm_mon), tm->tm_mday - 1, tm->tm_hour, tm->tm_min, tm->tm_sec, 0);

	// Just return the name
	TInt start = aName.LocateReverse('\\');
	if (start > 0)
		{
		if (anEntry.iAtt&KEntryAttDir)
			{
			anEntry.iName = aName.Left(start);
			start = anEntry.iName.LocateReverse('\\');
			if (start > 0)
				anEntry.iName = anEntry.iName.Mid(start + 1);
			}
		else
			anEntry.iName = aName.Mid(start + 1);
		}
	else
		anEntry.iName = aName;
	
    return KErrNone;
	}

EXPORT_C TInt RFs::SetEntry(const TDesC& aName,const TTime& aTime,TUint aSetAttMask,TUint aClearAttMask)
/**
Sets both the attributes and the last modified date and time for a file or directory.

The function uses two bitmasks. The first bitmask determines
which attributes should be set. The second bitmask determines which should be cleared.

An attempt to set or clear the KEntryAttDir, KEntryAttVolume or KEntryAttRemote
attributes have no effect.
			 
@param aName          File or directory name.
@param aTime	      New date and time. UTC date and time should be used.
@param aSetAttMask    Bitmask indicating which attributes are to be set.
@param aClearAttMask  Bitmask indicating which attributes are cleared. For more
                      information, see KEntryAttNormal, and the other file
                      or directory attributes.

@return KErrNone, if successful;
        KErrInUse, if the file is open;
        otherwise one of the other system-wide error codes.

@panic FSCLIENT 21 if any attribute appears in both bitmasks.        

@capability Dependent If aName is /Sys then Tcb capability is required.
@capability Dependent If aName begins with /Private and does not match
					  this process' SID then AllFiles capability is required.
@capability Dependent If aName is /Resource then Tcb capability is required.

@see KEntryAttNormal
@see KEntryAttDir
@see KEntryAttVolume
*/
	{
	// No way to set modified date
	return SetAtt(aName, aSetAttMask, aClearAttMask);
	}

TInt gBlockSize = 4096;
TInt gClusterSize = 4096;
EXPORT_C TInt RFs::VolumeIOParam(TInt aDriveNo, TVolumeIOParamInfo& aParamInfo) const
/**
This function queries a set of I/O parameters on the specified volume, this includes the block size of underlying media,
cluster size of the mounted file system and the recommended read/write buffer sizes. 

The volume information is retuned through aParamInfo. Even if VolumeIOParam() returns successful, errors 
can effect the return value of each field within aParamInfo.

@param aDrive A drive number, specifies which volume to query.
@param aParamInfo A TVolumeIOParamInfo containing the volume parameters.

@return KErrNone if successful; otherwise, another system wide error code is returned.
*/
	{
	aParamInfo.iBlockSize = gBlockSize;
	aParamInfo.iClusterSize = gClusterSize;
	aParamInfo.iRecReadBufSize = 4096;
	aParamInfo.iRecWriteBufSize = 4096;
	return KErrNone;
	}

EXPORT_C TInt RFs::SetErrorCondition(TInt anError, TInt aCount)
	{
	return KErrNotSupported;
	}

void Panic(TClientPanic aPanic)
//
// Panic the current client with a file server client side panic.
//
	{

	User::Panic(_L("FSCLIENT panic"),aPanic);
	}
