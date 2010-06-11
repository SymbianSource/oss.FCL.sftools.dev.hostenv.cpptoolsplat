// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifdef __TOOLS2__
#include <sys/stat.h>
#include <stdlib.h>
#ifdef linux
#include <string.h>
#endif
#endif

#include <e32test.h>
#include <f32file.h>

RTest test(_L("SymFile RFs Tests"));

void testAtt()
	{
	// Connect to the file system
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	_LIT(KTestDir1, "c:\\symport_test_rfs1\\");
	_LIT(KTestFile, "c:\\symport_test_rfs1\\test.txt");
	
	// Test with root
	TUint att;
	test(fs.Att(_L("C:\\"), att) == KErrBadName);
	test(fs.Att(_L("\\"), att) == KErrBadName);

	// Make sure the directory doesn't exist
	TInt err = fs.Delete(KTestFile);
	test(err == KErrNone || err == KErrNotFound || err == KErrPathNotFound);
	err = fs.RmDir(KTestDir1);
	test(err == KErrNone || err == KErrNotFound);

	// Check it fails
	test(fs.Att(KTestDir1, att) == KErrNotFound);

	// See if it works for directories
	test(fs.MkDir(KTestDir1) == KErrNone);
	test(fs.Att(KTestDir1, att) == KErrNone && att&KEntryAttDir);

	// See if it works for normal files
	test(fs.Att(KTestFile, att) == KErrNotFound);
	RFile file;
	test(file.Create(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();
	test(fs.Att(KTestFile, att) == KErrNone);
	test(!(att&KEntryAttDir));

	// Check readonly flag
	test(!(att&KEntryAttReadOnly));
	test(fs.SetAtt(KTestFile, KEntryAttReadOnly, 0) == KErrNone);
	test(fs.Att(KTestFile, att) == KErrNone && att&KEntryAttReadOnly);
	
	// Clear the flag
	test(fs.SetAtt(KTestFile, 0, KEntryAttReadOnly) == KErrNone);
	test(fs.Att(KTestFile, att) == KErrNone);
	test(!(att&KEntryAttReadOnly));

	// Delete the file and make sure it goes away
	test(fs.RmDir(KTestDir1) == KErrInUse);
	test(fs.Delete(KTestFile) == KErrNone);
	test(fs.Att(KTestFile, att) == KErrNotFound);
	
	// Delete directory and make sure it really went away
	test(fs.RmDir(KTestDir1) == KErrNone);
	test(fs.Att(KTestDir1, att) == KErrNotFound);

	fs.Close();
	}

void testSetAtt()
	{
	// Connect to the file system
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	// Make sure the file doesn't exist
	_LIT(KTestFile, "c:\\symport_test_rfs2_test.txt");
	fs.SetAtt(KTestFile, 0, KEntryAttReadOnly);
	fs.Delete(KTestFile);

	// File doesn't exist
	test(fs.SetAtt(KTestFile, KEntryAttReadOnly, 0) == KErrNotFound);
	
	// Create a file
	RFile file;
	test(file.Create(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();

	// Set readonly flag
	TUint att;
	test(fs.Att(KTestFile, att) == KErrNone);
	test(!(att&KEntryAttReadOnly));
	test(fs.SetAtt(KTestFile, KEntryAttReadOnly, 0) == KErrNone);
	test(fs.Att(KTestFile, att) == KErrNone && att&KEntryAttReadOnly);

	// Clear readonly flag
	test(fs.SetAtt(KTestFile, 0, KEntryAttReadOnly) == KErrNone);
	test(fs.Att(KTestFile, att) == KErrNone);
	test(!(att&KEntryAttReadOnly));
	
	test(fs.Delete(KTestFile) == KErrNone);
	fs.Close();
	}

void testModified()
	{
	// Connect to the file system
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	// Make sure the file doesn't exist
	_LIT(KTestFile, "c:\\symport_test_rfs2a_test.txt");
	fs.Delete(KTestFile);

	// Get time now
	TTime start;
	start.UniversalTime();
	
	// File doesn't exist
	TTime time1;
	test(fs.Modified(KTestFile, time1) == KErrNotFound);
	User::After(1000000);
	
	// Create a file
	RFile file;
	test(file.Create(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();

	// Get modified time
	test(fs.Modified(KTestFile, time1) == KErrNone && time1 > start);
	User::After(1000000);

	// Edit the file
	test(file.Open(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();
	
	// Get modified time
	TTime time2;
	test(fs.Modified(KTestFile, time2) == KErrNone && time2 > time1);
	
	test(fs.Delete(KTestFile) == KErrNone);
	fs.Close();
	}

void testMkDirAll()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	_LIT(KTestFile, "c:\\symport_test_rfs3_parent\\symport_test_rfs3_child");
	fs.Delete(KTestFile);
	
	// Make sure the directories don't exist already
	_LIT(KTestDirParent1, "c:\\symport_test_rfs3_parent\\");
	_LIT(KTestDir1, "c:\\symport_test_rfs3_parent\\symport_test_rfs3_child\\");
	fs.RmDir(KTestDir1);
	fs.RmDir(KTestDirParent1);
	
	// Check it can handle drive letter
	test(fs.MkDirAll(KTestDir1) == KErrNone);
	test(fs.MkDirAll(KTestDir1) == KErrAlreadyExists);
	TUint att;
	test(fs.Att(KTestDir1, att) == KErrNone && att&KEntryAttDir);	

	// Delete the folder for the next test
	test(fs.RmDir(KTestDir1) == KErrNone && fs.RmDir(KTestDirParent1) == KErrNone);
	TInt err = fs.Att(KTestDir1, att);
	test(err == KErrNotFound || err == KErrPathNotFound);	
		
	// Check it fails if file is encountered
	test(fs.MkDir(KTestDirParent1) == KErrNone);
	RFile file;
	test(file.Create(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();
	test(fs.MkDirAll(KTestDir1) == KErrAccessDenied);
	
	// Check for failure on itermediate file
	_LIT(KTestDir3, "c:\\symport_test_rfs3_parent\\symport_test_rfs3_child\\wibble\\");
	test(fs.MkDirAll(KTestDir3) == KErrAccessDenied);

	test(fs.Delete(KTestFile) == KErrNone);
	test(fs.RmDir(KTestDirParent1) == KErrNone);
	
	fs.Close();
	}

void testMkDir()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);

	_LIT(KTestDir1, "c:\\symport_test_rfs5\\");
	_LIT(KTestFile, "c:\\symport_test_rfs5");
	_LIT(KTestDir1Sub, "c:\\symport_test_rfs5\\sub\\");

	// Make sure the test stuff doesn't exist already
	fs.RmDir(KTestDir1Sub);
	fs.Delete(KTestFile);
	fs.RmDir(KTestDir1);
	
	// Check the directory can be created
	TUint att;
	test(fs.MkDir(KTestDir1) == KErrNone);
	test(fs.MkDir(KTestDir1) == KErrAlreadyExists);
	test(fs.Att(KTestDir1, att) == KErrNone);	
	test(fs.MkDir(KTestDir1Sub) == KErrNone);
	test(fs.MkDir(KTestDir1Sub) == KErrAlreadyExists);
	test(fs.RmDir(KTestDir1Sub) == KErrNone);
	test(fs.RmDir(KTestDir1) == KErrNone);
	test(fs.Att(KTestDir1, att) == KErrNotFound);	

	// Check file error
	RFile file;
	test(file.Create(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();
	
	test(fs.MkDir(KTestDir1) == KErrAccessDenied);
	test(fs.Delete(KTestFile) == KErrNone);
		
	fs.Close();
	}

void testRmDir()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);

	// Make sure the test stuff isn't there
	_LIT(KTestDir1, "c:\\symport_test_rfs6\\");
	_LIT(KTestFile, "c:\\symport_test_rfs6\\testfile");
	_LIT(KTestDir3, "c:\\symport_test_rfs6\\testfile\\");
	fs.Delete(KTestFile);
	fs.RmDir(KTestDir1);
	
	// Create the directory and make sure you can delete it
	TUint att;
	test(fs.MkDir(KTestDir1) == KErrNone);
	test(fs.Att(KTestDir1, att) == KErrNone && att|KEntryAttDir);	
	test(fs.RmDir(KTestDir1) == KErrNone);
	test(fs.RmDir(KTestDir1) == KErrNotFound);
	test(fs.Att(KTestDir1, att) == KErrNotFound);	
	
	test(fs.MkDir(KTestDir1) == KErrNone);
	test(fs.Att(KTestDir1, att) == KErrNone && att|KEntryAttDir);	

	RFile file;
	test(file.Create(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();

	// Check you can't delete a file or a none empty directory
	TInt err = fs.RmDir(KTestDir3);
	test(err == KErrNotFound || err == KErrPathNotFound);
	test(fs.RmDir(KTestDir1) == KErrInUse);
	test(fs.Delete(KTestFile) == KErrNone);
	test(fs.RmDir(KTestDir1) == KErrNone);

	// KErrInUse when removing a root directory
	test(fs.RmDir(_L("c:\\")) == KErrInUse);
	
	fs.Close();
	}

void testParse()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);

	_LIT(KTestFile, "c:\\symport_test_rfs7.txt");
	fs.Delete(KTestFile);
	
	// Parse file spec
	TParse parse;
	test(fs.Parse(KTestFile, parse) == KErrNone);
	
	// See if we can create the  file
	RFile file;
	test(file.Create(fs, parse.FullName(), EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();
	test(fs.Delete(parse.FullName()) == KErrNone);
	
	fs.Close();
	}

void testSessionPath()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);

	// Check we get a session path
	TFileName name;
	test(fs.SessionPath(name) == KErrNone);
	test(name.Length() > 0);
	
	// Check there's a backslash on the end and no forward slashes
	test(name[name.Length() - 1] == TChar('\\'));
	test(name.Locate(TChar('/')) == KErrNotFound);
	
	fs.Close();
	}

void testSetSessionPath()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);

	_LIT(KTestDir1, "c:\\symport_test_rfs8\\");
	_LIT(KTestFile1, "c:\\symport_test_rfs8\\test");
	_LIT(KTestFilename1, "test");
	fs.Delete(KTestFile1);
	fs.RmDir(KTestDir1);
	
	// Check we get a session path
	TFileName name;
	test(fs.SessionPath(name) == KErrNone);
	test(name.Length() > 0);

	// Check we can set it
	fs.SetSessionPath(KTestDir1);
	test(fs.MkDir(KTestDir1) == KErrNone);
	test(fs.SetSessionPath(KTestDir1) == KErrNone);
	
	// See if we can create a file in the session path
	RFile file;
	test(file.Create(fs, KTestFilename1, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();
	
	// Check the file is there
	TUint att;
	test(fs.Att(KTestFile1, att) == KErrNone);
	test(fs.Delete(KTestFilename1) == KErrNone);
	
	// Reset session path!
	test(fs.SetSessionPath(name) == KErrNone);
	test(fs.RmDir(KTestDir1) == KErrNone);
	
	fs.Close();
	}

void testDelete()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);

	_LIT(KTestDir, "c:\\symport_test_rfs9\\");
	_LIT(KTestFile, "c:\\symport_test_rfs9");
	fs.Delete(KTestFile);
	fs.RmDir(KTestDir);
	
	// Error if the file doesn't exist
	test(fs.Delete(KTestFile) == KErrNotFound);

	// Create the file
	RFile file;
	test(file.Create(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);



// On Linux it seems it's perfectly correct to "delete" an open file

// Removing an open file is sometimes recommended to hide the names of temporary files that may be prone to attack!

	TInt err = 0;
#ifndef linux

	// Should be locked

	err = fs.Delete(KTestFile);
	test(err == KErrInUse || err == KErrAccessDenied);
#endif

	file.Close();

	// Check success
	test(fs.Delete(KTestFile) == KErrNone);
	test(fs.Delete(KTestFile) == KErrNotFound);

	// Check you can't delete a directory
	test(fs.MkDir(KTestDir) == KErrNone);
	test(fs.Delete(KTestFile) == KErrAccessDenied);
	
	err = fs.Delete(KTestDir);
	test(err == KErrBadName || err == KErrAccessDenied);
	test(fs.RmDir(KTestDir) == KErrNone);
	
	fs.Close();
	}

// This is only valid for TOOLS2
#ifdef __TOOLS2__
extern TInt gBlockSize;
extern TInt gClusterSize;
void testVolumeIOParam()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	// We rely on this error code being returned - preventing STORE cleverness?
	TVolumeIOParamInfo param;
	param.iBlockSize = 0;
	param.iClusterSize = 0;
	param.iRecReadBufSize = 0;
	param.iRecWriteBufSize = 0;

	test(fs.VolumeIOParam(EDriveC, param) == KErrNone);
	
	test(param.iBlockSize == 4096);
	test(param.iClusterSize == 4096);
	test(param.iRecReadBufSize == 4096);
	test(param.iRecWriteBufSize == 4096);

	gBlockSize = 8096;
	gClusterSize = 8096;

	test(fs.VolumeIOParam(EDriveC, param) == KErrNone);

	test(param.iBlockSize == 8096);
	test(param.iClusterSize == 8096);
	test(param.iRecReadBufSize == 4096);
	test(param.iRecWriteBufSize == 4096);

	fs.Close();
	}
#endif

void testEntry()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	_LIT(KTestFile, "c:\\symport_test_rfs10");
	_LIT(KTestDir, "c:\\symport_test_rfs10_dir\\");
	fs.Delete(KTestFile);
	fs.RmDir(KTestDir);
	
	// Check error when file doesn't exist
	TEntry entry;
	test(fs.Entry(KTestFile, entry) == KErrNotFound);

	// Create test data
	TUidType testUid(TUid::Uid(0x1), TUid::Uid(0x2), TUid::Uid(0x3));
	TCheckedUid checkedUid(testUid);
	TPckg<TCheckedUid> checkedUidBuf(checkedUid);
	
	// Create the file
	RFile file;
	test(file.Create(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(checkedUidBuf) == KErrNone);
	file.Close();
		
	// Get file details
	User::After(1000000);
	test(fs.Entry(KTestFile, entry) == KErrNone);
	
	// Check the data looks right
	test(entry.iSize == checkedUidBuf.Length());
	test(entry.iType == testUid);
	test(entry.iName == KTestFile().Mid(3)); // No path
	test(!(entry.iAtt&KEntryAttDir));
	
	TTimeIntervalSeconds sec;
	TTime now; now.UniversalTime();
	test(now.SecondsFrom(entry.iModified, sec) == KErrNone);
	test(sec.Int() >= 1);
	
	_LIT8(KTestText, "Some junk that is not a uid");
	User::After(2000000);

	test(file.Replace(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(KTestText) == KErrNone);
	file.Close();

	// Get file details
	test(fs.Entry(KTestFile, entry) == KErrNone);
	
	// Check the data looks right
	test(entry.iSize == KTestText().Length());
	test(entry.iType == TUidType(TUid::Uid(0), TUid::Uid(0), TUid::Uid(0)));
	test(entry.iName == KTestFile().Mid(3)); // No path
	
	test(now.SecondsFrom(entry.iModified, sec) == KErrNone);
	test(sec.Int() <= -1);
	
	// Test it works for directories
	test(fs.Entry(KTestDir, entry) == KErrNotFound);
	test(fs.MkDir(KTestDir) == KErrNone);
	test(fs.Entry(KTestDir, entry) == KErrNone);
	test(entry.iSize == 0);
	test(entry.iType == TUidType(TUid::Null(), TUid::Null(), TUid::Null()));
	test(entry.iName == KTestDir().Mid(3, KTestDir().Length() - 4)); // Lose path and end slash
	test(entry.iAtt&KEntryAttDir);
	
	test(fs.Delete(KTestFile) == KErrNone);
	test(fs.RmDir(KTestDir) == KErrNone);
	fs.Close();
	}

void testSetEntry()
	{
	// Connect to the file system
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	TTime start;
	start.UniversalTime();
	
	// Make sure the file doesn't exist
	_LIT(KTestFile, "c:\\symport_test_rfs2b_test.txt");
	fs.SetEntry(KTestFile, start, 0, KEntryAttReadOnly);
	fs.Delete(KTestFile);

	// File doesn't exist
	test(fs.SetEntry(KTestFile, start, KEntryAttReadOnly, 0) == KErrNotFound);
	
	// Create a file
	RFile file;
	test(file.Create(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();

	// Set readonly flag
	TUint att;
	test(fs.Att(KTestFile, att) == KErrNone && !(att&KEntryAttReadOnly));
	test(fs.SetEntry(KTestFile, start, KEntryAttReadOnly, 0) == KErrNone);
	test(fs.Att(KTestFile, att) == KErrNone && att&KEntryAttReadOnly);

	// Clear readonly flag
	test(fs.SetEntry(KTestFile, start, 0, KEntryAttReadOnly) == KErrNone);
	test(fs.Att(KTestFile, att) == KErrNone && !(att&KEntryAttReadOnly));
	
	test(fs.Delete(KTestFile) == KErrNone);
	fs.Close();
	}

// Only relevant to TOOLS2
#ifdef __TOOLS2__
void testSetErrorCondition()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);
	test(fs.SetErrorCondition(KErrGeneral, 1) == KErrNotSupported);
	fs.Close();	
	}
#endif

void testCharToDrive()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	TInt drive;
	
	// Test invalid
	test(fs.CharToDrive(TChar('$'), drive) == KErrArgument);
	
	// Test valid - upper and lower case
	test(fs.CharToDrive(TChar('a'), drive) == KErrNone && drive == EDriveA);
	test(fs.CharToDrive(TChar('A'), drive) == KErrNone && drive == EDriveA);
	test(fs.CharToDrive(TChar('z'), drive) == KErrNone && drive == EDriveZ);
	test(fs.CharToDrive(TChar('Z'), drive) == KErrNone && drive == EDriveZ);
	
	fs.Close();	
	}

void testDriveToChar()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	TChar dChar;
	
	// Test invalid
	test(fs.DriveToChar(99, dChar) == KErrArgument);
	
	// Ask for default drive
	test(fs.DriveToChar(KDefaultDrive, dChar) == KErrNone);

	test(TUint(dChar) >= TUint(TChar('A')));

	test(TUint(dChar) <= TUint(TChar('Z')));

	// Check valid
	test(fs.DriveToChar(EDriveA, dChar) == KErrNone && dChar == TChar('A'));
	test(fs.DriveToChar(EDriveZ, dChar) == KErrNone && dChar == TChar('Z'));
	
	fs.Close();	
	}

void testIsValidDrive()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	// Invalid
	test(!fs.IsValidDrive(99));
	test(!fs.IsValidDrive(-99));
	
	// Valid
	test(fs.IsValidDrive(KDefaultDrive));
	test(fs.IsValidDrive(EDriveA));
	test(fs.IsValidDrive(EDriveZ));
	
	fs.Close();	
	}

void testDrive()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	// Test invalid drive
	TDriveInfo drive;
	test(fs.Drive(drive, 99) == KErrBadName);
	
#ifdef __TOOLS2__
	// Test valid drives
	test(fs.Drive(drive, EDriveC) == KErrNone);
	test(drive.iType == EMediaHardDisk && drive.iBattery == EBatNotSupported && drive.iDriveAtt&KDriveAttLocal|KDriveAttInternal && drive.iMediaAtt == 0);
	test(fs.Drive(drive, KDefaultDrive) == KErrNone);
	test(drive.iType == EMediaHardDisk && drive.iBattery == EBatNotSupported && drive.iDriveAtt&KDriveAttLocal|KDriveAttInternal && drive.iMediaAtt == 0);
#endif
	
	fs.Close();	
	}

void testGetSystemDrive()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);
	test(fs.GetSystemDrive() == EDriveC);
	test(fs.GetSystemDriveChar() == TChar('C'));
	fs.Close();
	}

