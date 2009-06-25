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
// f32\sfsrv\cl_file.cpp
// 
//

#include <f32file.h>
#include <e32math.h>

#include <cstdio>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

class TFileInfo
	{
public:
	FILE* iHandle;
	TFileName iName;
	TUint iMode;
	};

#define GETFILEHANDLE(A,B) TFileInfo* A; memcpy(&A, B, 4);
#define FILEHANDLE(A) GETFILEHANDLE(A,this);
	
extern TInt mapErr(TInt);
extern const char* PosixFilename(const TDesC &aIn);
extern const char* PosixFilename(const TDesC &aIn, TDes8& aOut);

/**
Make a duplicate of the passed file handle in the same thread.

By default, any thread in the process can use the duplicated handle to access the 
file. However, specifying EOwnerThread as the second parameter to this function, 
means that only the creating thread can use the handle.

@param	aFile	The file handle to duplicate
@param	aType	An enumeration whose enumerators define the ownership of this 
				handle. If not explicitly specified, EOwnerProcess is taken
				as default.

@return	one of the other system-wide error codes.
*/
EXPORT_C TInt RFile::Duplicate(const RFile& aFile, TOwnerType aType)
	{
	GETFILEHANDLE(h,&aFile);
	TFileName name(h->iName);
	RFs fs;  // This doesn't actually get used in the Open function
	TUint mode = h->iMode;

	// Get the file position
	TInt pos = ftell(h->iHandle);
	if (pos < 0)
		return mapErr(errno);

	// Open the file
	TInt err = Open(fs, name, mode);
	if (err != KErrNone)
		return err;

	// Set the file position
	return Seek(ESeekStart, pos); 
	}

EXPORT_C TInt RFile::Name(TDes &aName) const
/**
Gets the final part of a filename

This is used to retrieve the name and extension of a file that has been 
passed from one process to another using the RFile::AdoptXXX() methods.

@param	aName	On return, contains the name of the file, including the name and 
				extension but excluding the drive letter and path.

@return KErrNone if successful, otherwise one of the other
        system-wide error codes.

*/
	{
    FILEHANDLE(h);
    TParsePtrC ptr(h->iName);
    aName = ptr.NameAndExt();
    return KErrNone;
	}


EXPORT_C TInt RFile::FullName(TDes& aName) const
/**
Gets the full filename

This is used to retrieve the full filename, including drive and path,
of a file that has been passed from one process to another using the 
RFile::AdoptXXX() methods.

@param	aName	On return, contains the full name of the file, including drive and path.

@return KErrNone if successful, otherwise one of the other
        system-wide error codes.

*/
	{
    FILEHANDLE(h);
    aName = h->iName;
    return KErrNone;
	}



/*
"r" 	Open a text file for reading
"r+" 	Open a text file for read/write

"rb" 	Open a binary file for reading
"rb+" 	Open a binary file for read/write

"w" 	Create a text file for writing
"w+" 	Create a text file for read/write

"wb" 	Create a binary file for writing
"wb+" 	Create a binary file for read/write

"a" 	Append to a text file
"ab" 	Append to a binary file
*/
void GetFileMode(TDes8& aBuf, TUint aFileMode, TBool aOpen)
	{
	if (aOpen)
		{
		if (aFileMode & EFileWrite)
			// Open a binary file for read/write
			aBuf = _L8("rb+");
		else
			// Open a binary file for reading
			aBuf = _L8("rb");
		}
	else
		// Create a binary file for read/write 
		aBuf = _L8("wb+");
	}

