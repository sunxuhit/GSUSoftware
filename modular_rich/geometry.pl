#!/usr/bin/perl -w

use strict;
use lib ("$ENV{GEMC}/io");
use parameters;
use utils;

use geometry;
use math;

# Help Message
sub help()
{
	print "\n Usage: \n";
	print "   geometry.pl <configuration filename>\n";
 	print "   Will create the EIC Ring Imaging Cherenkov Detector (RICH) geometry\n";
 	print "   Note: The passport and .visa files must be present to connect to MYSQL. \n\n";
	exit;
}

# Make sure the argument list is correct
if( scalar @ARGV != 1) 
{
	help();
	exit;
}

# Loading configuration file and paramters
my $config_file   = $ARGV[0];
our %configuration = load_configuration($config_file);

# One can change the "variation" here if one is desired different from the config.dat
# $configuration{"variation"} = "myvar";


# Loading EIC RICH geometry routines specific subroutins
require "./geo/detector.pl";

build_detector();