void testReplace()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);

	_LIT(KTestFile1, "c:\\symport_test_rfs11a");
	_LIT(KTestFile2, "c:\\symport_test_rfs11b");
	
	fs.Delete(KTestFile1);
	fs.Delete(KTestFile2);
	
	// Error when file doesn't exist
	test(fs.Replace(KTestFile1, KTestFile2) == KErrNotFound);
	
	RFile file;
	test(file.Create(fs, KTestFile1, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();

	// Test success
	test(fs.Replace(KTestFile1, KTestFile2) == KErrNone);
	
	TUint att;
	test(fs.Att(KTestFile1, att) == KErrNotFound);
	test(fs.Att(KTestFile2, att) == KErrNone);
	test(fs.Replace(KTestFile2, KTestFile1) == KErrNone);
	test(fs.Att(KTestFile1, att) == KErrNone);
	test(fs.Att(KTestFile2, att) == KErrNotFound);

	// Test when both files exist
	test(file.Create(fs, KTestFile2, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();
	
	test(fs.Att(KTestFile1, att) == KErrNone);
	test(fs.Att(KTestFile2, att) == KErrNone);
	test(fs.Replace(KTestFile1, KTestFile2) == KErrNone);
	test(fs.Att(KTestFile1, att) == KErrNotFound);
	test(fs.Att(KTestFile2, att) == KErrNone);

	test(fs.Replace(KTestFile2, KTestFile1) == KErrNone);
	test(fs.Att(KTestFile1, att) == KErrNone);
	test(fs.Att(KTestFile2, att) == KErrNotFound);

	test(fs.Delete(KTestFile1) == KErrNone);
	fs.Close();
	}

void testRename()
	{
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	_LIT(KTestFile1, "c:\\symport_test_rfs12a");
	_LIT(KTestFile2, "c:\\symport_test_rfs12b");

	_LIT(KTestDir1, "c:\\symport_test_rfs12a\\");
	_LIT(KTestDir2, "c:\\symport_test_rfs12b\\");

	fs.Delete(KTestFile1);
	fs.Delete(KTestFile2);
	fs.RmDir(KTestDir1);
	fs.RmDir(KTestDir2);
	
	// Error when source doesn't exist
	test(fs.Rename(KTestFile1, KTestFile2) == KErrNotFound);
	
	// Error when destination does exist
	RFile file;
	test(file.Create(fs, KTestFile1, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();

	test(file.Create(fs, KTestFile2, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();

	test(fs.Rename(KTestFile1, KTestFile2) == KErrAlreadyExists);
	test(fs.Delete(KTestFile2) == KErrNone);
	
	// Test Success
	test(fs.SetAtt(KTestFile1, 0, KEntryAttReadOnly) == KErrNone);
	test(fs.Rename(KTestFile1, KTestFile2) == KErrNone);

	TUint att;
	test(fs.Att(KTestFile1, att) == KErrNotFound);
	test(fs.Att(KTestFile2, att) == KErrNone);
	test(fs.Rename(KTestFile2, KTestFile1) == KErrNone);
	test(fs.Att(KTestFile1, att) == KErrNone);
	test(fs.Att(KTestFile2, att) == KErrNotFound);

	test(fs.Delete(KTestFile1) == KErrNone);
		
	fs.Close();	
	}

TPtrC test_string=_L("Y:\\");
LOCAL_C void TestTParse1()
//
// Test all TParse methods
//
	{
	TBuf<16> relatedFiles(_L(".CCC"));
	TBuf<16> defaultPath(_L("C:\\"));
	TParse parser;
	TInt r=parser.Set(_L("\\WWW\\XXX\\YYY\\ZZZ\\AAA"),&relatedFiles,&defaultPath);
	test(r==KErrNone);
	test(parser.FullName()==_L("C:\\WWW\\XXX\\YYY\\ZZZ\\AAA.CCC"));
	test(parser.Drive()==_L("C:"));
	test(parser.Path()==_L("\\WWW\\XXX\\YYY\\ZZZ\\"));
	test(parser.DriveAndPath()==_L("C:\\WWW\\XXX\\YYY\\ZZZ\\"));
	test(parser.Name()==_L("AAA"));
	test(parser.Ext()==_L(".CCC"));
	test(parser.NameAndExt()==_L("AAA.CCC"));
	test(parser.DrivePresent()==EFalse);
	test(parser.PathPresent());
	test(parser.NamePresent());
	test(parser.ExtPresent()==EFalse);
	test(parser.NameOrExtPresent());
	test(parser.IsRoot()==EFalse);
	test(parser.IsWild()==EFalse);
	test(parser.IsNameWild()==EFalse);
	test(parser.IsExtWild()==EFalse);
	r=parser.SetNoWild(_L("\\WWW\\XXX\\YYY\\ZZZ\\AAA.EXT"),&relatedFiles,&defaultPath);
	test(r==KErrNone);
	test(parser.PopDir()==KErrNone);
	test(parser.AddDir(_L("BBB"))==KErrNone);
	}

LOCAL_C void TestTParse2()
//
// Test multiple PopDirs
//
	{

	TParse parser;
	TInt r=parser.Set(_L("\\WWW\\XXX\\YYY\\ZZZ\\"),NULL,NULL);
//	TParsePtrC parser(_L("\\WWW\\XXX\\YYY\\ZZZ\\"));
	test(r==KErrNone);
	r=parser.PopDir();
	test(r==KErrNone);
	test(parser.Path()==_L("\\WWW\\XXX\\YYY\\"));
	r=parser.PopDir();
	test(r==KErrNone);
	test(parser.Path()==_L("\\WWW\\XXX\\"));
	r=parser.PopDir();
	test(r==KErrNone);
	test(parser.Path()==_L("\\WWW\\"));
	r=parser.PopDir();
	test(r==KErrNone);
	test(parser.Path()==_L("\\"));
	r=parser.PopDir();
	test(r==KErrGeneral);
//
	test(parser.Set(_L("C:\\Documents\\.TXT"),NULL,NULL)==KErrNone);
	test(parser.PopDir()==KErrNone);
	test(parser.FullName()==_L("C:\\.TXT"));
	}

LOCAL_C void TestTParse3()
//
// Test conflicting drive letters
//
	{

	TParse parser;
    TPtrC one=_L("\\ONE\\");
    TPtrC null=_L("\\");
    TPtrC x=_L("X:");
    TPtrC x2=_L("X:\\");
    TPtrC z=_L("Z:");
	TInt r=parser.Set(_L("Z:\\Hello"),&one,&null);
	test(r==KErrNone);
	test(parser.FullName()==_L("Z:\\Hello"));
    TPtrC sht=_L("*.SHT");
    r=parser.Set(_L("Z:"),&sht,&x);
	test(r==KErrNone);
	test(parser.FullName()==_L("Z:*.SHT"));
	r=parser.Set(_L("Hello"),&z,&x2);
	test(r==KErrNone);
	test(parser.FullName()==_L("Z:\\Hello"));
	r=parser.Set(_L("W:\\Hello"),&z,&x2);
	test(r==KErrNone);
	test(parser.FullName()==_L("W:\\Hello"));
    TPtrC abcdefg=_L("abcdefg");
    TPtrC onetwo=_L("X:\\ONE\\TWO\\.CCC");
	r=parser.Set(_L("W:"),&abcdefg,&onetwo);
	test(r==KErrNone);
	test(parser.FullName()==_L("W:\\ONE\\TWO\\abcdefg.CCC"));
    TPtrC y=_L("Y:");
    TPtrC xhello=_L("X:\\HELLO\\");
    r=parser.Set(_L("World"),&y,&xhello);
	test(r==KErrNone);
	test(parser.FullName()==_L("Y:\\HELLO\\World"));
    TPtrC xhelloext=_L("X:\\HELLO\\.EXT");
    r=parser.Set(_L("World"),&y,&xhelloext);
	test(r==KErrNone);
	test(parser.FullName()==_L("Y:\\HELLO\\World.EXT"));
	}

LOCAL_C void TestTParse4()
//
// Conflicting relative path drives and names
//
	{

	TParse parser;
    TPtrC xone=_L("X:\\ONE\\");
    TPtrC y=_L("Y:\\");
	TInt r=parser.Set(_L("Z:\\Hello"),&xone,&y);
	test(r==KErrNone);
	test(parser.FullName()==_L("Z:\\Hello"));
    TPtrC zone=_L("Z:\\ONE\\");
    TPtrC xnew=_L("X:\\NEW\\");
    r=parser.Set(_L("\\Hello"),&zone,&xnew);
	test(r==KErrNone);
	test(parser.FullName()==_L("Z:\\Hello"));
    TPtrC aone=_L("A:\\ONE\\");
    TPtrC anew=_L("A:\\NEW\\");
    r=parser.Set(_L("A:Hello"),&aone,&anew);
	test(r==KErrNone);
	test(parser.FullName()==_L("A:\\ONE\\Hello"));
    TPtrC a=_L("A:\\");
    r=parser.Set(_L("Hello"),&a,&xnew);
	test(r==KErrNone);
	test(parser.FullName()==_L("A:\\Hello"));
	r=parser.Set(_L("Hello"),&aone,&xnew);
	test(r==KErrNone);
	test(parser.FullName()==_L("A:\\ONE\\Hello"));
	}


LOCAL_C void TestTParse5()
//
// Test illegal paths
//
	{

	TParse parser;
	TInt r=parser.Set(_L("FOO\\"),NULL,NULL);
	test(r==KErrBadName);
	r=parser.Set(_L("C:\\FOO\\\\"),NULL,NULL);
	test(r==KErrNone);
	}

LOCAL_C void TestTParse6()
//
// Test AddDir
//
	{

	TParse parser;
	test(parser.Set(_L("C:\\"),NULL,NULL)==KErrNone);
	test(parser.IsRoot());
	test(parser.FullName()==_L("C:\\"));
	test(parser.AddDir(_L("HELLO"))==KErrNone);
	test(parser.IsRoot()==EFalse);
	test(parser.FullName()==_L("C:\\HELLO\\"));
	test(parser.AddDir(_L("BYEBYE"))==KErrNone);
	test(parser.IsRoot()==EFalse);
	test(parser.FullName()==_L("C:\\HELLO\\BYEBYE\\"));
	test(parser.PopDir()==KErrNone);
	test(parser.IsRoot()==EFalse);
	test(parser.FullName()==_L("C:\\HELLO\\"));
	test(parser.PopDir()==KErrNone);
	test(parser.IsRoot());
	test(parser.FullName()==_L("C:\\"));
//
	test(parser.AddDir(_L(""))==KErrNone);
	test(parser.IsRoot());
	test(parser.FullName()==_L("C:\\"));
	test(parser.AddDir(_L("HELLO"))==KErrNone);
	test(parser.IsRoot()==EFalse);
	test(parser.FullName()==_L("C:\\HELLO\\"));
	test(parser.AddDir(_L(""))==KErrNone);
	test(parser.IsRoot()==EFalse);
	test(parser.FullName()==_L("C:\\HELLO\\"));
//
	test(parser.Set(_L("C:\\Documents\\.TXT"),NULL,NULL)==KErrNone);
	test(parser.AddDir(_L("Documents"))==KErrNone);
	test(parser.FullName()==_L("C:\\Documents\\Documents\\.TXT"));
	}

LOCAL_C void TestTParse7()
//
// Test TParsePtr
//
	{

	TBuf<128> nameBuf=_L("C:\\WWW\\XXX\\YYY\\ZZZ\\AAA.CCC");
	TParsePtr parser(nameBuf);

	test(parser.FullName()==_L("C:\\WWW\\XXX\\YYY\\ZZZ\\AAA.CCC"));
	test(parser.Drive()==_L("C:"));
	test(parser.Path()==_L("\\WWW\\XXX\\YYY\\ZZZ\\"));
	test(parser.DriveAndPath()==_L("C:\\WWW\\XXX\\YYY\\ZZZ\\"));
	test(parser.Name()==_L("AAA"));
	test(parser.Ext()==_L(".CCC"));
	test(parser.NameAndExt()==_L("AAA.CCC"));
	test(parser.DrivePresent());
	test(parser.PathPresent());
	test(parser.NamePresent());
	test(parser.ExtPresent());
	test(parser.NameOrExtPresent());
	test(parser.IsRoot()==EFalse);
	test(parser.IsWild()==EFalse);
	test(parser.IsNameWild()==EFalse);
	test(parser.IsExtWild()==EFalse);
	
	test(parser.AddDir(_L("HELLO"))==KErrNone);
	test(parser.Path()==_L("\\WWW\\XXX\\YYY\\ZZZ\\HELLO\\"));

	TBuf<16> shortName=_L("1234567812345678");
	TParsePtr parser2(shortName);
	test(parser2.FullName()==_L("1234567812345678"));
	test(parser2.Path()==_L(""));
	test(parser2.DriveAndPath()==_L(""));
	test(parser2.Ext()==_L(""));
	test(parser2.Name()==_L("1234567812345678"));
	test(parser2.AddDir(_L("TOOBIG"))==KErrGeneral);
	}

LOCAL_C void TestTParse8()
//
// Test TParsePtrC
//
	{
	
	TBuf<128> nameBuf=_L("C:\\WWW\\XXX\\YYY\\ZZZ\\AAA.CCC");
	TParsePtrC parser(nameBuf);

	test(parser.FullName()==_L("C:\\WWW\\XXX\\YYY\\ZZZ\\AAA.CCC"));
	test(parser.Drive()==_L("C:"));
	test(parser.Path()==_L("\\WWW\\XXX\\YYY\\ZZZ\\"));
	test(parser.DriveAndPath()==_L("C:\\WWW\\XXX\\YYY\\ZZZ\\"));
	test(parser.Name()==_L("AAA"));
	test(parser.Ext()==_L(".CCC"));
	test(parser.NameAndExt()==_L("AAA.CCC"));
	test(parser.DrivePresent());
	test(parser.PathPresent());
	test(parser.NamePresent());
	test(parser.ExtPresent());
	test(parser.NameOrExtPresent());
	test(parser.IsRoot()==EFalse);
	test(parser.IsWild()==EFalse);
	test(parser.IsNameWild()==EFalse);
	test(parser.IsExtWild()==EFalse);
	}

LOCAL_C void TestTParse9()
//
// Test names with leading spaces
//
	{

	TParse parser;
	TBuf<16> nameBuf=_L("   name.txt");
	TBuf<16> pathBuf=_L("\\PATH\\");
	
	TInt r=parser.Set(pathBuf,NULL,&nameBuf);
	test(r==KErrNone);
	test(parser.FullName()==_L("\\PATH\\   name.txt"));
	r=parser.Set(_L(""),&nameBuf,&pathBuf);
	test(r==KErrNone);
	test(parser.FullName()==_L("\\PATH\\   name.txt"));
	r=parser.Set(_L("   name.txt"),NULL,&pathBuf);
	test(r==KErrNone);
	test(parser.FullName()==_L("\\PATH\\   name.txt"));
	r=parser.Set(nameBuf,&pathBuf,NULL);
	test(r==KErrNone);
	test(parser.FullName()==_L("\\PATH\\   name.txt"));
	
	TBuf<16> badPath=_L("   \\PATH\\");
	r=parser.Set(_L("C:\\"),NULL,&badPath);
	test(r==KErrBadName);
	r=parser.Set(_L("C:\\"),&badPath,NULL);
	test(r==KErrBadName);

	TBuf<16> spacePath=_L("\\  PATH\\");
	r=parser.Set(_L("C:"),&nameBuf,&spacePath);
	test(r==KErrNone);
	test(parser.FullName()==_L("C:\\  PATH\\   name.txt"));

	TBuf<32> spacename=_L("\\  name   .   txt  ");
	r=parser.Set(_L("C:"),&spacename,NULL);
	test(r==KErrNone);
	test(parser.FullName()==_L("C:\\  name   .   txt"));

// Illegal (?) values

	TBuf<16> pureSpace=_L("     ");
	r=parser.Set(_L("C:\\NAME\\"),NULL,&pureSpace);
	test(r==KErrNone);
	test(parser.FullName()==_L("C:\\NAME\\")); // Trims right off name
	r=parser.Set(_L("C:\\NAME\\   "),NULL,NULL);
	test(r==KErrNone);
	test(parser.FullName()==_L("C:\\NAME\\"));

	TBuf<16> spacePlusExt=_L("    .   ext  ");
	r=parser.Set(_L("C:\\NAME\\"),NULL,&spacePlusExt);
	test(r==KErrNone);
	test(parser.FullName()==_L("C:\\NAME\\    .   ext")); // Trims right off ext
	r=parser.Set(_L("C:\\NAME\\    .   ext   "),NULL,NULL);
	test(r==KErrNone);
	test(parser.FullName()==_L("C:\\NAME\\    .   ext"));

	TBuf<32> pathSpace=_L("\\asdf\\zxcv\\   \\asdf\\");
	r=parser.Set(_L("C:"),NULL,&pathSpace);
	test(r==KErrNone);
	test(parser.FullName()==_L("C:\\asdf\\zxcv\\   \\asdf\\")); // Leaves spaces in path
	r=parser.Set(_L("C:\\NAME\\ \\alt.sdf"),NULL,NULL);
	test(r==KErrNone);
	test(parser.FullName()==_L("C:\\NAME\\ \\alt.sdf"));


	TBuf<32> zeroPath=_L("\\asdf\\wqer\\\\asdf\\");
	r=parser.Set(_L("NAME.TXT"),NULL,&zeroPath);
	test(r==KErrNone);
	test(parser.FullName()==_L("\\asdf\\wqer\\\\asdf\\NAME.TXT")); // Leaves zerolength path
	r=parser.Set(_L("C:\\NAME\\\\alt.sdf"),NULL,NULL);
	test(r==KErrNone);
	test(parser.FullName()==_L("C:\\NAME\\\\alt.sdf"));
	}

LOCAL_C void TestTParse10()
//
// Test a very long path
//
	{

	TBuf<16> pathPart=_L("\\2345678");
	TBuf<512> testPath;

	for(TInt i=0;i<63;i++)
		testPath+=pathPart;

	RFs fs;
	TInt r=fs.Connect();
	test(r==KErrNone);
	TParse parse;
	r=fs.Parse(testPath,parse);
	test(r==KErrBadName);
	fs.Close();

	TFileName longFileName;
	longFileName.SetLength(254);
//	Mem::Fill((TUint8*)longFileName.Ptr(),254,'A');
	Mem::Fill((TUint8*)longFileName.Ptr(),254*sizeof(TText),'A');
	longFileName[0]='\\';
	longFileName[253]='\\';
	r=parse.Set(longFileName,&test_string,NULL);
	test(r==KErrNone);
	r=parse.PopDir();
	test(r==KErrNone);

	longFileName[123]='\\';
	r=parse.Set(longFileName,&test_string,NULL);
	test(r==KErrNone);
	r=parse.PopDir();
	test(r==KErrNone);
	TPtrC startPath((TText*)longFileName.Ptr(),124);
	test(parse.Path()==startPath);

	TPtrC endPath((TText*)longFileName.Ptr()+124,252-124+1);
	r=parse.AddDir(endPath);
	test(r==KErrNone);
	test(parse.Path()==longFileName);
	}

LOCAL_C void testTFileTextApi()
//
// Test TFileText class methods
//
	{

	TPtrC record[5];
	record[0].Set(_L("First record"));
	record[1].Set(_L("Second record"));
	record[2].Set(_L("Third record"));
	record[3].Set(_L("Fourth record"));
	record[4].Set(_L("Fifth record"));
	
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	RFile f;
	TInt r=f.Replace(fs,_L("TEXTFILE.TXT"),0);
	test(r==KErrNone);
	TFileText textFile;
	textFile.Set(f);
	TInt i=0;
	for (i=0;i<5;i++)
		{
		r=textFile.Write(record[i]);
		test(r==KErrNone);
		}
	r=textFile.Seek(ESeekStart);
	test(r==KErrNone);
	TBuf<16> recBuf;
	for(i=0;i<5;i++)
		{
		r=textFile.Read(recBuf);
		test(r==KErrNone);
		test(recBuf==record[i]);
		}
	r=textFile.Read(recBuf);
	test(r==KErrEof);
	test(recBuf.Length()==0);
	f.Close();

	test.Next(_L("Test dosfile terminator"));
	TPtrC8 trecord[7];
	TPtrC tTextrecord[7];
	tTextrecord[0].Set(_L("First record\r\n"));
	tTextrecord[1].Set(_L("Second record\r\n"));
	tTextrecord[2].Set(_L("Third record\r\n"));
	tTextrecord[3].Set(_L("Fourth record\r\n"));
	tTextrecord[4].Set(_L("Fifth record\r\n"));
	tTextrecord[5].Set(_L("Sixth record\n\r"));
	tTextrecord[6].Set(_L("Seventh record\n"));
	trecord[0].Set((TUint8*)tTextrecord[0].Ptr(),tTextrecord[0].Length()*sizeof(TText));
	trecord[1].Set((TUint8*)tTextrecord[1].Ptr(),tTextrecord[1].Length()*sizeof(TText));
	trecord[2].Set((TUint8*)tTextrecord[2].Ptr(),tTextrecord[2].Length()*sizeof(TText));
	trecord[3].Set((TUint8*)tTextrecord[3].Ptr(),tTextrecord[3].Length()*sizeof(TText));
	trecord[4].Set((TUint8*)tTextrecord[4].Ptr(),tTextrecord[4].Length()*sizeof(TText));
	trecord[5].Set((TUint8*)tTextrecord[5].Ptr(),tTextrecord[5].Length()*sizeof(TText));
	trecord[6].Set((TUint8*)tTextrecord[6].Ptr(),tTextrecord[6].Length()*sizeof(TText));
	r=f.Replace(fs,_L("TEXTFILE.TXT"),0);
	test(r==KErrNone);
	for(i=0;i<7;i++)
		{
		TBuf8<256> buf;
		buf.Copy(trecord[i]);
		r=f.Write(buf);
		test(r==KErrNone);
		}
	textFile.Set(f);
	textFile.Seek(ESeekStart);
	for(i=0;i<5;i++)
		{
		r=textFile.Read(recBuf);
		test(r==KErrNone);
		test(recBuf==record[i]);
		}
	r=textFile.Read(recBuf);
	test(r==KErrNone);
	test(recBuf==_L("Sixth record"));
	r=textFile.Read(recBuf);
	test(r==KErrNone);
	test(recBuf==_L("\rSeventh record"));
	r=textFile.Read(recBuf);
	test(r==KErrEof);
	test(recBuf.Length()==0);
	f.Close();

	test.Next(_L("Test read with bufferSize == dataSize"));
	r=f.Replace(fs,_L("TEXTFILE.TXT"),0);
	test(r==KErrNone);
	record[0].Set(_L("1234567890123456"));
//	trecord[0].Set(_L8("1234567890123456\r\n"));
//	trecord[1].Set(_L8("1234567890123456\n"));

	TPtrC tmpTextrecord;
	tmpTextrecord.Set(_L("1234567890123456\r\n"));
	trecord[0].Set((TUint8*)tmpTextrecord.Ptr(),tmpTextrecord.Length()*sizeof(TText));

	tmpTextrecord.Set(_L("1234567890123456\n"));
	trecord[1].Set((TUint8*)tmpTextrecord.Ptr(),tmpTextrecord.Length()*sizeof(TText));

	for (i=0;i<2;i++)
		{
		r=f.Write(trecord[i]);
		test(r==KErrNone);
		}
	textFile.Set(f);
	textFile.Seek(ESeekStart);
	for(i=0;i<2;i++)
		{
		r=textFile.Read(recBuf);
		test(r==KErrNone);
		test(recBuf==record[0]);
		}
	r=textFile.Read(recBuf);
	test(r==KErrEof);
	test(recBuf.Length()==0);
	f.Close();

	test.Next(_L("Read into a buffer < recordSize"));
	TBuf<8> smallBuf;
	r=f.Open(fs,_L("TEXTFILE.txt"),0);
	test(r==KErrNone);
	textFile.Set(f);
	for(i=0;i<2;i++)
		{
		r=textFile.Read(smallBuf);
		test(r==KErrTooBig);
		test(smallBuf==_L("12345678"));
		}
	f.Close();

	test.Next(_L("Nasty cases: 1) split over buffer boundary"));
	r=f.Replace(fs,_L("TEXTFILE.txt"),0);
	test(r==KErrNone);
	HBufC* largeRecord=HBufC::NewL(600);
	largeRecord->Des().SetLength(250);
	largeRecord->Des().Fill('A');
	largeRecord->Des()[249]='\n';
	TPtrC8 bufPtr;
	bufPtr.Set((TUint8*)largeRecord->Ptr(),largeRecord->Size()); // Size() returns length in bytes
	r=f.Write(bufPtr);
	test(r==KErrNone);
	TBuf<16> boundaryBuf=_L("12345\r\n");
	bufPtr.Set((TUint8*)boundaryBuf.Ptr(),boundaryBuf.Size());
	r=f.Write(bufPtr);
	test(r==KErrNone);
	r=f.Write(trecord[0]);
	test(r==KErrNone);

	textFile.Set(f);
	textFile.Seek(ESeekStart);
	r=textFile.Read(recBuf);
	test(r==KErrTooBig);
	test(recBuf==_L("AAAAAAAAAAAAAAAA"));
	r=textFile.Read(recBuf);
	test(r==KErrNone);
	test(recBuf==_L("12345"));
	r=textFile.Read(recBuf);
	test(r==KErrNone);
	test(recBuf==record[0]);
	f.Close();

	test.Next(_L("Nasty cases: 2) on buffer boundary"));
	r=f.Replace(fs,_L("TEXTFILE.txt"),0);
	test(r==KErrNone);
	largeRecord->Des().SetLength(250);
	largeRecord->Des().Fill('A');
	largeRecord->Des()[249]='\n';
	bufPtr.Set((TUint8*)largeRecord->Ptr(),largeRecord->Size());
	r=f.Write(bufPtr);
	test(r==KErrNone);
	boundaryBuf=_L("12345\rxyz\n");
	bufPtr.Set((TUint8*)boundaryBuf.Ptr(),boundaryBuf.Size());
	r=f.Write(bufPtr);
	test(r==KErrNone);
	r=f.Write(trecord[0]);
	test(r==KErrNone);

	textFile.Set(f);
	textFile.Seek(ESeekStart);
	r=textFile.Read(recBuf);
	test(r==KErrTooBig);
	test(recBuf==_L("AAAAAAAAAAAAAAAA"));
	r=textFile.Read(recBuf);
	test(r==KErrNone);
	test(recBuf==_L("12345\rxyz"));
	r=textFile.Read(recBuf);
	test(r==KErrNone);
	test(recBuf==record[0]);
	f.Close();

	test.Next(_L("Nasty cases: 3) record size > buffer size"));
	r=f.Replace(fs,_L("TEXTFILE.txt"),0);
	test(r==KErrNone);
	largeRecord->Des().SetLength(600);
	largeRecord->Des().Fill('Z');
	largeRecord->Des()[511]='\r';
	largeRecord->Des()[599]='\n';
	bufPtr.Set((TUint8*)largeRecord->Ptr(),largeRecord->Size());
	r=f.Write(bufPtr);
	test(r==KErrNone);
	boundaryBuf=_L("12345\rxyz\n");
	bufPtr.Set((TUint8*)boundaryBuf.Ptr(),boundaryBuf.Size());
	r=f.Write(bufPtr);
	test(r==KErrNone);
	r=f.Write(trecord[0]);
	test(r==KErrNone);

	textFile.Set(f);
	textFile.Seek(ESeekStart);
	r=textFile.Read(recBuf);
	test(r==KErrTooBig);
	test(recBuf==_L("ZZZZZZZZZZZZZZZZ"));
	r=textFile.Read(recBuf);
	test(r==KErrNone);
	test(recBuf==_L("12345\rxyz"));
	r=textFile.Read(recBuf);
	test(r==KErrNone);
	test(recBuf==record[0]);
	
	TBuf<601> bigBuf;
	TPtrC largePtr((TText*)largeRecord->Ptr(),(largeRecord->Length()-1));
	textFile.Seek(ESeekStart);
	r=textFile.Read(bigBuf);
	test(r==KErrNone);
	test(bigBuf==largePtr);
	r=textFile.Read(recBuf);
	test(r==KErrNone);
	test(recBuf==_L("12345\rxyz"));
	r=textFile.Read(recBuf);
	test(r==KErrNone);
	test(recBuf==record[0]);
	f.Close();

	User::Free(largeRecord);
	test(fs.Delete(_L("TEXTFILE.txt")) == KErrNone);
	fs.Close();
	}

LOCAL_C void testTFileTextEndRecord()
//
// Test terminating record
//
	{
	RFs fs;
	test(fs.Connect() == KErrNone);

	RFile f;
	TInt r=f.Replace(fs,_L("TextFile"),0);
	test(r==KErrNone);
	TPtrC8 bufPtr;
	TBuf<16>boundaryBuf=_L("Record1\n");
	bufPtr.Set((TUint8*)boundaryBuf.Ptr(),boundaryBuf.Size());	
	r=f.Write(bufPtr);
	test(r==KErrNone);
	boundaryBuf=_L("Record2\n");
	bufPtr.Set((TUint8*)boundaryBuf.Ptr(),boundaryBuf.Size());	
	r=f.Write(bufPtr);
	test(r==KErrNone);
	boundaryBuf=_L("Record3\n");
	bufPtr.Set((TUint8*)boundaryBuf.Ptr(),boundaryBuf.Size());	
	r=f.Write(bufPtr);
	test(r==KErrNone);
	
	TFileText fText;
	fText.Set(f);
	r=fText.Seek(ESeekStart);
	test(r==KErrNone);
	TBuf<32> recBuf;
	r=fText.Read(recBuf);
	test(r==KErrNone);
	test(recBuf.MatchF(_L("record1"))!=KErrNotFound);
	r=fText.Read(recBuf);
	test(r==KErrNone);
	test(recBuf.MatchF(_L("record2"))!=KErrNotFound);
	r=fText.Read(recBuf);
	test(r==KErrNone);
	test(recBuf.MatchF(_L("record3"))!=KErrNotFound);
	r=fText.Read(recBuf);
	test(r==KErrEof);
	test(recBuf.Length()==0);
	f.Close();

	TBuf<0x100> bigBuf(0x100);
	bigBuf.Fill('A');
	r=f.Replace(fs,_L("TextFile"),0);
	test(r==KErrNone);
	
    bufPtr.Set((TUint8*)bigBuf.Ptr(),bigBuf.Size());	
	r=f.Write(bufPtr);
	test(r==KErrNone);

	fText.Set(f);
	r=fText.Seek(ESeekStart);
	test(r==KErrNone);
	bigBuf.SetLength(0);
	r=fText.Read(bigBuf);
//	test.Printf(_L("fText.Read returns %d\n"),r);
	test(r==KErrNone);
//	test.Printf(_L("BigBuf.Length()==%d\n"),bigBuf.Length());
	test(bigBuf.Length()==0x100);
	r=fText.Read(bigBuf);
	test(r==KErrEof);
	test(bigBuf.Length()==0);
	f.Close();
	
	test(fs.Delete(_L("TextFile")) == KErrNone);
	fs.Close();
	}

void testRFileOpen()
	{
	_LIT(KTestFile, "c:\\symport_test_rfile_open");
	_LIT8(KTestText, "abcdefghijklmnopqrstuvwxyz");
	
	RFs fs;
	test(fs.Connect() == KErrNone);

	fs.SetAtt(KTestFile, 0, KEntryAttReadOnly);
	fs.Delete(KTestFile);
	
	// File doesn't exist
	RFile file;
	test(file.Open(fs, KTestFile, 0) == KErrNotFound);
	
	// Create the file
	test(file.Create(fs, KTestFile, 0) == KErrNone);
	test(file.Write(KTestText) == KErrNone);

	// Should be okay to close a file twice
	file.Close();
	file.Close();
	
	// Open the file for reading
	TBuf8<26> buf;
	test(file.Open(fs, KTestFile, EFileShareAny) == KErrNone);
	test(file.Read(buf) == KErrNone && buf == KTestText);

	// Check you can't write to it
	test(file.Write(KTestText) == KErrAccessDenied);
	
	// Check another file can be opened at the same time
	RFile file2;
	test(file2.Open(fs, KTestFile, EFileShareAny) == KErrNone);	
	test(file2.Read(buf) == KErrNone && buf == KTestText);
	file2.Close();
	test(file2.Open(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);	
	file2.Close();
	file.Close();
	
	// Open the file for writing
	test(file.Open(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	test(file.Read(buf) == KErrNone && buf == KTestText);
	
	// Check you can write to it
	test(file.Write(KTestText) == KErrNone);
	
	// Check another file can be opened at the same time	
	test(file2.Open(fs, KTestFile, EFileShareAny|EFileRead) == KErrNone);
	file2.Close();
	test(file2.Open(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);	
	file2.Close();

	// Read back what we just wrote
	file.Close();
	test(file.Open(fs, KTestFile, 0) == KErrNone);
	test(file.Read(buf) == KErrNone && buf == KTestText);
	test(file.Read(buf) == KErrNone && buf == KTestText);
	file.Close();
	
	// Check error on read only file
	test(fs.SetAtt(KTestFile, KEntryAttReadOnly, 0) == KErrNone);
	test(file.Open(fs, KTestFile, EFileWrite) == KErrAccessDenied);
	test(fs.SetAtt(KTestFile, 0, KEntryAttReadOnly) == KErrNone);	
	test(file.Open(fs, KTestFile, EFileWrite) == KErrNone);
	file.Close();

	test(fs.Delete(KTestFile) == KErrNone);
	fs.Close();
	}

void testRFileCreate()
	{
	_LIT(KTestFile, "c:\\symport_test_rfile_create");
	_LIT8(KTestText, "abcdefghijklmnopqrstuvwxyz");
	
	RFs fs;
	test(fs.Connect() == KErrNone);
	fs.Delete(KTestFile);
	
	// Create should work when file doesn't exist
	RFile file;
	test(file.Create(fs, KTestFile, 0) == KErrNone);
	test(file.Write(KTestText) == KErrNone);
	file.Close();
	
	// Create should fail when file does exist
	test(file.Create(fs, KTestFile, 0) == KErrAlreadyExists);
	test(fs.Delete(KTestFile) == KErrNone);
	
	// Check you can read from the file
	test(file.Create(fs, KTestFile, 0) == KErrNone);
	test(file.Write(KTestText) == KErrNone);
	TInt pos = 0;
	test(file.Seek(ESeekStart, pos) == KErrNone);
	TBuf8<26> buf;
	test(file.Read(buf) == KErrNone && buf == KTestText);
	test(file.Write(KTestText) == KErrNone);
	pos = 26;
	test(file.Seek(ESeekStart, pos) == KErrNone);
	test(file.Read(buf) == KErrNone && buf == KTestText);
	file.Close();

	// Check error on read only file
	test(fs.SetAtt(KTestFile, KEntryAttReadOnly, 0) == KErrNone);
	test(file.Replace(fs, KTestFile, EFileWrite) == KErrAccessDenied);
	test(fs.SetAtt(KTestFile, 0, KEntryAttReadOnly) == KErrNone);	
	test(file.Replace(fs, KTestFile, EFileWrite) == KErrNone);
	file.Close();

	fs.Delete(KTestFile);
	fs.Close();
	}

void testRFileReplace()
	{
	_LIT(KTestFile, "c:\\symport_test_rfile_replace");
	_LIT8(KTestText, "abcdefghijklmnopqrstuvwxyz");
	
	RFs fs;
	test(fs.Connect() == KErrNone);
	fs.Delete(KTestFile);
	
	// Reaplce should work when file doesn't exist
	RFile file;
	test(file.Replace(fs, KTestFile, 0) == KErrNone);
	test(file.Write(KTestText) == KErrNone);
	file.Close();
	
	// Replace should also work when file does exist
	test(file.Replace(fs, KTestFile, 0) == KErrNone);
	test(file.Write(KTestText) == KErrNone);
	
	// Check you can read from the file
	TInt pos = 0;
	test(file.Seek(ESeekStart, pos) == KErrNone);
	TBuf8<26> buf;
	test(file.Read(buf) == KErrNone && buf == KTestText);
	test(file.Write(KTestText) == KErrNone);
	pos = 26;
	test(file.Seek(ESeekStart, pos) == KErrNone);
	test(file.Read(buf) == KErrNone && buf == KTestText);
	file.Close();

	fs.Delete(KTestFile);
	fs.Close();
	}

void testRFileTemp()
	{
	_LIT(KTestDir, "c:\\symport_test_rfile_temp\\");
	_LIT8(KTestText, "abcdefghijklmnopqrstuvwxyz");
	
	RFs fs;
	test(fs.Connect() == KErrNone);
	fs.MkDir(KTestDir);

	TFileName f1;
	RFile file;
	
	// Use invalid path
	test(file.Temp(fs, _L("invalid"), f1, 0) == KErrBadName);
	test(file.Temp(fs, _L("invalid\\"), f1, 0) == KErrBadName);
	
	// Try to use a file for the path
	test(file.Replace(fs, _L("invalid"), 0) == KErrNone);
	file.Close();
	test(file.Temp(fs, _L("invalid"), f1, 0) == KErrBadName);
	test(fs.Delete(_L("invalid")) == KErrNone);
	
	test(file.Temp(fs, KTestDir, f1, 0) == KErrNone);
	test(file.Write(KTestText) == KErrNone);
	file.Close();
	
	// Check it works a second time
	TFileName f2;
	test(file.Temp(fs, KTestDir, f2, 0) == KErrNone);
	
	// Check you can read from the file
	test(file.Write(KTestText) == KErrNone);
	TInt pos = 0;
	test(file.Seek(ESeekStart, pos) == KErrNone);
	TBuf8<26> buf;
	test(file.Read(buf) == KErrNone && buf == KTestText);
	test(file.Write(KTestText) == KErrNone);
	pos = 26;
	test(file.Seek(ESeekStart, pos) == KErrNone);
	test(file.Read(buf) == KErrNone && buf == KTestText);
	file.Close();

	test(fs.Delete(f1) == KErrNone);
	test(fs.Delete(f2) == KErrNone);

	// See if you can create a temp on root
	test(file.Temp(fs, _L("C:\\"), f1, 0) == KErrNone);
	file.Close();
	test(fs.Delete(f1) == KErrNone);

	// See if empty path is okay
	test(file.Temp(fs, KNullDesC, f1, 0) == KErrNone);
	file.Close();
	test(fs.Delete(f1) == KErrNone);

	fs.RmDir(KTestDir);
	fs.Close();
	}

void testRFileRead()
	{
	_LIT(KTestFile, "c:\\symport_test_rfile_read");
	_LIT8(KTestText, "abcdefghijklmnopqrstuvwxyz");
	
	RFs fs;
	test(fs.Connect() == KErrNone);

	// Create a test file
	RFile file;
	test(file.Replace(fs, KTestFile, EFileWrite) == KErrNone);
	test(file.Write(KTestText) == KErrNone);
	file.Close();
	
	TBuf8<13> buf;
	test(file.Open(fs, KTestFile, 0) == KErrNone);

	// File position at the start
	TInt pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 0);
	
	test(file.Read(buf) == KErrNone);
	test(buf.Length() == 13 && buf == KTestText().Left(13));
	
	// Check file position is updated
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 13);
	
	test(file.Read(buf) == KErrNone);
	test(buf.Length() == 13 && buf == KTestText().Right(13));
	test(file.Read(buf) == KErrNone);
	test(buf.Length() == 0);
	
	pos = 13;
	test(file.Seek(ESeekStart, pos) == KErrNone);
	test(file.Read(buf) == KErrNone);
	test(buf.Length() == 13 && buf == KTestText().Right(13));
	test(file.Read(buf) == KErrNone);
	test(buf.Length() == 0);

	// Move back to the start
	pos = 0;
	test(file.Seek(ESeekStart, pos) == KErrNone);
	
	test(file.Read(buf, 6) == KErrNone);
	test(buf.Length() == 6 && buf == KTestText().Left(6));
	pos = 20;
	test(file.Seek(ESeekStart, pos) == KErrNone);
	test(file.Read(buf, 6) == KErrNone);
	test(buf.Length() == 6 && buf == KTestText().Right(6));
	test(file.Read(buf, 1) == KErrNone);
	test(buf.Length() == 0);

	test(file.Read(0, buf) == KErrNone);
	test(buf.Length() == 13 && buf == KTestText().Left(13));
	test(file.Read(13, buf) == KErrNone);
	test(buf.Length() == 13 && buf == KTestText().Right(13));
	test(file.Read(23, buf) == KErrNone);
	test(buf.Length() == 3 && buf == KTestText().Right(3));
	test(file.Read(27, buf) == KErrNone);
	test(buf.Length() == 0);

	test(file.Read(0, buf, 6) == KErrNone);
	test(buf.Length() == 6 && buf == KTestText().Left(6));
	test(file.Read(20, buf, 6) == KErrNone);
	test(buf.Length() == 6 && buf == KTestText().Right(6));
	test(file.Read(23, buf, 6) == KErrNone);
	test(buf.Length() == 3 && buf == KTestText().Right(3));
	test(file.Read(27, buf, 6) == KErrNone);
	test(buf.Length() == 0);

	file.Close();
	
	test(fs.Delete(KTestFile) == KErrNone);
	fs.Close();
	}

void testRFileWrite()
	{
	_LIT(KTestFile, "c:\\symport_test_rfile_write");
	_LIT8(KTestText, "abcdefghijklm");
	
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	TBuf8<26> buf;
	RFile file;
	test(file.Replace(fs, KTestFile, 0) == KErrNone);
	test(file.Read(buf) == KErrNone && buf.Length() == 0);

	// File position at the start
	TInt pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 0);

	test(file.Write(KTestText) == KErrNone); //13

	// Check position updated
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 13);

	test(file.Read(0, buf) == KErrNone && buf == KTestText);
	test(file.Write(KTestText) == KErrNone); //26
	test(file.Read(0, buf) == KErrNone && buf.Length() == 26 && buf.Left(13) == KTestText  && buf.Right(13) == KTestText);
	file.Close();

	// Test read after write
	test(file.Open(fs, KTestFile, EFileWrite) == KErrNone);
	test(file.Write(KTestText) == KErrNone);
	test(file.Read(buf) == KErrNone);
	test(buf.Length() == 13);
	file.Close();

	// Test write after read
	test(file.Open(fs, KTestFile, EFileWrite) == KErrNone);
	test(file.Read(buf) == KErrNone && buf.Length() == 26);
	test(file.Write(KTestText) == KErrNone);
	test(file.Read(26, buf) == KErrNone && buf.Length() == 13);
	file.Close();
	
	test(file.Replace(fs, KTestFile, 0) == KErrNone);
	test(file.Read(buf) == KErrNone && buf.Length() == 0);
	test(file.Write(KTestText, 6) == KErrNone); //6
	test(file.Read(0, buf) == KErrNone && buf == KTestText().Left(6));
	test(file.Write(KTestText, 6) == KErrNone); //12
	test(file.Read(0, buf) == KErrNone && buf.Length() == 12 && buf.Left(6) == KTestText().Left(6)  && buf.Right(6) == KTestText().Left(6));
	file.Close();
	
	// Test read after write
	test(file.Open(fs, KTestFile, EFileWrite) == KErrNone);
	test(file.Write(KTestText, 6) == KErrNone); //6
	test(file.Read(buf) == KErrNone && buf.Length() == 6);
	file.Close();

	// Test write after read
	test(file.Open(fs, KTestFile, EFileWrite) == KErrNone);
	test(file.Read(buf) == KErrNone && buf.Length() == 12);
	test(file.Write(KTestText, 6) == KErrNone);
	test(file.Read(12, buf) == KErrNone && buf.Length() == 6);
	file.Close();

	test(file.Replace(fs, KTestFile, 0) == KErrNone);
	test(file.Read(buf) == KErrNone && buf.Length() == 0);
	test(file.Write(3, KTestText) == KErrNone); //0+13=13
	test(file.Read(0, buf) == KErrNone);
	test(buf.Length() == 13);
	test(buf == KTestText);
	test(file.Write(7, KTestText) == KErrNone); //7+13=20
	test(file.Read(0, buf) == KErrNone && buf.Length() == 20 && buf.Mid(7,13) == KTestText());
	file.Close();

	// Test read after write
	test(file.Open(fs, KTestFile, EFileWrite) == KErrNone);
	test(file.Write(0, KTestText) == KErrNone); //13
	test(file.Read(buf) == KErrNone && buf.Length() == 7);
	file.Close();

	// Test write after read
	test(file.Open(fs, KTestFile, EFileWrite) == KErrNone);
	test(file.Read(buf) == KErrNone && buf.Length() == 20);
	test(file.Write(10, KTestText) == KErrNone); //10+13=23
	test(file.Read(0, buf) == KErrNone && buf.Length() == 23);
	file.Close();

	test(file.Replace(fs, KTestFile, 0) == KErrNone);
	test(file.Read(buf) == KErrNone && buf.Length() == 0);
	test(file.Write(3, KTestText, 10) == KErrNone); //0+10=10
	test(file.Read(0, buf) == KErrNone);
	test(buf.Length() == 10 && buf == KTestText().Left(10));
	test(file.Write(0, KTestText, 3) == KErrNone); //10
	test(file.Read(0, buf) == KErrNone );
	test(buf.Length() == 10);
	test(buf.Left(3) == KTestText().Left(3));
	file.Close();

	// Test read after write
	test(file.Open(fs, KTestFile, EFileWrite) == KErrNone);
	test(file.Write(0, KTestText, 3) == KErrNone); //3
	test(file.Read(buf) == KErrNone);
	test(buf.Length() == 7);
	file.Close();

	// Test write after read
	test(file.Open(fs, KTestFile, EFileWrite) == KErrNone);
	test(file.Read(buf) == KErrNone && buf.Length() == 10);
	test(file.Write(13, KTestText, 6) == KErrNone); //10+6=16
	test(file.Read(0, buf) == KErrNone && buf.Length() == 16);
	file.Close();
	
	test(fs.Delete(KTestFile) == KErrNone);
	fs.Close();
	}

void testRFileSeek()
	{
	_LIT(KTestFile, "c:\\symport_test_rfile_seek");
	_LIT8(KTestText, "abcdefghijklmnopqrstuvwxyz");
	
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	// Create the file
	RFile file;
	test(file.Replace(fs, KTestFile, EFileWrite) == KErrNone);
	test(file.Write(KTestText) == KErrNone);
	
	// ESeekAddress is invalid
	TInt pos = 0;
	test(file.Seek(ESeekAddress, pos) == KErrNotSupported);
	
	// Error on negative offset
	pos = -999;
	test(file.Seek(ESeekStart, pos) == KErrArgument);
	
	// Test you can get the file position
	pos = 12;
	test(file.Seek(ESeekStart, pos) == KErrNone);
	test(pos == 12);
	TBuf8<26> buf;
	test(file.Read(buf, 1) == KErrNone);
	test(buf == _L8("m")); //13

	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 13);
	
	// Move to the end of the file
	pos = 30;
	test(file.Seek(ESeekStart, pos) == KErrNone);
	test(pos == 30);
	test(file.Size(pos) == KErrNone);
	test(pos == 26);
	
	// Move back from the end
	pos = -1;
	test(file.Seek(ESeekEnd, pos) == KErrNone);
	test(pos == 25);
	test(file.Read(buf) == KErrNone);
	test(buf == _L8("z"));

	// Move past the end
	pos = 10;
	test(file.Seek(ESeekEnd, pos) == KErrNone);
	test(pos == 26);
	test(file.Read(buf) == KErrNone);
	test(buf.Length() == 0);
	
	// Move before the start
	pos = -30;
	test(file.Seek(ESeekEnd, pos) == KErrNone);
	test(pos == 0);
	test(file.Read(buf) == KErrNone);
	test(buf.Length() == 26);
	
	// Check seek to current
	pos = 13;
	test(file.Seek(ESeekStart, pos) == KErrNone);
	test(file.Read(buf, 1) == KErrNone); //14
	test(buf == _L8("n"));
	pos = -13;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 1);
	test(file.Read(buf, 1) == KErrNone); //2
	test(buf == _L8("b"));

	// Before the start
	pos = -10;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 0);
	test(file.Read(buf, 1) == KErrNone); //1
	test(buf == _L8("a"));

	// After the end
	pos = 26;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 26);
	test(file.Read(buf, 1) == KErrNone);
	test(buf.Length() == 0);
	
	// Check current pos
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 26);

	// And check you can go back
	pos = -1;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(file.Read(buf, 1) == KErrNone);
	test(buf == _L8("z"));
	
	file.Close();
	test(fs.Delete(KTestFile) == KErrNone);
	fs.Close();
	}

void testRFileFlush()
	{
	_LIT(KTestFile, "c:\\symport_test_rfile_flush");
	_LIT8(KTestText, "abcdefghijklmnopqrstuvwxyz");
	
	RFs fs;
	test(fs.Connect() == KErrNone);
	fs.Delete(KTestFile);
	
	// Create the file
	RFile file;
	test(file.Replace(fs, KTestFile, EFileWrite) == KErrNone);

	// Check file pos
	TInt pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 0);

	test(file.Write(KTestText) == KErrNone);
	
	// Check file pos
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 26);

	test(file.Flush() == KErrNone);

	// Check file pos hasn't changed
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 26);

	file.Close();
	
	test(fs.Delete(KTestFile) == KErrNone);
	fs.Close();
	}