TInt DoOpen(RFile* aFile, const TDesC &aName, TUint aFileMode, TBool aOpen)
    {
	const char* name = PosixFilename(aName);
   
	TFileInfo* info = new TFileInfo;
	if (!info)
		return KErrNoMemory;
	
	TBuf8<4> mode;
	GetFileMode(mode, aFileMode, aOpen);
	info->iHandle = fopen(name, reinterpret_cast<const char*>(mode.PtrZ()));
    
#ifdef _DEBUG_LOGGING
    RDebug::Print(_L("DEBUG: DoOpen> file=%S id=%x\n"), &aName, info->iHandle);
#endif
    
    if (!info->iHandle)
    	{
    	delete info;
    	return mapErr(errno);
    	}

	// Symbian expects file pos at the begining
	if (aOpen && aFileMode|EFileWrite)
		{
		if (fseek(info->iHandle, 0, SEEK_SET) != 0)
			{
			fclose(info->iHandle);
			delete info;
	    	return mapErr(errno);
			}
		}

    info->iName = aName;
    info->iMode = aFileMode;
    memcpy(aFile, &info, 4);
    return KErrNone;
    }

EXPORT_C TInt RFile::Open(RFs& aFs, const TDesC& aName, TUint aFileMode)
/**
Opens an existing file for reading or writing.

If the file does not already exist, an error is returned.

Notes:

1. To close the file, use Close()

2. Attempting to open a file with the read-only attribute using the EFileWrite
   access mode results in an error.

3. Attempting to open a file which is greater than or equal to 2GByte (2,147,483,648 bytes)
   will fail with KErrTooBig

4. After a file has been opened, the current write position is set to the start
   of the file.
   If necessary, use RFile::Seek() to move to a different position within
   the file.

@param aFs   The file server session.
@param aName The name of the file. Any path components (i.e. drive letter
             or directory), which are not specified, are taken from
             the session path.
@param aMode The mode in which the file is opened. See TFileMode.

@return KErrNone if successful, otherwise one of the other system-wide
        error codes.
        
@see TFileMode

@capability Dependent If the path for aName is /Sys and aMode is neither
					  EFileShareReadersOnly nor EFileRead then Tcb capability is required.
@capability Dependent If the path for aName is /Sys and aMode is either
					  EFileShareReadersOnly or EFileRead then Allfiles capability is required.
@capability Dependent If the path for aName begins with /Private and does not match this process'
					  SID then AllFiles capability is required.
@capability Dependent If the path for aName begins with /Resource and aMode is neither
 					  EFileShareReadersOrWriters|EFileRead nor EFileShareReadersOnly 
 					  nor EFileRead then Tcb capability is required.

*/
    {
#ifdef _DEBUG_LOGGING
	RDebug::Print(_L("DEBUG: RFile::Open> name=%S mode=%x\n"), &aName, aFileMode);
#endif
	
    return DoOpen(this, aName, aFileMode, ETrue);
    }

EXPORT_C void RFile::Close()
/**
Closes the file.

Any open files are closed when the file server session is closed.

Close() is guaranteed to return, and provides no indication whether
it completed successfully or not. When closing a file you have written to,
you should ensure that data is committed to the file by invoking RFile::Flush()
before closing. If Flush() completes successfully, Close() is essentially a
no-operation.
*/
    {
    FILEHANDLE(h);
    if (h)
    	{
    	fflush(h->iHandle);
    	fclose(h->iHandle);
    	delete h;

		// Make sure the handle is null
        memset(this, 0, 4);
    	}
    }

EXPORT_C TInt RFile::Create(RFs& aFs, const TDesC& aName, TUint aFileMode)
/**
Creates and opens a new file for writing.

If the file already exists, an error is returned.

If the resulting path does not exist, then the operation cannot proceed and
the function returns an error code.

Notes:

1. To close the file, use Close()

2. It automatically sets the file's archive attribute.

@param aFs   The file server session.
@param aName The name of the file. Any path components (i.e. drive letter
             or directory), which are not specified, are taken from
             the session path.
@param aMode The mode in which the file is opened. The access mode is
             automatically set to EFileWrite. See TFileMode.

@return KErrNone if successful, otherwise one of the other system-wide
        error codes.
        
@see TFileMode

@capability Dependent If the path in aName starts with /Sys then capability Tcb is required
@capability Dependent If the path in aName starts with /Resource then capability Tcb is required
@capability Dependent If the path in aName starts with /Private and does not match this process'
					  SID then AllFiles capability is required.

*/
    {
#ifdef _DEBUG_LOGGING
    RDebug::Print(_L("DEBUG: RFile::Create> name=%S mode=%x\n"), &aName, aFileMode);
#endif
    
    TUint att;
    if (aFs.Att(aName, att) == KErrNone)
    	return KErrAlreadyExists;
    return DoOpen(this, aName, aFileMode|EFileWrite, EFalse);
    }

