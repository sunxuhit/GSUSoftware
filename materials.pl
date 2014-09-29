#!/usr/bin/perl -w

use strict;
use lib ("$ENV{GEMC}/io");
use utils;
use materials;

# Help Message
sub help()
{
	print "\n Usage: \n";
	print "   materials.pl <configuration filename>\n";
 	print "   Will create a simple scintillator material\n";
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
my @PhotonEnergyBin = ( "2.034*eV", "2.068*eV", "2.103*eV", "2.139*eV",
       	        	"2.177*eV", "2.216*eV", "2.256*eV", "2.298*eV",
                	"2.341*eV", "2.386*eV", "2.433*eV", "2.481*eV",
                	"2.532*eV", "2.585*eV", "2.640*eV", "2.697*eV",
                	"2.757*eV", "2.820*eV", "2.885*eV", "2.954*eV",
                	"3.026*eV", "3.102*eV", "3.181*eV", "3.265*eV",
                	"3.353*eV", "3.446*eV", "3.545*eV", "3.649*eV",
                	"3.760*eV", "3.877*eV", "4.002*eV", "4.136*eV" );

# Table of optical properties for aerogel
my @AgelRefrIndex = ( 1.02435, 1.0244,  1.02445, 1.0245,  1.02455,
              	      1.0246,  1.02465, 1.0247,  1.02475, 1.0248,
              	      1.02485, 1.02492, 1.025,   1.02505, 1.0251,
             	      1.02518, 1.02522, 1.02530, 1.02535, 1.0254,
              	      1.02545, 1.0255,  1.02555, 1.0256,  1.02568,
               	      1.02572, 1.0258,  1.02585, 1.0259,  1.02595,
              	      1.026,   1.02608 );

my @AgelAbsLength = ( "3.448*m",  "4.082*m",  "6.329*m",  "9.174*m",
		      "12.346*m", "13.889*m", "15.152*m", "17.241*m",
		      "18.868*m", "20.000*m", "26.316*m", "35.714*m",
		      "45.455*m", "47.619*m", "52.632*m", "52.632*m",
	  	      "55.556*m", "52.632*m", "52.632*m", "47.619*m",
		      "45.455*m", "41.667*m", "37.037*m", "33.333*m",
		      "30.000*m", "28.500*m", "27.000*m", "24.500*m",
		      "22.000*m", "19.500*m", "17.500*m", "14.500*m" );

# Table of optical properties for lens Acrylic
my @AcRefrIndex = ( 1.4902, 1.4907, 1.4913, 1.4918, 1.4924,
                  1.4930,  1.4936,  1.4942,  1.4948,  1.4954,
                  1.4960,  1.4965,  1.4971,  1.4977,  1.4983,
                  1.4991,  1.5002,  1.5017,  1.5017,  1.5017,
                  1.5017,  1.5017,  1.5017,  1.5017,  1.5017,
                  1.5017,  1.5017,  1.5017,  1.5017,  1.5017,
                  1.5017,  1.5017 );

my @AcAbsLength = (  "00.448*cm", "00.082*cm", "00.329*cm", "00.174*cm",
		     "00.346*cm", "00.889*cm", "00.152*cm", "00.241*cm",
		     "00.868*cm", "00.000*cm", "00.316*cm", "00.714*cm",
                     "00.455*cm", "00.619*cm", "00.632*cm", "00.632*cm",
		     "00.556*cm", "00.632*cm", "00.632*cm", "00.619*cm",
		     "00.455*cm", "00.667*cm", "00.037*cm", "00.333*cm",
                     "00.001*cm", "00.001*cm", "00.001*cm", "00.001*cm",
		     "00.001*cm", "00.001*cm", "00.001*cm", "00.001*cm" ); 


sub define_material
{
	# the first argument to this function become the variation
	$configuration{"variation"} = shift;

	# Aerogel
	my %mat = init_mat();
	$mat{"name"}          = "aerogel";
	$mat{"description"}   = "eic rich aerogel material";
	$mat{"density"}       = "0.02";  # in g/cm3
	$mat{"ncomponents"}   = "2";
	$mat{"components"}    = "Si 1 O 2";
	$mat{"photonEnergy"}      = arrayToString(@PhotonEnergyBin);
	$mat{"indexOfRefraction"} = arrayToString(@AgelRefrIndex);
	$mat{"absorptionLength"}  = arrayToString(@AgelAbsLength);
	print_mat(\%configuration, \%mat);

	# Lens Acrylic
	%mat = init_mat();
	$mat{"name"}          = "acrylic";
	$mat{"description"}   = "eic rich lens material";
	$mat{"density"}       = "1.19";  # in g/cm3
	$mat{"ncomponents"}   = "3";
	$mat{"components"}    = "C 5 H 8 O 2";
	$mat{"photonEnergy"}      = arrayToString(@PhotonEnergyBin);
	$mat{"indexOfRefraction"} = arrayToString(@AcRefrIndex);
	$mat{"absorptionLength"}  = arrayToString(@AcAbsLength);
	print_mat(\%configuration, \%mat);
}

define_material("Original");