void testRFileSize()
	{
	_LIT(KTestFile, "c:\\symport_test_rfile_flush");
	_LIT8(KTestText, "abcdefghijklmnopqrstuvwxyz");
	
	RFs fs;
	test(fs.Connect() == KErrNone);
	fs.Delete(KTestFile);
	
	// Create the file
	RFile file;
	test(file.Replace(fs, KTestFile, EFileWrite) == KErrNone);

	TInt size = 0;
	test(file.Size(size) == KErrNone);
	test(size == 0);
	
	// Check file pos
	TInt pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 0);

	test(file.Write(KTestText) == KErrNone);
	
	size = 0;
	test(file.Size(size) == KErrNone);
	test(size == 26);

	// Check file pos
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 26);

	file.Close();
	
	test(fs.Delete(KTestFile) == KErrNone);
	fs.Close();
	}

void testRFileSetSize()
	{
	_LIT(KTestFile, "c:\\symport_test_rfile_flush");
	_LIT8(KTestText, "abcdefghijklmnopqrstuvwxyz");
	
	RFs fs;
	test(fs.Connect() == KErrNone);
	fs.Delete(KTestFile);
	
	// Create the file
	RFile file;
	test(file.Replace(fs, KTestFile, EFileWrite) == KErrNone);
	
	// Check file pos
	TInt pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 0);

	test(file.Write(KTestText) == KErrNone); //26

	// Set the size to bigger
	test(file.SetSize(30) == KErrNone);

	// Check file pos is unchanged
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 26);

	// Set the size to smaller
	test(file.SetSize(20) == KErrNone);

	// Check file pos is at the end
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 20);

	file.Close();

	// Open for reading	
	test(file.Open(fs, KTestFile, 0) == KErrNone);

	// Shouldn't be allowed to set the size
	test(file.SetSize(10) == KErrAccessDenied);
	
	file.Close();
	
	test(fs.Delete(KTestFile) == KErrNone);
	fs.Close();
	}