EXPORT_C TInt RFile::Replace(RFs& aFs, const TDesC& aName, TUint aFileMode)
/**
Opens a file for writing, replacing the content of any existing file of the
same name if it exists, or creating a new file if it does not exist.

If the resulting path exists, then:

- the length of an existing file with the same filename is re-set to zero 

- a new file is created, if no existing file with the same filename can be found.

If the resulting path does not exist, then the operation cannot proceed and
the function returns an error code.

Notes:

- To close the file, use Close(), defined in the base class RFsBase.

- It automatically sets the file's archive attribute.

@param aFs   The file server session.
@param aName The name of the file. Any path components (i.e. drive letter
             or directory), which are not specified, are taken from
             the session path.
@param aMode The mode in which the file is opened. The access mode is
             automatically set to EFileWrite. See TFileMode.

@return KErrNone if successful, otherwise one of the other system-wide
        error codes.
        
@see TFileMode

@capability Dependent If the path in aName starts with /Sys then capability Tcb is required
@capability Dependent If the path in aName starts with /Resource then capability Tcb is required
@capability Dependent If the path in aName starts with /Private and does not match this process'
					  SID then AllFiles capability is required.

*/
    {
#ifdef _DEBUG_LOGGING
    RDebug::Print(_L("DEBUG: RFile::Replace> name=%S mode=%x\n"), &aName, aFileMode);
#endif
    
    return DoOpen(this, aName, aFileMode|EFileWrite, EFalse);
    }

EXPORT_C TInt RFile::Temp(RFs& aFs, const TDesC& aPath, TFileName& aName, TUint aFileMode)
/**
Creates and opens a temporary file with a unique name for writing and reading.

Notes:

1. To close the file, use Close()

@param aFs   The file server session.
@param aPath The directory in which the file is created.
@param aName On return, contains the full path and file name of the file.
             The filename is guaranteed to be unique within the directory
             specified by aPath.
@param aMode The mode in which the file is opened. The access mode is
             automatically set to EFileWrite. See TFileMode.

@return KErrNone if successful, otherwise one of the other system-wide
        error codes.
        
@see TFileMode

@capability Dependent If aPath starts with /Sys then capability Tcb is required
@capability Dependent If aPath starts with /Resource then capability Tcb is required
@capability Dependent If aPath starts with /Private and does not match this process'
					  SID then AllFiles capability is required.
*/
	{
	// Check for a valid path
	if (aPath.Length() > 0 && aPath[aPath.Length() - 1] != '\\')
		return KErrBadName;

	// Create a temporary file name
	TFileName name;
	while(1)
		{
		name.Format(_L("%Ssymport%d.tmp"), &aPath, Math::Random());	

		TUint att;
		TInt err = aFs.Att(name, att);
		if (err == KErrNotFound)
			break;
		else if (err != KErrNone)
			return err;
		}
		
	TInt err = Create(aFs, name, aFileMode);
	if (err == KErrNone)
		{
		aName = name;
		return KErrNone;
		}
	// The path couldn't be found
	else if (err == KErrNotFound)
		return KErrBadName;

	return err;
	}

EXPORT_C TInt RFile::Read(TDes8& aDes) const
/**
Reads from the file at the current position.

This is a synchronous function.

Note that when an attempt is made to read beyond the end of the file,
no error is returned. 
The descriptor's length is set to the number of bytes read into 
it. Therefore, when reading through a file,the end of file has been reached 
when the descriptor length, as returned by TDesC8::Length(), is zero.

@param aDes Descriptor into which binary data is read. Any existing contents 
            are overwritten. On return, its length is set to the number of
            bytes read.
@return KErrNone if successful, otherwise one of the other system-wide error 
        codes.

@see TDesC8::Length
*/
	{
	return Read(aDes, aDes.MaxLength());
	}

