#! /bin/tcsh

./geometry.pl config.dat
./bank.pl config.dat
./hit.pl config.dat
./materials.pl config.dat
./mirrors.pl config.dat
eic_gemc meic_det1.gcard