void testRFileAtt()
	{
	// Connect to the file system
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	_LIT(KTestFile, "c:\\symport_test_rile_att.txt");	

	// See if it works for normal files
	TUint att;
	RFile file;
	test(file.Replace(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	
	// Check file pos is at the start
	TInt pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 0);

	test(file.Write(_L8("test")) == KErrNone);
	test(file.Att(att) == KErrNone);
	test(!(att&KEntryAttDir));

	// Check file pos is correct
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 4);

	// Check readonly flag
	test(!(att&KEntryAttReadOnly));
	test(file.SetAtt(KEntryAttReadOnly, 0) == KErrNone);
	test(file.Att(att) == KErrNone && att&KEntryAttReadOnly);
	
	// Clear the flag
	test(file.SetAtt(0, KEntryAttReadOnly) == KErrNone);
	test(file.Att(att) == KErrNone);
	test(!(att&KEntryAttReadOnly));
	file.Close();
	
	// Delete the file
	test(fs.Delete(KTestFile) == KErrNone);	
	fs.Close();
	}

void testRFileModified()
	{
	// Connect to the file system
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	_LIT(KTestFile, "c:\\symport_test_rile_modified.txt");	

	// See if it works for normal files
	RFile file;
	test(file.Replace(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	
	// Check file pos is at the start
	TInt pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 0);

	test(file.Write(_L8("test")) == KErrNone);
	test(file.Flush() == KErrNone);

	TTime then;
	test(file.Modified(then) == KErrNone);
	User::After(2000000);
	
	// Check file pos is correct
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 4);
	
	file.Close();
	
	test(file.Open(fs, KTestFile, EFileWrite) == KErrNone);
	test(file.Write(_L8("test")) == KErrNone);
	test(file.Flush() == KErrNone);
	
	TTime now;
	test(file.Modified(now) == KErrNone);

	TTimeIntervalSeconds sec;
	test(now.SecondsFrom(then, sec) == KErrNone);
	test(sec.Int() >= 1);
	
	file.Close();
	
	// Delete the file
	test(fs.Delete(KTestFile) == KErrNone);	
	fs.Close();
	}