EXPORT_C TInt RFile::Read(TDes8& aDes, TInt aLength) const
/**
Reads the specified number of bytes of binary data from the file at the current position.

This is a synchronous function.

Note that when an attempt is made to read beyond the end of the file,
no error is returned. 
The descriptor's length is set to the number of bytes read into 
it. Therefore, when reading through a file,the end of file has been reached 
when the descriptor length, as returned by TDesC8::Length(), is zero.
Assuming aLength is less than the maximum length of the descriptor, the only circumstances 
in which Read() can return fewer bytes than requested, is when the end of 
file is reached or if an error occurs.

@param aDes    Descriptor into which binary data is read. Any existing
               contents are overwritten. On return, its length is set to
               the number of bytes read.
            
@param aLength The number of bytes to be read from the file into the descriptor. 
               If an attempt is made to read more bytes than the descriptor's 
               maximum length, the function returns KErrOverflow.
               This value must not be negative, otherwise the function
               returns KErrArgument.
               
@return KErrNone if successful, otherwise one of the other system-wide error
        codes.
*/
	{

#ifdef _DEBUG_LOGGING

    RDebug::Print(_L("DEBUG: Read1> length=%d\n"), aLength);

#endif


    FILEHANDLE(h);
    if (aLength > aDes.MaxLength())
    	return KErrOverflow;

	// If the file has been written then you have set the position for a read to succeed
	fseek(h->iHandle, 0, SEEK_CUR);

    size_t len = fread(const_cast<TUint8*>(aDes.Ptr()), 1, aLength, h->iHandle);
	TInt err = ferror(h->iHandle);
	if (err != 0)
		return mapErr(err);
	aDes.SetLength(len);
		
    return KErrNone;
	}

EXPORT_C TInt RFile::Read(TInt aPos, TDes8& aDes) const
/**
Reads from the file at the specified offset within the file

This is a synchronous function.

Note that when an attempt is made to read beyond the end of the file,
no error is returned. 
The descriptor's length is set to the number of bytes read into it.
Therefore, when reading through a file, the end of file has been reached 
when the descriptor length, as returned by TDesC8::Length(), is zero.

@param aPos Position of first byte to be read.  This is an offset from
            the start of the file. If no position is specified, reading
            begins at the current file position. 
            If aPos is beyond the end of the file, the function returns
            a zero length descriptor.
            
@param aDes The descriptor into which binary data is read. Any existing content
            is overwritten. On return, its length is set to the number of
            bytes read.
            
@return KErrNone if successful, otherwise one of the other system-wide error 
        codes.

@panic FSCLIENT 19 if aPos is negative.        
*/
	{
    return Read(aPos, aDes, aDes.MaxLength());
	}

EXPORT_C TInt RFile::Read(TInt aPos, TDes8& aDes, TInt aLength) const
/**
Reads the specified number of bytes of binary data from the file at a specified 
offset within the file.

This is a synchronous function.

Note that when an attempt is made to read beyond the end of the file,
no error is returned. 
The descriptor's length is set to the number of bytes read into it.
Therefore, when reading through a file, the end of file has been reached 
when the descriptor length, as returned by TDesC8::Length(), is zero.
Assuming aLength is less than the maximum length of the descriptor, the only
circumstances in which Read() can return fewer bytes than requested is when
the end of file is reached or if an error has occurred.

@param aPos    Position of first byte to be read. This is an offset from
               the start of the file. If no position is specified, 
               reading begins at the current file position.
               If aPos is beyond the end of the file, the function returns
               a zero length descriptor.
               
@param aDes    The descriptor into which binary data is read. Any existing
               contents are overwritten. On return, its length is set to
               the number of bytes read.
@param aLength The number of bytes to read from the file into the descriptor. 
               If an attempt is made to read more bytes than the descriptor's
               maximum length, then the function updates aStatus parameter with KErrOverflow.
               It must not be negative otherwise the function updates aStatus with KErrArgument.
               
@return KErrNone if successful, otherwise one of the other system-wide
        error codes.

@panic FSCLIENT 19 if aPos is negative.        
*/
	{
#ifdef _DEBUG_LOGGING

    RDebug::Print(_L("DEBUG: Read2> length=%d\n"), aLength);

#endif



	FILEHANDLE(h);
    if (aLength > aDes.MaxLength())
    	return KErrOverflow;

	// Check pos doesn't go past the end of the file
	TInt size;
	TInt err = Size(size);
	if (err != KErrNone)
		return err;
	aPos = Min(size, aPos);

    if (fseek(h->iHandle, aPos, SEEK_SET) == 0)
    	{
    	size_t len = fread(const_cast<TUint8*>(aDes.Ptr()), 1, aLength, h->iHandle);
    	if (len == 0 && ferror(h->iHandle) != 0)
    		return mapErr(ferror(h->iHandle));
        aDes.SetLength(len);
        return KErrNone;
    	}
    return mapErr(errno);
	}

