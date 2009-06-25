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
use File::Copy;

# The following are the tests to run
BEGIN { plan tests => 60 }
my @tests = qw/t_bflat t_bseg t_buf t_rbuf t_char t_circ t_collate t_des t_farray t_func t_graph t_key t_lex t_match t_parray t_que t_readar t_regn t_sque t_varray t_versio t_hashtab t_huff t_float t_i64 t_i64_2 t_math t_math2 t_trap t_ctrap/;

# Make sure we're in the correct folder
chdir('../symuser') or die "Failed to set folder: $!";

# Version of the script - just use the date
$main::VERSION = '13-Oct-08';

# Get command line arguments
print "\n";
my ( $verbose, $skip );
GetOptions("verbose" => \$verbose, "skip" => \$skip) or pod2usage(2);

my $win32 = 1 if $^O =~ /MSWin32/;
print "Running on Win32\n" if $win32 && $verbose;
print "Running on Linux\n" if !$win32 && $verbose;

# Some files have to be copied to an accessible location
unlink 'UnicodeData.txt';
copy(catfile('..', 'e32test', 'buffer', 'UnicodeData.txt'), 'unicodedata.txt') or die "Failed to copy file: $!";

# Build the code and run the tests
doBuild() if (!$skip);
foreach my $test ( @tests )
	{
	doTest('deb', $test);
	doTest('rel', $test);
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
	while(<TEST>)
		{
		print "\t$_" if $verbose;
		$pass = 1 if /^RTEST: SUCCESS/ or /TEST Successfully Completed/;
		}

	print "FAIL: $test\n" if (!$pass);
	ok($pass);	
	close TEST;
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
	my $nul = $win32 ? 'nul' : '/dev/null';
	my $redir = $verbose? '' : " >$nul 2>&1";

	# Now build the test code - assumes current working directory is correct!
	print "Building test code using SBSv$sbs_ver\n" if $verbose;
	if ($sbs_ver == 1)
		{
		print "Building test code using SBSv1\n";

		# Use the old build system
		system("bldmake bldfiles$redir");
		system("abld test makefile$redir");
		system("abld test clean$redir");
		system("abld test build$redir");
		}
	else
		{
		print "Building test code using SBSv2\n";

		# Use the new build system
		system("sbs -c tools2.test CLEAN$redir");
		system("sbs -c tools2.test$redir");
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