void testRFileDrive()
	{
	// Connect to the file system
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	_LIT(KTestFile, "c:\\symport_test_rile_drive.txt");	

	// See if it works for normal files
	RFile file;
	test(file.Replace(fs, KTestFile, EFileShareAny|EFileWrite) == KErrNone);
	
	TInt drive;
	TDriveInfo info;
	
	test(file.Drive(drive, info) == KErrNone);
	test(drive == EDriveC);
	
#ifdef __TOOLS2__
	test(info.iType == EMediaHardDisk);
	test(info.iBattery == EBatNotSupported);
	test(info.iDriveAtt&(KDriveAttLocal|KDriveAttInternal));
	test(info.iMediaAtt == 0);
#endif

	file.Close();
	
	// Delete the file
	test(fs.Delete(KTestFile) == KErrNone);	
	file.Close();
	fs.Close();
	}

void testRFileRename()
	{
	// Connect to the file system
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	_LIT(KTestFile1, "c:\\symport_test_rile_rename_a.txt");	
	_LIT(KTestFile2, "c:\\symport_test_rile_rename_b.txt");	
	fs.Delete(KTestFile1);
	fs.Delete(KTestFile2);
	
	// See if it works for normal files
	RFile file;
	test(file.Replace(fs, KTestFile1, EFileShareExclusive|EFileWrite) == KErrNone);
	
	// Check file pos is correct
	TInt pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 0);
	test(file.Write(_L8("test")) == KErrNone);
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 4);

	test(file.Rename(KTestFile2) == KErrNone);
	
	// Check file pos is correct
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 4);
	test(file.Write(_L8("test")) == KErrNone);
	file.Close();

	test(file.Open(fs, KTestFile2, EFileShareExclusive) == KErrNone);

	// Check file pos is correct
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 0);
	TBuf8<16> buf;
	test(file.Read(buf) == KErrNone);
	test(buf.Length() == 8);
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 8);

	// Should fail because not opened for write
	test(file.Rename(KTestFile1) == KErrAccessDenied);

	// Check file pos is correct
	pos = 0;
	test(file.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 8);

	// Check you can't write to the file
	test(file.Write(_L8("test")) == KErrAccessDenied);
	file.Close();
	
	// Recreate the first file
	test(file.Replace(fs, KTestFile1, EFileShareExclusive|EFileWrite) == KErrNone);
	file.Close();
	
	// Check error if renaming to existing file
	test(file.Open(fs, KTestFile2, EFileShareExclusive) == KErrNone);	
	test(file.Rename(KTestFile1) == KErrAccessDenied);
	file.Close();
	
	// Cleanup
	test(fs.Delete(KTestFile1) == KErrNone);
	test(fs.Delete(KTestFile2) == KErrNone);
	fs.Close();
	}

