#!/usr/bin/perl

# Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of the License "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:
#

use strict;
use warnings;
use Test;
use Getopt::Long qw(:config auto_version auto_help);
use File::Spec::Functions;
use File::Basename;
use Cwd;

# The following are the tests to run
BEGIN { plan tests => 96 }

# Make sure we're in the correct folder
chdir('../bldtest') or die "Failed to set folder: $!";

# Version of the script - just use the date
$main::VERSION = '09-Oct-08';

# Get command line arguments
print "\n";
my ( $verbose, $ignore );
GetOptions("verbose" => \$verbose, "ignore" => \$ignore) or pod2usage(2);

my $win32 = 1 if $^O =~ /MSWin32/;
print "Running on Win32\n" if $win32 && $verbose;
print "Running on Linux\n" if !$win32 && $verbose;

# Test new build system
my $flm = catfile($ENV{SBS_HOME}, 'lib', 'flm', 'x86tool.xml');
my $tem = catfile($ENV{EPOCROOT}, 'epoc32', 'tools', 'makefile_templates', 'tools', 'x86tool.mk');

warn "FLM $flm doesn't exist?" if !-e $flm;
print "Testing SBSv2 with FLM\n";
rename $tem, "$tem.disabled";
$ENV{SBS_VERSION} = 2 if $win32;
doCheck('deb');
doCheck('rel');
rename "$tem.disabled", $tem;

# Disable the FLM if it exists and run test with the TEM
print "Testing SBSv2 with TEM\n";
rename $flm, "$flm.disabled";
doCheck('deb');
doCheck('rel');
rename "$flm.disabled", $flm;

# Test old build system
print "Testing SBSv1 with TEM\n";
if ($win32)
	{
	$ENV{SBS_VERSION} = 1;
	doCheck('deb');
	doCheck('rel');
	}
else
	{
	# No such thing on Linux - just keep Test happy
	for(my $i = 1; $i <= 32; $i++)
		{
		ok(1);
		}
	}

# ***
# Check everything works
#
sub doCheck
	{
	my $variant = shift;

	# Have to set library path for Linux
	if (!$win32)
		{
		$ENV{LD_LIBRARY_PATH} =~ s[/epoc32[^:]+:/][];
		$ENV{LD_LIBRARY_PATH} = "$ENV{EPOCROOT}epoc32/release/tools2/linux-i386/$variant:$ENV{LD_LIBRARY_PATH}";
		}

	# Build the code
	my $sbs_ver = doBuild($variant);

	# File extensions differ on Windows and Linux
	my ( $ext_exe, $ext_dll ) = $win32 ? ( '.exe', '.dll' ) : ( '', '.so' );

	# Generate string for the location of built file in epoc tree
	my $release = catfile($ENV{EPOCROOT}, 'epoc32', 'release', 'tools2');
	$release = catfile($release, 'linux-i386') if !$win32;
	$release = catfile($release, '%s', '%s%s');

	# The simple test should pass
	doTest(sprintf($release, $variant, 'testexe', $ext_exe));
	doTest(sprintf($release, $variant, 'testexe_main.test', ''));

	# Check that the exe and lib have been built
	checkExist(sprintf($release, $variant, 'testexe', $ext_exe));
	checkExist(sprintf($release, $variant, 'testexe_main.test', ''));
	checkExist(sprintf($release, $variant, 'libteststatic', '.a'));
	checkExist(sprintf($release, $variant, 'libtestdll', $ext_dll));

	# Check all the files exist
	if ($sbs_ver == 1)
		{
		my $build = catfile($ENV{EPOCROOT}, 'epoc32', 'build', cwd(), 'wrappermakefiles', '%s', cwd(), '%s');
		$build =~ s/\w:\\//g;

		# Check the object files exist
		checkExist(sprintf($build, $variant, 'testexe.cpp.o'));
		checkExist(sprintf($build, $variant, 'teststatic.cpp.o'));
		checkExist(sprintf($build, $variant, 'testdll.cpp.o'));

		# Clean should delete everything
		doClean($sbs_ver, $variant);
	
		# Object files should be gone now
		checkMissing(sprintf($build, $variant, 'testexe.cpp.o'));
		checkMissing(sprintf($build, $variant, 'teststatic.cpp.o'));
		checkMissing(sprintf($build, $variant, 'testdll.cpp.o'));
		}
	else
		{
		my $build;

		# Build location differs between the TEM and FLM
		my $flm = catfile($ENV{SBS_HOME}, 'lib', 'flm', 'x86tool.xml');
		if (-e $flm)
			{
			my $cwd = cwd();
			$cwd =~ s/$ENV{HOME}// if $ENV{HOME};
			$build = catfile($ENV{EPOCROOT}, 'epoc32', 'build', 'x86tools', '%s', '%s', $cwd, '%s');
			$build =~ s/\w:\\//g;

			# Check the object files exist
			checkExist(sprintf($build, $variant, 'testexe', 'testexe.cpp.o'));
			checkExist(sprintf($build, $variant, 'teststatic', 'teststatic.cpp.o'));
			checkExist(sprintf($build, $variant, 'testdll', 'testdll.cpp.o'));
	
			# Clean should delete everything
			doClean($sbs_ver, $variant);
	
			# Object files should be gone now
			checkMissing(sprintf($build, $variant, 'testexe', 'testexe.cpp.o'));
			checkMissing(sprintf($build, $variant, 'teststatic', 'teststatic.cpp.o'));
			checkMissing(sprintf($build, $variant, 'testdll', 'testdll.cpp.o'));
			}
		else
			{
			$build = catfile($ENV{EPOCROOT}, 'epoc32', 'build', '%s', cwd(), '%s');
			$build =~ s/\w:\\//g;

			# Check the object files exist
			checkExist(sprintf($build, $variant, 'testexe.cpp.o'));
			checkExist(sprintf($build, $variant, 'teststatic.cpp.o'));
			checkExist(sprintf($build, $variant, 'testdll.cpp.o'));
	
			# Clean should delete everything
			doClean($sbs_ver, $variant);
	
			# Object files should be gone now
			checkMissing(sprintf($build, $variant, 'testexe.cpp.o'));
			checkMissing(sprintf($build, $variant, 'teststatic.cpp.o'));
			checkMissing(sprintf($build, $variant, 'testdll.cpp.o'));
			}
		}

	# Releaseables should be gone
	checkMissing(sprintf($release, $variant, 'testexe', $ext_exe));
	checkMissing(sprintf($release, $variant, 'testexe_main.test', ''));
	checkMissing(sprintf($release, $variant, 'libteststatic', '.a'));
	checkMissing(sprintf($release, $variant, 'libtestdll', $ext_dll));
	}

