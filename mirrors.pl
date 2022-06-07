#!/usr/bin/perl -w

use strict;
use lib ("$ENV{GEMC}/io");
use utils;
use mirrors;

# Help Message
sub help()
{
	print "\n Usage: \n";
	print "   mirrors.pl <configuration filename>\n";
 	print "   Will create a simple mirror\n";
	exit;
}

# Make sure the argument list is correct
# If not pring the help
if( scalar @ARGV != 1)
{
	help();
	exit;
}

# Loading configuration file from argument
our %configuration = load_configuration($ARGV[0]);

# One can change the "variation" here if one is desired different from the config.dat
# $configuration{"variation"} = "myvar";


# Photon energy bins
my @PhotonEnergyBin = ( "1.7712*eV", "6.1992*eV" );
my @Reflectivity = ( 0.95, 0.95 );


sub define_mirror
{
	# the first argument to this function become the variation
	$configuration{"variation"} = shift;

	# Mirror Optical Properties
	my %mir = init_mir();
	$mir{"name"}         = "rich_mirrors";
	$mir{"description"}  = "reflective mirrors for eic rich";
	$mir{"type"}         = "dielectric_dielectric";
	$mir{"finish"}       = "polishedfrontpainted";
	$mir{"model"}        = "unified";
	$mir{"border"} 	     = "SkinSurface";
	$mir{"photonEnergy"} = arrayToString(@PhotonEnergyBin);
	$mir{"reflectivity"} = arrayToString(@Reflectivity);
	print_mir(\%configuration, \%mir);
}

define_mirror("Original");