void testRFileDuplicate()
	{
	// Connect to the file system
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	_LIT(KTestFile1, "c:\\symport_test_rile_duplicate.txt");	

	RFile file1;
	test(file1.Replace(fs, KTestFile1, EFileWrite) == KErrNone);
	test(file1.Write(_L8("Test")) == KErrNone);
	test(file1.Flush() == KErrNone);
	
	// Test you can duplicate the handle and read from it
	RFile file2;
	test(file2.Duplicate(file1) == KErrNone);
	
	// File position should match
	TInt pos = 0;
	test(file2.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 4);
	
	// Move back to the start
	pos = 0;
	test(file2.Seek(ESeekStart, pos) == KErrNone);
	test(pos == 0);
	
	// Check you can read from the file
	TBuf8<8> buf;
	test(file2.Read(buf) == KErrNone);
	test(buf.Length() == 4);

	// Check you can write to the file
	test(file2.Write(_L8("Test")) == KErrNone);

	file1.Close();
	file2.Close();
	
	// Now open readonly
	test(file1.Open(fs, KTestFile1, 0) == KErrNone);
	pos = 4;
	test(file1.Seek(ESeekStart, pos) == KErrNone);
	test(pos == 4);
	
	// Test you can duplicate the handle and read from it
	test(file2.Duplicate(file1) == KErrNone);
	
	// File position should match
	pos = 0;
	test(file2.Seek(ESeekCurrent, pos) == KErrNone);
	test(pos == 4);
	
	// Move back to the start
	pos = 0;
	test(file2.Seek(ESeekStart, pos) == KErrNone);
	test(pos == 0);
	
	// Check you can read from the file
	test(file2.Read(buf) == KErrNone);
	test(buf.Length() == 8);

	// Check you can't write to the file
	test(file2.Write(_L8("Test")) == KErrAccessDenied);

	file1.Close();
	file2.Close();
	
	test(fs.Delete(KTestFile1) == KErrNone);
	fs.Close();
	}

