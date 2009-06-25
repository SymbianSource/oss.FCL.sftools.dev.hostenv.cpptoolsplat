#!/usr/bin/perl

# Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of the License "Symbian Foundation License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
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

# The following are the tests to run
BEGIN { plan tests => 6 }

# Make sure we're in the correct folder
chdir('../symfile') or die "Failed to set folder: $!";

# Version of the script - just use the date
$main::VERSION = '02-Oct-08';

# Get command line arguments
print "\n";
my ( $verbose, $skip );
GetOptions("verbose" => \$verbose, "skip" => \$skip) or pod2usage(2);

# Get OS version
my $win32 = 1 if $^O =~ /MSWin32/;
print "Running on Win32\n" if $win32 && $verbose;
print "Running on Linux\n" if !$win32 && $verbose;

# Build the code
doBuild() if (!$skip);

# Run the tests
doTest('deb', 't_symfile');
doTest('rel', 't_symfile');
doTest('deb', 't_symfile_dll');
doTest('rel', 't_symfile_dll');

# Run tests in emulator to check compatibility
if ($win32)
	{
	doEmuTest(catfile($ENV{EPOCROOT}, 'epoc32', 'release', 'winscw', 'udeb', 't_symfile.exe'));
	doEmuTest(catfile($ENV{EPOCROOT}, 'epoc32', 'release', 'winscw', 'urel', 't_symfile.exe'));
	}
else
	{
	# No emulator on Linux
	ok(1);
	ok(1);
	}

# ***
# Runs test code
#
sub doTest
	{
	my ( $variant, $name ) = @_;

	my $test;
	if ($win32)
		{
		$test = catfile($ENV{EPOCROOT}, 'epoc32', 'release', 'tools2', $variant, "$name.exe");
		}
	else
		{
		# Have to set library path for Linux
		$ENV{LD_LIBRARY_PATH} =~ s[/epoc32[^:]+:/][];
		$ENV{LD_LIBRARY_PATH} = "$ENV{EPOCROOT}epoc32/release/tools2/linux-i386/$variant:$ENV{LD_LIBRARY_PATH}";
		$test = catfile($ENV{EPOCROOT}, 'epoc32', 'release', 'tools2', 'linux-i386', $variant, $name);
		}

	die "Can't find test exe: $test" if !-e $test;
	print "Running test: $test\n" if $verbose;

	my $pass;
	open TEST, "$test|" or die "Failed to start test $test: $!";
	while(my $line = <TEST>)
		{
		next unless $line =~ /RTEST: /;
		print "\t$line" if $verbose;
		$pass = 1 if $line =~ /^RTEST: SUCCESS/ or $line =~ /TEST Successfully Completed/;
		}

	close TEST;
	print "FAIL: $test\n" if (!$pass);
	ok($pass);
	}

# ***
# Runs emulator test code
#
sub doEmuTest
	{
	my $test = shift;
	die "Can't find test exe: $test" if !-e $test;
	print "Running emulator test: $test\n" if $verbose;
	my $vis = $verbose ? 'textshell' : 'NoGui';

	# Make sure the log file doesn't exist
	my $logfile = catfile($ENV{TEMP}, 'epocwind.out');
	unlink $logfile;

	# Start the emulator	
	die "Failed to Launch emulator for $test: $!" if system("$test -D$vis --") != 0;

	# Check the log file
	my $pass;
	open LOG, $logfile, or die "Failed open log file $logfile: $!";
	while(my $line = <LOG>)
		{
		next unless $line =~ /RTEST: /;

		$line =~ s/^.*\t//;
		chomp $line;

		# Next line contains the test description
		my $nextline = <LOG>;
		$nextline =~ s/^.*\t//;
		$line .= $nextline;

		print "\t$line" if $verbose;
		$pass = 1 if $line =~ /^RTEST: SUCCESS/ or $line =~ /TEST Successfully Completed/;
		}
	close LOG;
	print "FAIL: $test\n" if (!$pass);
	ok($pass);
	}

# ***
# Builds the test code
# *
sub doBuild
	{
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
	
	# Override the result using environment variable
	$sbs_ver = 1 if $ENV{SBS_VERSION} && $ENV{SBS_VERSION} == 1;
	my $redir = $verbose?'':' >nul 2>&1';

	# Now build the test code - assumes current working directory is correct!
	print "Building test code using SBSv$sbs_ver\n";
	if ($sbs_ver == 1)
		{
		# Use the old build system
		system("bldmake bldfiles$redir");
		system("abld test makefile$redir");
		system("abld test clean$redir");
		system("abld test build$redir");
		}
	else
		{
		# Use the new build system
		system("sbs -c tools2.test CLEAN$redir");
		system("sbs -c tools2.test$redir");
		system("sbs -c winscw.test CLEAN$redir");
		system("sbs -c winscw.test$redir");
		}
	}

=head1 NAME

test.pl - A script for running tests

=head1 SYNOPSIS

test.pl [-help] [-version] [-verbose] [-skip]

 Options:
   -help      brief help message
   -version   version of the script
   -verbose   print what the scripts does
   -skip      skip building the code

=cut