EXPORT_C TInt RFile::Write(const TDesC8& aDes)
/**
Writes to the file at the current offset within the file.

This is a synchronous function.

NB Attempting to extend the file to 2 GByte or greater will fail with KErrTooBig

@param aDes The descriptor from which binary data is written.
            The function writes the entire contents of aDes to the file.

@return KErrNone if successful, otherwise one of the other system-wide error 
        codes.
*/
	{
    return Write(aDes, aDes.Length());
	}

EXPORT_C TInt RFile::Write(const TDesC8& aDes, TInt aLength)
/**
Writes a portion of a descriptor to the file at the current offset within
the file.

This is a synchronous function.

NB Attempting to extend the file to 2 GByte or greater will fail with KErrTooBig

@param aDes    The descriptor from which binary data is written.
@param aLength The number of bytes to be written from the descriptor.
               This must not be greater than the length of the descriptor.
               It must not be negative.

@return KErrNone if successful; KErrArgument if aLength is negative;
		otherwise one of the other system-wide error codes.
        
@panic FSCLIENT 27 in debug mode, if aLength is greater than the length
       of the descriptor aDes.  
*/
    {
#ifdef _DEBUG_LOGGING

    RDebug::Print(_L("DEBUG: Write1> length=%d\n"), aLength);

#endif



    FILEHANDLE(h);
    if (aLength < 0)
    	return KErrArgument;


	// Trying to write to a file that hasn't been opened for write?

	if (!(h->iMode&EFileWrite))

		return KErrAccessDenied;


	// If the file has been read then you have set the position for a write to succeed
	fseek(h->iHandle, 0, SEEK_CUR);

    size_t sz = fwrite(aDes.Ptr(), 1, aLength, h->iHandle);
	if (sz != (TUint)aLength)
		return mapErr(ferror(h->iHandle));
    return KErrNone;
    }

EXPORT_C TInt RFile::Write(TInt aPos, const TDesC8& aDes)
/**
Writes to the file at the specified offset within the file

This is a synchronous function.

NB Attempting to extend the file to 2 GByte or greater will fail with KErrTooBig

@param aPos The offset from the start of the file at which the first
            byte is written. 
            If a position beyond the end of the file is specified, then
            the write operation begins at the end of the file.
            If the position has been locked, then the write fails.
            
@param aDes The descriptor from which binary data is written. The function writes 
            the entire contents of aDes to the file.
            
@return KErrNone if successful, otherwise one of the other system-wide error
        codes.

@panic FSCLIENT 19 if aPos is negative.                       
*/
	{
    return Write(aPos, aDes, aDes.Length());
	}

