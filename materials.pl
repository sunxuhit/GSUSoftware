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


#===============================  Photon energy bins ===================================#
#my @PhotonEnergyBin = ( "2.034*eV", "2.068*eV", "2.103*eV", "2.139*eV",
#       	        	"2.177*eV", "2.216*eV", "2.256*eV", "2.298*eV",
#                	"2.341*eV", "2.386*eV", "2.433*eV", "2.481*eV",
#                	"2.532*eV", "2.585*eV", "2.640*eV", "2.697*eV",
#                	"2.757*eV", "2.820*eV", "2.885*eV", "2.954*eV",
#                	"3.026*eV", "3.102*eV", "3.181*eV", "3.265*eV",
#                	"3.353*eV", "3.446*eV", "3.545*eV", "3.649*eV",
#                	"3.760*eV", "3.877*eV", "4.002*eV", "4.136*eV" );

my @PhotonEnergyBin = ( 2.034, 2.068, 2.103, 2.139,
       	        	2.177, 2.216, 2.256, 2.298,
                	2.341, 2.386, 2.433, 2.481,
                	2.532, 2.585, 2.640, 2.697,
                	2.757, 2.820, 2.885, 2.954,
                	3.026, 3.102, 3.181, 3.265,
                	3.353, 3.446, 3.545, 3.649,
                	3.760, 3.877, 4.002, 4.136 );
#=======================================================================================#
#======================= Table of optical properties for aerogel =======================#

#my @AgelRefrIndex = ( 1.02435, 1.0244,  1.02445, 1.0245,  1.02455,
#              	      1.0246,  1.02465, 1.0247,  1.02475, 1.0248,
#              	      1.02485, 1.02492, 1.025,   1.02505, 1.0251,
#             	      1.02518, 1.02522, 1.02530, 1.02535, 1.0254,
#              	      1.02545, 1.0255,  1.02555, 1.0256,  1.02568,
#              	      1.02572, 1.0258,  1.02585, 1.0259,  1.02595,
#              	      1.026,   1.02608 );

my @AgelRefrIndex = ( 1.03, 1.03, 1.03, 1.03, 1.03, 1.03, 1.03, 1.03,
		      1.03, 1.03, 1.03, 1.03, 1.03, 1.03, 1.03, 1.03,
		      1.03, 1.03, 1.03, 1.03, 1.03, 1.03, 1.03, 1.03,
		      1.03, 1.03, 1.03, 1.03, 1.03, 1.03, 1.03, 1.03 );

my @AgelAbsLength = ( 3.448,  4.082,  6.329,  9.174,
		      12.346, 13.889, 15.152, 17.241,
		      18.868, 20.000, 26.316, 35.714,
		      45.455, 47.619, 52.632, 52.632,
	  	      55.556, 52.632, 52.632, 47.619,
		      45.455, 41.667, 37.037, 33.333,
		      30.000, 28.500, 27.000, 24.500,
		      22.000, 19.500, 17.500, 14.500 );

my @AgelRScatLength;           #Rayleigh Scattering
my $AgelTypeAClarity=0.002;   #unit: micrometer^4*cm^-1
my $Cparam=$AgelTypeAClarity;  #unit: dimensionless
my $PhotMomWaveConv=1239;      #unit eV*nm
my $i;

if ($Cparam!=0) {
    for ($i=0;$i<32;$i++) {
	my $WaveLength=($PhotMomWaveConv/$PhotonEnergyBin[$i])/1000;   #unit: micrometer
	$AgelRScatLength[$i]=($WaveLength**4)/$Cparam;
	$AgelRScatLength[$i]="$AgelRScatLength[$i]*nm*nm*nm*nm";
    }
}

for ($i=0;$i<32;$i++) { 
    $AgelAbsLength[$i]="$AgelAbsLength[$i]*m";
    $PhotonEnergyBin[$i]="$PhotonEnergyBin[$i]*eV";
}

#print"\n\n Agel Rayleigh scattering:\n";
#for ($i=0;$i<32;$i++) {
#    print"$AgelRScatLength[$i]\n";
#}

#print"AgelAbsLength array: @AgelAbsLength\n";
#print"PhotonEnergyBin array: @PhotonEnergyBin\n";    

#--------------------------- Aerogel definition from Marco -----------------------------#
my @RichAerogel_PhoE=
    ("1.87855*eV","1.96673*eV","2.05490*eV","2.14308*eV","2.23126*eV",
     "2.31943*eV","2.40761*eV","2.49579*eV","2.58396*eV","2.67214*eV",
     "2.76032*eV","2.84849*eV","2.93667*eV","3.02485*eV","3.11302*eV",
     "3.20120*eV","3.28938*eV","3.37755*eV","3.46573*eV","3.55391*eV",
     "3.64208*eV","3.73026*eV","3.81844*eV","3.90661*eV","3.99479*eV",
     "4.08297*eV","4.17114*eV","4.25932*eV","4.34750*eV","4.43567*eV",
     "4.52385*eV","4.61203*eV","4.70020*eV","4.78838*eV","4.87656*eV",
     "4.96473*eV","5.05291*eV","5.14109*eV","5.22927*eV","5.31744*eV",
     "5.40562*eV","5.49380*eV","5.58197*eV","5.67015*eV","5.75833*eV",
     "5.84650*eV","5.93468*eV","6.02286*eV","6.11103*eV","6.19921*eV" );
