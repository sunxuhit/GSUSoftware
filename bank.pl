#!/usr/bin/perl -w

use strict;
use lib ("$ENV{GEMC}/io");
use utils;
use bank;

use strict;
use warnings;

# Help Message
sub help()
{
	print "\n Usage: \n";
	print "   bank.pl <configuration filename>\n";
 	print "   Will create the EIC Ring Imaging Cherenkov Detector (RICH) bank\n";
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
our %configuration = load_configuration($ARGV[0]);

# Variable Type is two chars.
# The first char:
#  R for raw integrated variables
#  D for dgt integrated variables
#  S for raw step by step variables
#  M for digitized multi-hit variables
#  V for voltage(time) variables
#
# The second char:
# i for integers
# d for doubles

sub define_bank
{
	# uploading the hit definition
	my $bankId = 500;
	my $bankname = "eic_rich";

	insert_bank_variable(\%configuration, $bankname, "bankid", $bankId, "Di", "$bankname bank ID");

        insert_bank_variable(\%configuration, $bankname, "id",       90, "Di", "Volume ID");
        insert_bank_variable(\%configuration, $bankname, "hitn",     99, "Di", "hit number");

        insert_bank_variable(\%configuration, $bankname, "pid",      1, "Di", "Particle ID");
        insert_bank_variable(\%configuration, $bankname, "mpid",     2, "Di", "Mother Particle ID");
        insert_bank_variable(\%configuration, $bankname, "tid",      3, "Di", "track ID");
        insert_bank_variable(\%configuration, $bankname, "mtid",     4, "Di", "mother track ID");
        insert_bank_variable(\%configuration, $bankname, "otid",     5, "Di", "original track id");
        insert_bank_variable(\%configuration, $bankname, "trackE",   6, "Dd", "totEdep");
        insert_bank_variable(\%configuration, $bankname, "totEdep",  7, "Dd", "Total Energy Deposited");
        insert_bank_variable(\%configuration, $bankname, "avg_x",    8, "Dd", "Average global x position");
        insert_bank_variable(\%configuration, $bankname, "avg_y",    9, "Dd", "Average global y position");
        insert_bank_variable(\%configuration, $bankname, "avg_z",   10, "Dd", "Average global z position");
        insert_bank_variable(\%configuration, $bankname, "avg_lx",  11, "Dd", "Average local  x position");
        insert_bank_variable(\%configuration, $bankname, "avg_ly",  12, "Dd", "Average local  y position");
        insert_bank_variable(\%configuration, $bankname, "avg_lz",  13, "Dd", "Average local  z position");
        insert_bank_variable(\%configuration, $bankname, "px",      14, "Dd", "x component of track momentum");
        insert_bank_variable(\%configuration, $bankname, "py",      15, "Dd", "y component of track momentum");
        insert_bank_variable(\%configuration, $bankname, "pz",      16, "Dd", "z component of track momentum");
        insert_bank_variable(\%configuration, $bankname, "vx",      17, "Dd", "x coordinate of primary vertex");
        insert_bank_variable(\%configuration, $bankname, "vy",      18, "Dd", "y coordinate of primary vertex");
        insert_bank_variable(\%configuration, $bankname, "vz",      19, "Dd", "z coordinate of primary vertex");
        insert_bank_variable(\%configuration, $bankname, "mvx",     20, "Dd", "x coordinate of mother vertex");
        insert_bank_variable(\%configuration, $bankname, "mvy",     21, "Dd", "y coordinate of mother vertex");
        insert_bank_variable(\%configuration, $bankname, "mvz",     22, "Dd", "z coordinate of mother vertex");   
        insert_bank_variable(\%configuration, $bankname, "avg_t",   23, "Dd", "Average t");

        insert_bank_variable(\%configuration, $bankname, "nsteps",  24, "Di", "number of steps for track");
        insert_bank_variable(\%configuration, $bankname, "in_x",    25, "Dd", "entrance global x position");
        insert_bank_variable(\%configuration, $bankname, "in_y",    26, "Dd", "entrance global y position");
        insert_bank_variable(\%configuration, $bankname, "in_z",    27, "Dd", "entrance global z position");
        insert_bank_variable(\%configuration, $bankname, "in_px",   28, "Dd", "entrance global x position");
        insert_bank_variable(\%configuration, $bankname, "in_py",   29, "Dd", "entrance global y position");
        insert_bank_variable(\%configuration, $bankname, "in_pz",   30, "Dd", "entrance global z position");
        insert_bank_variable(\%configuration, $bankname, "in_t",    31, "Dd", "entrance time");
        insert_bank_variable(\%configuration, $bankname, "out_x",   32, "Dd", "exit global x position");
        insert_bank_variable(\%configuration, $bankname, "out_y",   33, "Dd", "exit global y position");
        insert_bank_variable(\%configuration, $bankname, "out_z",   34, "Dd", "exit global z position");
        insert_bank_variable(\%configuration, $bankname, "out_px",  35, "Dd", "exit global x position");
        insert_bank_variable(\%configuration, $bankname, "out_py",  36, "Dd", "exit global y position");
        insert_bank_variable(\%configuration, $bankname, "out_pz",  37, "Dd", "exit global z position");
        insert_bank_variable(\%configuration, $bankname, "out_t",   38, "Dd", "exit time");
}

define_bank();

1;