EXPORT_C TInt RFile::Write(TInt aPos, const TDesC8& aDes, TInt aLength)
/**
Writes the specified number of bytes to the file at the specified offset within the file.

This is a synchronous function.

NB Attempting to extend the file to 2 GByte or greater will fail with KErrTooBig

@param aPos    The offset from the start of the file at which the first
               byte is written. 
               If a position beyond the end of the file is specified, then
               the write operation begins at the end of the file.
               If the position has been locked, then the write fails.
                             
@param aDes    The descriptor from which binary data is written.
@param aLength The number of bytes to be written from aDes .
			   It must not be negative.

@return KErrNone if successful; KErrArgument if aLength is negative;
		otherwise one of the other system-wide error codes.
        
@panic FSCLIENT 19 if aPos is negative.                       
*/
    {
#ifdef _DEBUG_LOGGING

    RDebug::Print(_L("DEBUG: Write2> length=%d\n"), aLength);

#endif



    FILEHANDLE(h);
    if (aLength < 0)
    	return KErrArgument;


	// Trying to write to a file that hasn't been opened for write?

	if (!(h->iMode&EFileWrite))

		return KErrAccessDenied;


	// Check pos doesn't go past the end of the file
	TInt size;
	TInt err = Size(size);
	if (err != KErrNone)
		return err;
	aPos = Min(size, aPos);

    if (fseek(h->iHandle, aPos, SEEK_SET) == 0)
    	{
    	size_t sz = fwrite(aDes.Ptr(), 1, aLength, h->iHandle);
    	if (sz != (TUint)aDes.Length())
    		return mapErr(ferror(h->iHandle));
    	return KErrNone;
    	}
    return mapErr(errno);
	}

EXPORT_C TInt RFile::Seek(TSeek aMode, TInt& aPos) const
/**
Sets the the current file position.

The function can also be used to get the current file 
position without changing it. The file position is the position at which
reading and writing takes place. The start of the file is position zero.

To retrieve the current file position without changing it, specify ESeekCurrent 
for the seek mode, and zero for the offset.

If the seek mode is ESeekStart, then:

1. the function does not modify the aPos argument,

2. the function returns an error if the offset specified is negative.

If the seek mode is ESeekAddress, an error is returned if:

1. the file is not in ROM, 

2. the offset specified is greater than the size of the file.

@param aMode Seek mode. Controls the destination of the seek operation.
@param aPos  Offset from location specified in aMode. Can be negative.
             On return contains the new file position.
             If the seek mode is either ESeekCurrent or ESeekEnd and the offset
             specifies a position before the start of the file 
             or beyond the end of the file, then on return, aPos is set to
             the new file position (either the start or the end of the file).
             If the seek mode is ESeekAddress, aPos returns the address of
             the byte at the specified offset within the file.

@return KErrNone if successful, otherwise one of the other system-wide error 
        codes.
*/
	{
	FILEHANDLE(h);
	
	// Get file size
	TInt size;
	TInt ret = Size(size);
	if (ret != KErrNone)
		return ret;

	switch(aMode)
		{
		case ESeekStart:
			{			
			// Make sure you don't seek past the end
			ret = fseek(h->iHandle, Min(aPos, size), SEEK_SET);
			break;
			}
		case ESeekCurrent:
			{
			// Get file position
			if ((ret = ftell(h->iHandle)) >= 0)
				{
				TInt current = ret;
				ret = 0;

				// Try and move the file position
				// fseek will move past the end of the file but RFile doesn't allow this
				// fseek thinks it's invalid to move before the start, RFile allows this and sets file pos to zero
				TInt request = Max(0, Min(current + aPos, size));
				if ((ret = fseek(h->iHandle, request, SEEK_SET)) == 0)
					aPos = request;
				}
			break;
			}
		case ESeekEnd:
			{
			// Try and move the file position
			// fseek will move past the end of the file but RFile doesn't allow this
			TInt request = Max(0, Min(size + aPos, size));
			if ((ret = fseek(h->iHandle, request, SEEK_SET)) == 0)
				aPos = request;
			break;
			}
		default: // Applies to ESeekAddress
			return KErrNotSupported;
		}
	if (ret)
		return mapErr(errno);
	return KErrNone;
	}