my @RichAerogel_Rind3=
    ("1.02825","1.02829","1.02834","1.02839","1.02844",
     "1.02849","1.02854","1.02860","1.02866","1.02872",
     "1.02878","1.02885","1.02892","1.02899","1.02906",
     "1.02914","1.02921","1.02929","1.02938","1.02946",
     "1.02955","1.02964","1.02974","1.02983","1.02993",
     "1.03003","1.03014","1.03025","1.03036","1.03047",
     "1.03059","1.03071","1.03084","1.03096","1.03109",
     "1.03123","1.03137","1.03151","1.03166","1.03181",
     "1.03196","1.03212","1.03228","1.03244","1.03261",
     "1.03279","1.03297","1.03315","1.03334","1.03354" );
my @RichAerogel_Abs=
    ("17.5000*cm","17.7466*cm","17.9720*cm","18.1789*cm","18.3694*cm",
     "18.5455*cm","18.7086*cm","18.8602*cm","19.0015*cm","19.1334*cm",
     "19.2569*cm","19.3728*cm","19.4817*cm","19.5843*cm","19.6810*cm",
     "19.7725*cm","19.8590*cm","19.9410*cm","20.0188*cm","20.0928*cm",
     "18.4895*cm","16.0174*cm","13.9223*cm","12.1401*cm","10.6185*cm",
     "9.3147*cm","8.1940*cm","7.2274*cm","6.3913*cm","5.6659*cm",
     "5.0347*cm","4.4841*cm","4.0024*cm","3.5801*cm","3.2088*cm",
     "2.8817*cm","2.5928*cm","2.3372*cm","2.1105*cm","1.9090*cm",
     "1.7296*cm","1.5696*cm","1.4266*cm","1.2986*cm","1.1837*cm",
     "1.0806*cm","0.9877*cm","0.9041*cm","0.8286*cm","0.7603*cm" );

#rescale RichAerogel_Scat array -- 
my @RichAerogel_Scat=
    ( "35.1384*cm", "29.24805*cm", "24.5418*cm", "20.7453*cm", "17.6553*cm",
      "15.1197*cm", "13.02345*cm", "11.2782*cm", "9.81585*cm", "8.58285*cm",
      "7.53765*cm", "6.6468*cm", "5.88375*cm", "5.22705*cm", "4.6596*cm",
      "4.167*cm", "3.73785*cm", "3.36255*cm", "3.03315*cm", "2.7432*cm", 
      "2.487*cm", "2.26005*cm", "2.05845*cm", "1.87875*cm", "1.71825*cm", 
      "1.57455*cm", "1.44555*cm", "1.3296*cm", "1.2249*cm", "1.1304*cm",
      "1.04475*cm", "0.9672*cm", "0.89655*cm", "0.83235*cm", "0.77385*cm",
      "0.7203*cm", "0.67125*cm", "0.6264*cm", "0.58515*cm", "0.54735*cm",
      "0.51255*cm", "0.48045*cm", "0.45075*cm", "0.4233*cm", "0.39795*cm", 
      "0.37455*cm", "0.3528*cm", "0.33255*cm", "0.3138*cm", "0.29625*cm",);

#=======================================================================================#
#==================== Table of optical properties for lens Acrylic =====================#
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
#=======================================================================================#

sub define_material
{
    my $Hubert=0;
    my $Marco=1;

    my $agel=$Marco;
    my $RS=1; #Rayleigh Scattering: 0--Off, 1--On

# the first argument to this function become the variation
    $configuration{"variation"} = shift;
    
    # Aerogel
    my %mat = init_mat();
    $mat{"name"}          = "aerogel";
    $mat{"description"}   = "eic rich aerogel material";
    $mat{"density"}       = "0.02";  # in g/cm3
    $mat{"ncomponents"}   = "2";
    $mat{"components"}    = "Si 1 O 2";

    if ($agel == $Hubert) {
        $mat{"photonEnergy"} = arrayToString(@PhotonEnergyBin);
        $mat{"indexOfRefraction"} = arrayToString(@AgelRefrIndex);
        $mat{"absorptionLength"}  = arrayToString(@AgelAbsLength);
        if ($RS==1) {$mat{"rayleigh"} = arrayToString(@AgelRScatLength);}
    }
    elsif ($agel == $Marco) {
        $mat{"photonEnergy"} = arrayToString(@RichAerogel_PhoE);                                          
	$mat{"indexOfRefraction"} = arrayToString(@RichAerogel_Rind3);
        $mat{"absorptionLength"}  = arrayToString(@RichAerogel_Abs);
        if ($RS==1) {$mat{"rayleigh"} = arrayToString(@RichAerogel_Scat);}
    }
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
    
    # Detector holder Acrylic
    %mat = init_mat();
    $mat{"name"}          = "holder_acrylic";
    $mat{"description"}   = "eic rich detector holder material";
    $mat{"density"}       = "1.19";  # in g/cm3
    $mat{"ncomponents"}   = "3";
    $mat{"components"}    = "C 5 H 8 O 2";
    print_mat(\%configuration, \%mat);
    	
}

define_material("Original");