void testRFileName()
	{
	// Connect to the file system
	RFs fs;
	test(fs.Connect() == KErrNone);
	
	_LIT(KTestFile1, "C:\\symport_test_rile_filename.txt");	

	RFile file1;
	test(file1.Replace(fs, KTestFile1, 0) == KErrNone);

	TFileName name;
	test(file1.Name(name) == KErrNone);
	test(name == KTestFile1().Mid(3));
	test(file1.FullName(name) == KErrNone);
	test(name == KTestFile1);
	file1.Close();
	
	test(fs.Delete(KTestFile1) == KErrNone);
	fs.Close();
	}

void testRFs()
	{	
	test.Start(_L("Att"));
	testAtt();
	
	test.Next(_L("SetAtt"));
	testSetAtt();

	test.Next(_L("Modified"));
	testModified();

	test.Next(_L("MkDir"));
	testMkDir();

	test.Next(_L("MkDirAll"));
	testMkDirAll();
	
	test.Next(_L("RmDir"));
	testRmDir();
	
	test.Next(_L("Parse"));
	testParse();
	
	test.Next(_L("SessionPath"));
	testSessionPath();
	
	test.Next(_L("SetSessionPath"));
	testSetSessionPath();
	
	test.Next(_L("Delete"));
	testDelete();
	
	test.Next(_L("VolumeIOParam"));
#ifdef __TOOLS2__
	testVolumeIOParam();
#endif
	
	test.Next(_L("Entry"));
	testEntry();

	test.Next(_L("SetEntry"));
	testSetEntry();
	
	test.Next(_L("SetErrorCondition"));
#ifdef __TOOLS2__
	testSetErrorCondition();
#endif
	
	test.Next(_L("CharToDrive"));
	testCharToDrive();

	test.Next(_L("DriveToChar"));
	testDriveToChar();

	test.Next(_L("IsValidDrive"));
	testIsValidDrive();

	test.Next(_L("Drive"));
	testDrive();

	test.Next(_L("GetSystemDrive"));
	testGetSystemDrive();

	test.Next(_L("Replace"));
	testReplace();

	test.Next(_L("Rename"));
	testRename();

	test.End();
	}