EXPORT_C TInt RFile::Flush()
/**
Commits data to the storage device and flushes internal buffers without closing 
the file.

Although RFile::Close() also flushes internal buffers, it is often useful 
to call Flush() before a file is closed. This is because Close() returns no 
error information, so there is no way of telling whether the final data was 
written to the file successfully or not. Once data has been flushed, Close() 
is effectively a no-operation.

@return KErrNone if successful, otherwise one of the other system-wide error 
        codes.
*/
    {
    FILEHANDLE(h);

	// Flush messes with the file pointer!
    TInt oldpos = ftell(h->iHandle);
	if (oldpos < 0)
		return mapErr(errno);
    if (fflush(h->iHandle) != 0)
    	return mapErr(errno);
    if (fseek(h->iHandle, oldpos, SEEK_SET) != 0)
		return mapErr(errno);

	return KErrNone;
    }

EXPORT_C TInt RFile::Size(TInt& aSize) const
/**
Gets the current file size.

@param aSize On return, the size of the file in bytes.

@return KErrNone if successful, otherwise one of the other system-wide error 
        codes.
*/
	{
    FILEHANDLE(h);
    
	// Have to flush data before changing size - flush messes with the file pointer!
    TInt oldpos = ftell(h->iHandle);
	if (oldpos < 0)
		return mapErr(errno);
    if (fflush(h->iHandle) != 0)
    	return mapErr(errno);
    if (fseek(h->iHandle, oldpos, SEEK_SET) != 0)
    	return mapErr(errno);
	
    struct stat sb;
    if (fstat(fileno(h->iHandle), &sb) == 0)
    	{
    	aSize = sb.st_size;
    	return KErrNone;
    	}

    return mapErr(errno);
	}

EXPORT_C TInt RFile::SetSize(TInt aSize)
/**
Sets the file size.

If the size of the file is reduced, data may be lost from 
the end of the file.

Note:

1. The current file position remains unchanged unless SetSize() reduces the size 
   of the file in such a way that the current file position is now beyond
   the end of the file. In this case, the current file position is set to
   the end of file. 

2. If the file was not opened for writing, an error is returned.

@param aSize The new size of the file, in bytes. This value must not be negative, otherwise the function raises a panic.

@return KErrNone if successful, otherwise one of the other system-wide error 
        codes.

@panic FSCLIENT 20 If aSize is negative.

*/
	{
	FILEHANDLE(h);


	// Trying to set size on a file that hasn't been opened for write?

	if (!(h->iMode&EFileWrite))

		return KErrAccessDenied;


    // fflush messes with the file position!
    TInt oldpos = ftell(h->iHandle);
	if (oldpos < 0)
		return mapErr(errno);
    if (fflush(h->iHandle) != 0)
    	return mapErr(errno);
    if (fseek(h->iHandle, oldpos, SEEK_SET) != 0)
    	return mapErr(errno);
    
	// Now change the file size
	if (ftruncate(fileno(h->iHandle), aSize) != 0)
		return mapErr(errno);
	return KErrNone;
	}

EXPORT_C TInt RFile::Att(TUint& aVal) const
/**
Gets the file's attributes.

@param aVal A bitmask which, on return, contains the file’s attributes.
            For more information, see KEntryAttNormal and the other
            file/directory attributes.    

@return KErrNone if successful, otherwise one of the other system-wide error 
        codes.
        
@see KEntryAttNormal        
*/
	{
	FILEHANDLE(h);
	RFs fs;
	return fs.Att(h->iName, aVal);
	}

EXPORT_C TInt RFile::SetAtt(TUint aSetAttMask,TUint aClearAttMask)
/**
Sets or clears file attributes using two bitmasks.

The first mask controls which attributes are set.
The second controls which attributes are cleared.

Notes:

1. The file must have been opened for writing, or an error is returned.

2. A panic is raised if any attribute is specified in both bitmasks.

3. An attempt to set or clear the KEntryAttDir, KEntryAttVolume or KEntryAttRemote
   attributes have no effect.

4. The new attribute values take effect when the file is flushed or closed (which 
   implies a flush).

@param aSetAttMask   A bitmask indicating the file attributes to be set
@param aClearAttMask A bitmask indicating the attributes to be cleared. For 
                     more information see KEntryAttNormal, and the other
                     file/directory attributes.
                     
@return KErrNone if successful, otherwise one of the other system-wide error 
        codes.
        
@panic FSCLIENT 21 if the same attribute bit is set in both bitmasks.
*/
	{
	FILEHANDLE(h);
	RFs fs;
	return fs.SetAtt(h->iName, aSetAttMask, aClearAttMask);
	}

