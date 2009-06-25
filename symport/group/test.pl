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
use Test::Harness;
use Getopt::Long qw(:config auto_version auto_help);
use File::Spec::Functions;
use Cwd;

$Test::Harness::Verbose = 1;

# Version of the script - just use the date
$main::VERSION = '17-Oct-08';

# a list of all the tests.
my @all = ( 
'../bldtest/test.pl',
'../symuser/test.pl',
'../symfile/test.pl'
);

my ( $verbose, $ignore, $coverage );
GetOptions("verbose" => \$verbose, "ignore" => \$ignore, "coverage" => \$coverage) or pod2usage(2);

# Get OS version
my $win32 = 1 if $^O =~ /MSWin32/;
print "Running on Win32\n" if $win32 && $verbose;
print "Running on Linux\n" if !$win32 && $verbose;

# Clear coverage stats and rebuild the code with coverage enabled
if ($coverage)
	{
	clearCoverageStats();
	setCoverage(1);
	}

# Run all the tests
my $fail;
eval { runtests(@all); };
$fail = $@ if $@;

# Show coverage stats and rebuild the code with coverage disabled
if ($coverage)
	{
	showCoverageStats();
	setCoverage(0);
	clearCoverageStats();
	}
die $fail if $fail;

# Show coverage stats
sub showCoverageStats
	{
	my %hits;

	my $loc = cwd();
	print "Calculating converage stats...\n";

	# Find all the GCDA files
	my @dirs = "$ENV{EPOCROOT}epoc32/build";
	foreach my $dir ( @dirs )
		{
		opendir DIR, $dir or die "Failed on $dir: $!";
		foreach my $child ( readdir DIR )
			{
			next if $child =~ /^\./;
			
		 	push @{ $hits{$dir} }, "$child" if $child =~ /\.gcda$/;
			push @dirs, "$dir/$child" if -d "$dir/$child";
			}
		closedir DIR;
		}

	# Collect the stats
	my %result;
	foreach my $dir ( keys %hits )
		{
		chdir($dir) or die "Failed: $!";
		my $cmd = $win32 ? "$ENV{EPOCROOT}epoc32/gcc_mingw/bin/gcov.exe @{ $hits{$dir} }" : "gcov @{ $hits{$dir} }";
		print "Executing: $cmd\n";
		open CMD, "$cmd|" or die "Failed: $!";
		while(my $line = <CMD>)
			{
			if ($line =~ /Lines executed:(\d+.\d+)% of (\d+)/)
				{
				my ($total, $coverage) = ( $2, int(($2 / 100) * $1));
				my $oldline = $line;
				$line = <CMD>;

				# Filter by filename symport/<component>
				if ($line =~ m[symport/([^/]+)/])
					{
					print "\t$oldline";
					my $comp = $1;
	
					$result{$comp}{total} = $result{$comp}{total} ? $result{$comp}{total} + $total : $total;
					$result{$comp}{coverage} = $result{$comp}{coverage} ? $result{$comp}{coverage} + $coverage : $coverage;
					}
				else
					{
					print STDERR "Unknown: Line $. - $line" if $verbose;
					}
				}
			}
		close CMD;
		}

	# Print a summary of the stats
	print "\nTEST COVERAGE RESULTS\n";
	foreach ( keys %result )
		{
		my $percent = ($result{$_}{coverage}/$result{$_}{total})*100;
		print "$_: $result{$_}{coverage}/$result{$_}{total} (${percent}%)\n";
		}
	chdir($loc);
	}

# Make sure all GCDA files are deleted
sub clearCoverageStats
	{
	print "Clearing converage stats...\n" if $verbose;

	my @dirs = "$ENV{EPOCROOT}epoc32/build";
	foreach my $dir ( @dirs )
		{
		opendir DIR, $dir or die "Failed on $dir: $!";
		foreach my $child ( readdir DIR )
			{
			next if $child =~ /^\./;
			
			if ($child =~ /\.gcda$/)
				{
				print "Deleting: $dir/$child\n" if $verbose;
			 	unlink "$dir/$child";
				}
			push @dirs, "$dir/$child" if -d "$dir/$child";
			}
		closedir DIR;
		}
	}

# Enables/Disables code coverage
sub setCoverage
	{
	my $enable = shift;
	
	if ($verbose) { print $enable ? "Enabling code coverage...\n" : "Disabling code coverage...\n"; }
	my $makefile = catfile($ENV{EPOCROOT}, 'epoc32', 'tools', 'makefile_templates', 'tools', 'x86tool.mk');
	die "Can't find makefile $makefile" if !-e $makefile;
	
	# Read the makefile
	my @lines;
	open IN, $makefile or die "Failed to open $makefile: $!";
	while(<IN>)
		{
		if (/CODE_COVERAGE:=/)
			{
			push @lines, $enable ? "CODE_COVERAGE:=1\n" : "CODE_COVERAGE:=\n";
			next;
			}
		push @lines, $_;
		}
	close IN;

	# Write out the makefile with the change
	open OUT, ">$makefile" or die "Failed to open $makefile for writing: $!";
	print OUT @lines;
	close OUT;

	# Now do the same for the new build system
	my $xml = catfile($ENV{SBS_HOME}, 'lib', 'flm', 'x86tool.xml');
	die "Can't find xml $xml" if !-e $xml;
	
	# Read the makefile
	@lines = ();
	open IN, $xml or die "Failed to open $xml: $!";
	while(<IN>)
		{
		if (/(\s*<param name=\"CODE_COVERAGE\" default=\")\d?(\" \/>)/)
			{
			push @lines, sprintf("$1%s$2\n", $enable ? '1' : '');
			next;
			}
		push @lines, $_;
		}
	close IN;

	# Write out the makefile with the change
	open OUT, ">$xml" or die "Failed to open $xml for writing: $!";
	print OUT @lines;
	close OUT;

	doClean();
	doBuild();
	}

# ***
# Get the version of the build system
# *
sub getBuildVersion
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
	return $sbs_ver;
	}

# ***
# Cleans the code
# *
sub doClean
	{
	print "Cleaning code...\n" if $verbose;
	my $sbs_ver = getBuildVersion();
	my $redir = $verbose?'':' >nul 2>&1';

	if ($sbs_ver == 1)
		{
		# Use the old build system
		system("bldmake bldfiles$redir");
		system("abld makefile$redir");
		system("abld clean$redir");
		}
	else
		{
		# Use the new build system
		system("sbs -c tools2 CLEAN$redir");
		system("sbs -c winscw CLEAN$redir");
		}
	}

# ***
# Builds the code
# *
sub doBuild
	{
	print "Building code...\n" if $verbose;
	my $sbs_ver = getBuildVersion();
	my $redir = $verbose?'':' >nul 2>&1';

	# Now build the test code - assumes current working directory is correct!
	print "Using SBSv$sbs_ver\n" if $verbose;
	if ($sbs_ver == 1)
		{

		# Use the old build system
		system("bldmake bldfiles$redir");
		system("abld build$redir");
		}
	else
		{
		# Use the new build system
		system("sbs -c tools2$redir");
		system("sbs -c winscw$redir");
		}
	}

=head1 NAME

test.pl - A script for running other tests

=head1 SYNOPSIS

test.pl [-help] [-version] [-verbose] [-coverage]

 Options:
   -help      brief help message
   -version   version of the script
   -verbose   print what the scripts does
   -coverage  gather code coverage stats (rebuilds everything twice)

=cut