void testTParse()
	{
	test.Start(_L("Test all TParse methods"));
	TestTParse1();

	test.Next(_L("Test multiple PopDirs"));
	TestTParse2();
	
	test.Next(_L("Test conflicting default drive letters"));
	TestTParse3();

	test.Next(_L("Test conflicting relative drive letters"));
	TestTParse4();
	
	test.Next(_L("Test errors returned by illegal paths"));
	TestTParse5();
	
	test.Next(_L("Test AddDir"));
	TestTParse6();
	
	test.Next(_L("Test TParsePtr"));
	TestTParse7();
	
	test.Next(_L("Test TParsePtrC"));
	TestTParse8();

	test.Next(_L("Test names with leading spaces"));
	TestTParse9();
	
	test.Next(_L("Test a Path > 256 chars"));
	TestTParse10();
	
	test.End();
	}

void testTFileText()
	{
	test.Start(_L("Test file text"));
	testTFileTextApi();
	
	test.Next(_L("Test FileText last record has no terminator"));
	testTFileTextEndRecord();

	test.End();
	}

void testRFile()
	{
	test.Start(_L("Open"));
	testRFileOpen();
	
	test.Next(_L("Create"));
	testRFileCreate();
	
	test.Next(_L("Replace"));
	testRFileReplace();
	
	test.Next(_L("Temp"));
	testRFileTemp();
	
	test.Next(_L("Read"));	
	testRFileRead();
	
	test.Next(_L("Write"));
	testRFileWrite();
	
	test.Next(_L("Seek"));
	testRFileSeek();
	
	test.Next(_L("Flush"));
	testRFileFlush();
	
	test.Next(_L("Size"));
	testRFileSize();
	
	test.Next(_L("SetSize"));
	testRFileSetSize();
	
	test.Next(_L("Att"));
	testRFileAtt();
	
	test.Next(_L("Modified"));
	testRFileModified();
	
	test.Next(_L("Drive"));
	testRFileDrive();
	
	test.Next(_L("Rename"));
	testRFileRename();
	
	test.Next(_L("Duplicate"));
	testRFileDuplicate();
	
	test.Next(_L("Name"));
	testRFileName();
	
	test.End();
	}

// This following isn't relevant to WINSCW
#ifdef __TOOLS2__
void testSpecialFilenameProcessing()
	{
	// Connect to the file system
	RFs fs;
	test(fs.Connect() == KErrNone);

	fs.RmDir(_L("c:\\testMisc\\epoc32\\test1\\"));
	fs.RmDir(_L("c:\\testMisc\\epoc32\\test2\\"));
	fs.RmDir(_L("c:\\testMisc\\epoc32\\"));
	fs.RmDir(_L("c:\\testMisc\\"));

	TUint att;
	_LIT(KCDriveTest1, "c:\\testMisc\\epoc32\\test1\\");
	test(fs.MkDirAll(KCDriveTest1) == KErrNone);
	test(fs.Att(KCDriveTest1, att) == KErrNone);
	
	struct stat sb;
#ifdef _WIN32
	test(stat("c:\\testMisc\\epoc32\\test1", &sb) == 0);
#else
	// Get home
	char* env = getenv("HOME");
	test(env != NULL);
	char home[0x100];
	strcpy(home, env);
	
	// Check it's replaced c: with HOME
	char testpath[0x100];
	strcpy(testpath, home);
	strcat(testpath, "/testmisc/epoc32/test1");
	test(stat(testpath, &sb) == 0);
#endif
		
#ifdef _WIN32
	static char *env = "EPOCROOT=\\";
	test(putenv(env) == 0);
#else
	test(setenv("EPOCROOT", home, 1) == 0);
#endif
	
	_LIT(KEpocRootTest, "\\epoc32\\winscw\\c\\testMisc\\test1\\");
	test(fs.MkDirAll(KEpocRootTest) == KErrNone);
	test(fs.Att(KEpocRootTest, att) == KErrNone);

#ifdef _WIN32
	test(stat("\\epoc32\\winscw\\c\\testMisc\\test1", &sb) == 0);
#else
	// Check it's replaced \epoc32 with $(EPOCROOT)epoc32
	strcpy(testpath, home);
	strcat(testpath, "/epoc32/winscw/c/testmisc/test1");
	test(stat(testpath, &sb) == 0);
#endif

	test(fs.RmDir(KEpocRootTest) == KErrNone);
	test(fs.RmDir(_L("\\epoc32\\winscw\\c\\testMisc\\")) == KErrNone);
	
	// Now set EPOCROOT to something a bit more complicated
#ifdef _WIN32
	env = "EPOCROOT=c:\\testMisc";
	test(putenv(env) == 0);
#else
	strcpy(testpath, home);
	strcat(testpath, "/testmisc/");
	test(setenv("EPOCROOT", testpath, 1) == 0);
#endif

	_LIT(KCDriveTest2, "\\epoc32\\test2\\");
	test(fs.MkDirAll(KCDriveTest2) == KErrNone);
	test(fs.Att(_L("\\epoc32\\test1\\"),att) == KErrNone);
	test(fs.Att(_L("\\epoc32\\test2\\"),att) == KErrNone);

#ifdef _WIN32
	test(stat("c:\\testMisc\\epoc32\\test2", &sb) == 0);
#else
	strcpy(testpath, home);
	strcat(testpath, "/testmisc/epoc32/test2");
	test(stat(testpath, &sb) == 0);
#endif

#ifdef _WIN32
	env = "EPOCROOT=\\";
	test(putenv(env) == 0);
#else
	test(setenv("EPOCROOT", home, 1) == 0);
#endif
	
	test(fs.RmDir(_L("c:\\testMisc\\epoc32\\test1\\")) == KErrNone);
	test(fs.RmDir(_L("c:\\testMisc\\epoc32\\test2\\")) == KErrNone);
	test(fs.RmDir(_L("c:\\testMisc\\epoc32\\")) == KErrNone);
	test(fs.RmDir(_L("c:\\testMisc\\")) == KErrNone);
	
	fs.Close();
	}

#ifdef USING_SYMPORT_DLL
IMPORT_C TVersion SymPortVersion();
void testVersion()
	{
	TVersion ver(SymPortVersion());
	test(ver.iMajor >= 1);

	TPtrC name(ver.Name());
	test.Printf(_L("Symport Version: %S\n"), &name);
	}
#endif
#endif

void doMainL()
	{
	test.Start(_L("RFs"));
	testRFs();
	
	test.Next(_L("TParse"));
	testTParse();

	test.Next(_L("TFileText"));
	testTFileText();
	
	test.Next(_L("RFile"));
	testRFile();

	test.Next(_L("Misc"));
#ifdef __TOOLS2__
	testSpecialFilenameProcessing();
#ifdef USING_SYMPORT_DLL
	testVersion();
#endif
#endif

	test.End();	
	}
int E32Main()
	{
	__UHEAP_MARK;
	CTrapCleanup* theCleanup = CTrapCleanup::New();
	TRAPD(ret, doMainL());
	test(ret == KErrNone);
	test.Close();
	delete theCleanup;
	__UHEAP_MARKEND;
	return ret;
	}
