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
use Getopt::Long;
use File::Spec::Functions;
use Pod::Usage;
use File::Copy;

# Version of the script - just use the date
$main::VERSION = '13-Oct-08';

# Get command line arguments
my ( $version, $help, $verbose );
GetOptions("verbose|v" => \$verbose, "version|ver" => \$version, "help|h" => \$help) or pod2usage(2);

# Handle help and version
pod2usage({ verbose => 1, exitval => 0}) if $help;
version() if $version;

# Check we can find the build system
die "SBS_HOME environment variable not defined" if !$ENV{SBS_HOME};
die "Can't find SBS in $ENV{SBS_HOME}" if !-d $ENV{SBS_HOME};

# Now do the work
doCopy('x86tool.flm', catfile($ENV{SBS_HOME}, 'lib', 'flm', 'tools', 'x86tool.flm'));
doCopy('x86tool.xml', catfile($ENV{SBS_HOME}, 'lib', 'flm', 'x86tool.xml'));

# Copy files
sub doCopy
	{
	my ( $src, $dst ) = @_;
	print "Copying $src to $dst\n" if $verbose;
	unlink $dst;
	copy($src, $dst) or die "Failed to copy $src to $dst: $!";
	}

# New getopt::long provides this - but old version doesn't?
sub version
	{
	print sprintf("$0: $main::VERSION\n$^X: %vd\nos: $^O", $^V);
	exit;
	}

=head1 NAME

export.pl - Copies FLM files to the right place

=head1 SYNOPSIS

export.pl [-help] [-version] [-verbose]

 Options:
   -help      brief help message
   -version   version of the script
   -verbose   print what the scripts does

=cut