EXPORT_C TInt RFile::Modified(TTime& aTime) const
/**
Gets local date and time the file was last modified, in universal time.

@param aTime On return, contains the date and time the file was last modified in UTC.

@return KErrNone if successful, otherwise one of the other system-wide error 
        codes.
*/
	{
	FILEHANDLE(h);
	RFs fs;
	TEntry entry;
	TInt err = fs.Entry(h->iName, entry);
	if (err < KErrNone)
		return err;
	aTime = entry.iModified;
	return KErrNone;
	}

EXPORT_C TInt RFile::ChangeMode(TFileMode aNewMode)
/**
Switches an open file's access mode between EFileShareExclusive and EFileShareReadersOnly.

This allows or disallows read-only access without having to close and re-open the file.

@param aNewMode The new access mode.

@return KErrNone, if successful;
        KErrArgument, if aNewMode has any value other than the two specified;
        KErrAccessDenied, if:
        a) the function is called when the current file share
        mode is EFileShareAny;
        b) the file has multiple readers, and an attempt is made
        to change the share mode to EFileShareExclusive; 
        c) the file has been opened for writing in EFileShareExclusive mode, and an 
        attempt is made to change the access mode to EFileShareReadersOnly.

@capability Dependent If the path starts with /Resource then capability DiskAdmin is required

*/
	{
	// Pretend it worked!
	return KErrNone;
	}

EXPORT_C TInt RFile::Drive(TInt &aDriveNumber, TDriveInfo &aDriveInfo) const
/**
Gets information about the drive on which this file resides.
 
@param aDriveNumber On return, the drive number.

@param aDriveInfo   On return, contains information describing the drive
                    and the medium mounted on it. The value of TDriveInfo::iType
                    shows whether the drive contains media.

@return       KErrNone, if successful, otherwise one of the other
              system-wide error codes
              
@see RFs::Drive
*/
	{
	aDriveNumber = EDriveC;
	aDriveInfo.iType = EMediaHardDisk;
	aDriveInfo.iBattery = EBatNotSupported;
	aDriveInfo.iDriveAtt = KDriveAttLocal|KDriveAttInternal;
	aDriveInfo.iMediaAtt = 0;
	return KErrNone;
	}

EXPORT_C TInt RFile::Rename(const TDesC& aNewName)
/**
Renames a file.

If aNewName specifies a different directory to the one in which 
the file is currently located, then the file is moved.

@param aNewName The new file name and/or directory path. No part may contain 
                wildcard characters or an error is returned.
                
@return KErrNone if successful, otherwise one of the other system-wide error 
        codes.
*/
	{
	FILEHANDLE(h);
	TFileName name(h->iName);
	
	// Need the file to be opened for write
	if (!(h->iMode & EFileWrite))
		return KErrAccessDenied;

	TBuf8<KMaxFileName + 1> copyOld;
	const char* oldName = PosixFilename(name, copyOld);
	
	TBuf8<KMaxFileName + 1> copyNew;
	const char* newName = PosixFilename(aNewName, copyNew);
	
	// Remember the file position
	TInt pos = 0;
	TInt err = Seek(ESeekCurrent, pos);
	if (err != KErrNone)
		return err;

	// Close the file
	Close();
	
	// Rename the file
	if (rename(oldName, newName) != 0)
		return mapErr(errno);
	
	// Open the file again
	RFs fs;
	err = Open(fs, aNewName, EFileWrite);
	if (err != KErrNone)
		return err;

	// Reposition the file pointer
	return Seek(ESeekStart, pos);
	}