sub checkExist
	{
	my $file = shift;
	print "Checking file exists: $file\n" if $verbose;
	ok(-e $file);
	}

sub checkMissing
	{
	my $file = shift;
	print "Checking file missing: $file\n" if $verbose;
	ok(!-e $file);
	}

# ***
# Runs test code
#
sub doTest
	{
	my $test = shift;
	warn "Can't find test exe: $test" if (!-e $test);

	my $pass;
	if (-e $test)
		{
		print "Running test: $test\n" if $verbose;
		open TEST, "$test|" or die "Failed to start test $test: $!";
		while(<TEST>)
			{
			$pass = 1 if /^RTEST: SUCCESS/ or /TEST Successfully Completed/;
			}
		close TEST;
		}
	ok($pass);	
	}

# ***
# Cleans the test code
# *
sub doClean
	{
	my $sbs_ver = shift;
	my $variant = shift;

	my $redir = $verbose?'':' >nul 2>&1';
	print "Cleaning component for $variant\n" if $verbose;
	if ($sbs_ver == 1)
		{
		# Use the old build system
		system("bldmake bldfiles$redir");
		system("abld test makefile tools2$redir");
		system("abld test clean tools2 $variant$redir");
		}
	else
		{
		# Use the new build system
		system("sbs -c tools2_$variant.test CLEAN$redir");
		}
	}

# ***
# Builds the test code
# *
sub doBuild
	{
	my $variant = shift;

	# First of all see if SBSv2 is installed
	my $sbs_ver = 1;
	open SBS, 'sbs -v 2>&1|' or die "Failed to execute command: $!";
	while(<SBS>)
		{
		if (/^sbs version/)
			{
			$sbs_ver = 2;
			last;
			}
		}
	close SBS;
	die "This test requires SBSv2. Run with -ignore to run this test anyway" if $sbs_ver == 1 && !$ignore;

	# Override the result using environment variable
	$sbs_ver = 1 if $ENV{SBS_VERSION} && $ENV{SBS_VERSION} == 1;

	# Now build the test code - assumes current working directory is correct!
	my $redir = $verbose?'':' >nul 2>&1';
	print "Building test code using SBSv$sbs_ver for $variant\n" if $verbose;
	if ($sbs_ver == 1)
		{
		print "Building using sbsv1\n" if $verbose;

		# Use the old build system
		system("bldmake bldfiles$redir");
		system("abld test makefile tools2$redir");
		system("abld test clean tools2$redir");
		system("abld test library tools2$redir");
		system("abld test target tools2 $variant$redir");
		}
	else
		{
		print "Building using sbsv2\n" if $verbose;

		# TEM can't handle parallel builds due to dependencies between extension makefiles
		my $flm = catfile($ENV{SBS_HOME}, 'lib', 'flm', 'x86tool.xml');
		my $jopt = (!-e $flm) ? '-j 1' : '';

		# Use the new build system
		system("sbs $jopt -c tools2_$variant.test CLEAN$redir");
		system("sbs $jopt -c tools2_$variant.test$redir");
		}

	# Return the sbs version for later
	return $sbs_ver;
	}

=head1 NAME

test.pl - A script for running tests

=head1 SYNOPSIS

test.pl [-help] [-version] [-verbose] [-ignore]

 Options:
   -help      brief help message
   -version   version of the script
   -verbose   print what the scripts does
   -ignore    ignore lack of SBSv2 and run tests anyway
=cut
