#############################################################
#############################################################
#
#  Geometry of the 2nd mRICH prototype detector
#  
#  All dimensions are in mm
#  Use the hit type eic_rich 
#  
#  The geometry are divided in 5 parts: 
#  Aerogel, Fresnel lens, Mirrors, Photonsensor, and Readout
#
#############################################################
#############################################################

use strict;
use warnings;
use Getopt::Long;
use Math::Trig;
use List::Util qw(min max);
use POSIX;

our %configuration;

my $DetectorMother="root";
my $DetectorName = 'meic_det1_rich';
my $hittype="eic_rich";

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~ Define detector size and location ~~~~~~~~~~~~~~~~~~~~~~~~~~#

my $inch_to_mm = 25.4; # 1" = 25.4mm
my $BoxDelz = 2.0;   #gap ?

#===================================================================================================#
#---- Fresnel lens dimension and info ---#
my $fresnelLens_focalLength = 6.0*$inch_to_mm; #=6"
my $fresnelLensEffDiameter = 6.0*$inch_to_mm;   #effective diameter in mm.
my $grooveDensity = 125/25.4;   #100 grooves per inch. converted to grooves per mm.
my $centerThickness = 0.06*$inch_to_mm;
my $lens_numOfGrooves = floor($grooveDensity*($fresnelLensEffDiameter/2));
my $GrooveWidth=1/$grooveDensity;
my $R_min = ($lens_numOfGrooves-1)*$GrooveWidth;
my $R_max = ($lens_numOfGrooves-0)*$GrooveWidth;

my $fresnelLens_halfx = 5.0*$inch_to_mm/2.0; # effect area in simulation: 5"*5"  | real area: (5"+1"/8) * 4.992
my $fresnelLens_halfy = $fresnelLens_halfx;
my $fresnelLens_halfz = (GetSagita($R_max)-GetSagita($R_min)+$centerThickness)/2.0; #dZ + center thickness
my $fresnelLensDiameter = 2.0*sqrt(2.0)*$fresnelLens_halfx;

my $focalLength = $fresnelLens_focalLength; # 6"
my $lens_halfz = $fresnelLens_halfz;
my $lens_gap=25.4/8.0;
#---- Fresnel lens dimension and info ---#

#------------- Detector box -------------#
my $box_thicknessX = (1.0/4.0)*$inch_to_mm;    #1/4 inches aluminum sheet
my $box_thicknessZ = (1.0/16.0)*$inch_to_mm;
my $box_chamber = 2.0*$inch_to_mm; # 2": cushion: 0.5" & aeorgel+foamHodler: 1"+7"/16 & frensnel lens: 1"/16

my $box_halfx = 5.5*$inch_to_mm/2.0; # 5.5"
my $box_halfy=$box_halfx;
my $box_halfz = ($box_thicknessZ+$box_chamber+$focalLength)/2.0; # 8"+1"/16
my $offset = $box_halfz;     #detector box pos_z

# if ($build_copper) { $box_halfz = $box_halfz+(1*$metalSheet_halfz+$insulation)/2.0;}

my $hollow_halfx=$box_halfx-$box_thicknessX; # hollow volume
my $hollow_halfy=$hollow_halfx;
my $hollow_halfz=(2.0*$box_halfz-$box_thicknessZ)/2.0;
#------------- Detector box -------------#

#---------------  Aerogel ---------------#
my $agel_halfx = 55.25;
my $agel_halfy = $agel_halfx;
my $agel_halfz = 15.0; # 3*10mm thick aerogel

my $foamHolderThicknessX = $hollow_halfx-$agel_halfx; # distance between aerogel and holder box
my $foamHolderThicknessZ = $box_chamber-$lens_halfz*2.0 - $agel_halfz*2.0; # box_chamber - lens thickness - aeorgel thickness
my $foamHolder_halfx = $agel_halfx+$foamHolderThicknessX;
my $foamHolder_halfy = $foamHolder_halfx;
my $foamHolder_halfz = $foamHolderThicknessZ/2.0;
#---------------  Aerogel ---------------#

#---------------  Mirrors ---------------#
my $mirror_gap_front = (1.0/16.0)*$inch_to_mm; # 1"/16
my $mirror_gap_back  = (1.0/8.0)*$inch_to_mm; # 1"/8
my $mirrorFront_halfx = 4.5*$inch_to_mm/2.0; # front mirrors open area 4.5"*4.5"
my $mirrorFront_halfy = $mirrorFront_halfx;
my $mirrorBack_halfx = 4.0*$inch_to_mm/2.0; # back mirrors open area 4"*4"
my $mirrorBack_halfy = $mirrorBack_halfx;
my $mirror_halfz = ($focalLength-$mirror_gap_front-$mirror_gap_back)/2.0;
# print"\t",'mirrorlength in z direction =',$mirror_halfz*2.0,"\n";
# print"\t",'mirror_ set: total length in z direction =', ($mirror_gap_front+$mirror_halfz*2.0+$mirror_gap_back)/$inch_to_mm,"\n";
#---------------  Mirrors ---------------#
#===================================================================================================#

#===================================================================================================#
#---------- Readout electronics ---------#
my $readout_thicknessZ    = 3.0*$inch_to_mm; # Xu's guess
my $readout_halfx        = 5.5*$inch_to_mm/2.0; # 5.5" => same as holder box
my $readout_halfy        = $readout_halfx;
my $readout_halfz        = $readout_thicknessZ/2.0;

my $readout_hollow_halfx = 4.0*$inch_to_mm/2.0; # 4" hollow volume for readbout
my $readout_hollow_halfy = $readout_hollow_halfx;
my $readout_hollow_halfz = $readout_halfz;

my $readout_gap = 30.0;     # 3mm: gap between optical box and readout box
my $readout_z = $offset + $box_halfz + $readout_gap + $readout_halfz;

my $airgap_thickness    = $readout_gap;
my $airgap_halfx        = $readout_halfx; # air gap to let photon reach readout box
my $airgap_halfy        = $readout_halfy;
my $airgap_halfz        = $airgap_thickness/2.0;
my $airgap_z = $offset + $box_halfz + $airgap_halfz;

#---------- Readout electronics ---------#

#------------ Photon Sensor -------------#
my $glassWindow_halfx = 52/2;
my $glassWindow_halfy = $glassWindow_halfx;
my $glassWindow_halfz = 0.75;  #glass window thickness=1.5mm

my $sensorGap=0.5;             #half the gap between sensor 
my $phodet_halfx = 24.0;       #1/2 eff. area of Hamamatsu H12700a
my $phodet_halfy = $phodet_halfx;
my $phodet_halfz = 0.75;       #Hamamatsu H12700
my $phodet_gapz = 3.0;

my $metalSheet_halfx=$glassWindow_halfx;
my $metalSheet_halfy=$metalSheet_halfx;
my $metalSheet_halfz=0.5;     #estimation
my $insulation=27.4;          #gap between sensor and metal sheet

my $sensor_total_halfx=2*$glassWindow_halfx+$sensorGap;   #Glass window larger than sensor
my $build_copper=0;            #1: build copper plate
#------------ Photon Sensor -------------#
#===================================================================================================#

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Print detector information  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#---------- optical---------#
my $hollow_z=-$box_halfz+$box_thicknessZ+$hollow_halfz; # w.r.t holder box

my $foamHolder_posz=-$hollow_halfz+$foamHolder_halfz; # w.r.t hollow volume
my $agel_posz=$foamHolder_posz+$foamHolder_halfz+$agel_halfz; # w.r.t hollow volume
my $lens_z=$agel_posz+$agel_halfz+$lens_halfz; # w.r.t hollow volume
#---------- optical---------#

my $readout_hollow_z=-$readout_halfz+$readout_hollow_halfz; # w.r.t readout box
my $glassWindow_z= $lens_z-$lens_halfz+$focalLength+$phodet_gapz+$glassWindow_halfz;
my $phodet_z =$glassWindow_z+$glassWindow_halfz+$phodet_halfz;
my $metalSheet_z=$phodet_z-$phodet_halfz+$insulation-$metalSheet_halfz;

# my @readout_z= ($glassWindow_z-$glassWindow_halfz, $phodet_z+$phodet_halfz);
# if ($build_copper) {$readout_z[1]=$metalSheet_z+$metalSheet_halfz+$readout_halfz;}

my $hollowOffset=$hollow_z+$offset;   #accumulated offset due to asymmetric detector walls (z-direction)
my @detposZ = ( $offset, $hollowOffset, $hollowOffset+$foamHolder_posz, $hollowOffset+$agel_posz, $hollowOffset+$lens_z, $hollowOffset+$phodet_z);

my @freslens = ( 2.0*sqrt(2.0)*$fresnelLensDiameter/8.0, 2.0*sqrt(2.0)*$fresnelLensDiameter/8.0, $fresnelLens_halfz );
# my @readoutposZ = ( $hollowOffset+$readout_z[0], $hollowOffset+$readout_z[1]);
sub print_detector()
{
  my $agelThickness=2*$agel_halfz/10;
 
  print"=====================================================================\n"; 
  print'                   ',$agelThickness,' cm thick Aerogel',"\n";
  print"=====================================================================\n";
  print"Printing detector positions and sizes ...\n\n";
  
  print"hold box       position=(0.0, 0.0, $detposZ[0])mm, half size in XYZ=($box_halfx, $box_halfy, $box_halfz)mm\n";
  print"hollowVol      position=(0.0, 0.0, $detposZ[1])mm, half size in XYZ=($hollow_halfx, $hollow_halfy, $hollow_halfz)mm\n";
  print"foamHolder     position=(0.0, 0.0, $detposZ[2])mm, half size in XYZ=($foamHolder_halfx, $foamHolder_halfy, $foamHolder_halfz)mm\n";
  print"aerogel        position=(0.0, 0.0, $detposZ[3])mm, half size in XYZ=($agel_halfx, $agel_halfy, $agel_halfz)mm\n";
  print"fresnel lens   position=(0.0, 0.0, $detposZ[4])mm, half size in XYZ=($freslens[0], $freslens[1], $freslens[2])mm\n";
  # print"photon sensor  position=(0.0, 0.0, $detposZ[5])mm, half size in XYZ=($phodet_halfx, $phodet_halfy, $phodet_halfz)mm\n";
  # print'glass window   pos_z=',$hollowOffset+$glassWindow_z,'mm, half_z=',$glassWindow_halfz,'mm',"\n";
  # print"readout        position=(0.0, 0.0, $readoutposZ[0] mm, and $readoutposZ[1] mm )\n";
  print"=====================================================================\n";
  print"sensor_total_halfx= $sensor_total_halfx mm\n";
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~ Define the holder Box for Detectors ~~~~~~~~~~~~~~~~~~~~~~~~~#
my $box_name = "detector_holder";
my $box_mat = "G4_Al";
my $hollow_mat="Air_Opt";

sub build_box_optical()
{
    print"building detector holder box...\n";

    my %detector=init_det();
    $detector{"name"} = "$DetectorName\_$box_name";
    $detector{"mother"} = "$DetectorMother";
    $detector{"description"} = "$DetectorName\_$box_name";
    $detector{"pos"} = "0*mm 0*mm $offset*mm"; # w.r.t mother volume
    $detector{"color"} = "81f7f3";
    $detector{"type"} = "Box";
    $detector{"visible"} = "1";
    $detector{"dimensions"} = "$box_halfx*mm $box_halfy*mm $box_halfz*mm";
    $detector{"material"} = "$box_mat";
    $detector{"sensitivity"} = "no";
    $detector{"hit_type"}    = "no";
    $detector{"identifiers"} = "no";
    print_det(\%configuration, \%detector);
  
    %detector=init_det();
    $detector{"name"} = "$DetectorName\_hollow";
    $detector{"mother"} = "$DetectorName\_$box_name";
    $detector{"description"} = "$DetectorName\_hollow";
    $detector{"pos"} = "0*mm 0*mm $hollow_z*mm";   #w.r.t. detector
    $detector{"color"} = "ffffff";
    $detector{"type"} = "Box";
    $detector{"style"} = "0";
    $detector{"visible"} = "1";
    $detector{"dimensions"} = "$hollow_halfx*mm $hollow_halfy*mm $hollow_halfz*mm";
    $detector{"material"} = "$hollow_mat";
    $detector{"sensitivity"} = "no";
    $detector{"hit_type"}    = "no";
    $detector{"identifiers"} = "no";
    print_det(\%configuration, \%detector);
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Foam holder ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my $foamHolder_name="FoamHolder";
my $foamHolder_mat="Air_Opt";

sub build_foamHolder()
{
    print"building foam holder...\n";

    my %detector=init_det();
    $detector{"name"} = "$DetectorName\_$foamHolder_name";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$DetectorName\_$foamHolder_name";
    $detector{"pos"} = "0*mm 0*mm $foamHolder_posz*mm";
    $detector{"color"} = "00ff00";
    $detector{"type"} = "Box";
    $detector{"visible"} = "1";
    $detector{"dimensions"} = "$foamHolder_halfx*mm $foamHolder_halfy*mm $foamHolder_halfz*mm";
    $detector{"material"} = "$foamHolder_mat";
    $detector{"sensitivity"} = "no";
    $detector{"hit_type"}    = "no";
    $detector{"identifiers"} = "no";
    print_det(\%configuration, \%detector);

    my @foamHolder_z=($agel_posz-$agel_halfz,$agel_posz+$agel_halfz);
    my @foamHolder_rinner = ( $agel_halfx, $agel_halfx );
    my @foamHolder_router = ( $agel_halfx+$foamHolderThicknessX, $agel_halfx+$foamHolderThicknessX );

    $detector{"name"} = "$DetectorName\_$foamHolder_name\_1";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$DetectorName\_$foamHolder_name";
    $detector{"pos"} = "0*mm 0*mm 0*mm";
    $detector{"rotation"} = "0*deg 0*deg 0*deg";
    $detector{"color"} = "00ff00";
    #$detector{"style"} = "1";
    $detector{"type"} = "Pgon";    ### Polyhedra                                                                            
    $detector{"visible"} = "0";
    my $dimen = "45*deg 360*deg 4*counts 2*counts";
    for(my $i=0; $i<2; $i++) {$dimen = $dimen ." $foamHolder_rinner[$i]*mm";}
    for(my $i=0; $i<2; $i++) {$dimen = $dimen ." $foamHolder_router[$i]*mm";}
    for(my $i=0; $i<2; $i++) {$dimen = $dimen ." $foamHolder_z[$i]*mm";}
    $detector{"dimensions"} = "$dimen";
    $detector{"material"} = "$foamHolder_mat";
    $detector{"sensitivity"} = "no";
    $detector{"hit_type"}    = "no";
    $detector{"identifiers"} = "no";
    print_det(\%configuration, \%detector);
}

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Aerogel ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my $agel_name = "Aerogel";
my $agel_mat  = "aerogel";
#my $agel_mat  = "G4_WATER";

sub build_aerogel()
{
    print"building aerogel block...\n";

    my %detector=init_det();
    $detector{"name"} = "$DetectorName\_$agel_name";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$DetectorName\_$agel_name";
    $detector{"pos"} = "0*mm 0*mm $agel_posz*mm";
    $detector{"color"} = "ff0002";
    $detector{"type"} = "Box";
    $detector{"visible"} = "1";
    $detector{"dimensions"} = "$agel_halfx*mm $agel_halfy*mm $agel_halfz*mm";
    $detector{"material"} = "$agel_mat";
    $detector{"sensitivity"} = "$hittype";
    $detector{"hit_type"}    = "$hittype";
    $detector{"identifiers"} = "id manual 1";
    print_det(\%configuration, \%detector);
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Fresnel lens ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my $lens_numOfHoldBox = 4;        #number of hold box for fresnel lens
my $lens_holdbox_name="$DetectorName\_lensHoldBox";
my $lens_holdbox_mat  = "Air_Opt";

my $lens_mat  = "acrylic";
my $max_numOfGrooves=1000;

#========================================#
#---------- build Fresnel Lens ----------#
sub build_fresnel_lens()
{
    print"building Fresnel Lens... It takes some time\n";
    print"\tFresnel Lens: num. of grooves = $lens_numOfGrooves groove width = $GrooveWidth\n";
    print"\tFresnel Lens: Lens diameter = $fresnelLensDiameter\n";
    print"\tFresnel Lens: effective diameter = $fresnelLensEffDiameter mm \n";

    my $count=0;
    #========================================#
    #----- holder box for fresnel lens ------#
    my $lens_holdbox_halfz=$fresnelLens_halfz;
    print"\tholdbox half thickness= ",$lens_holdbox_halfz,"\n";
    #my @lens_holdbox_rotZ = ( -270, -180, -90, 0 );
    #========================================#
    #----------- build holder box -----------#
    my %detector; 
    %detector=init_det();
    $detector{"name"} = "$lens_holdbox_name";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$lens_holdbox_name";
    $detector{"pos"} = "0*mm 0*mm $lens_z*mm";
    $detector{"color"} = "ff0000";
    $detector{"type"} = "Box";
    $detector{"dimensions"} = "$fresnelLens_halfx*mm $fresnelLens_halfy*mm $lens_holdbox_halfz*mm";
    $detector{"material"} = $lens_holdbox_mat;
    $detector{"rotation"} = "0*deg -180*deg 0*deg";
    $detector{"visible"} = "0";
    $detector{"sensitivity"} = "no";
    $detector{"hit_type"}    = "no";
    $detector{"identifiers"} = "no";
    print_det(\%configuration, \%detector);
    
    #========================================#
    #------------ build grooves -------------#
    for(my $igroove=0; $igroove<$max_numOfGrooves; $igroove++){
	#--------------------------------------------------------#
	#Grooves' inner and outer radius
	my $iRmin1 = ($igroove+0)*$GrooveWidth;
	my $iRmax1 = ($igroove+1)*$GrooveWidth;
	my $iRmin2 = $iRmin1;
	my $iRmax2 = $iRmin2+0.0001;
	
	my @lens_poly_rmin = ($iRmin1, $iRmin1, $iRmin2);
        my @lens_poly_rmax = ($iRmax1, $iRmax1, $iRmax2);

	if ($iRmax1>$fresnelLensDiameter/2.0) { last; }    #if iRmin1>Lens radius, outside the lens, break
	#--------------------------------------------------------#
	#phi angle (from Ping)
	my $phi1;
	my $phi2;
	my $deltaPhi;
	
	if ($iRmax1<$fresnelLens_halfx) {   #1/4 of a full circle
	    $phi1=0;                 #in degree
	    $deltaPhi=360;           #in degree
	}
	else {
	    $phi1=acos($fresnelLens_halfx/$iRmax1)*180/pi;   #in degree
	    $phi2=asin($fresnelLens_halfy/$iRmax1)*180/pi;   #in degree
	    $deltaPhi=$phi2-$phi1;
	    #if ($iholdbox==0) {print "this is ", $igroove, "th grooves, phi1=",$phi1,", delPhi=",$deltaPhi,"\n";}
	}
	#--------------------------------------------------------#
	#grooves profile
	my $dZ=0.06*25.4;                     #center thickness=0.06 inches
	my @lens_poly_z;
	my $numOfLayer;

	if ($iRmin1<$fresnelLensEffDiameter/2.0) {   #if iRmin>=effective radius, dZ=0, i.e. flat
	    $numOfLayer=3;
	    $dZ = GetSagita($iRmax1) - GetSagita($iRmin1);
	    #print "alpha=",atan($GrooveWidth/$dZ)*180/pi," deg\n";
	    if($dZ<=0) { print "build_lens::Groove depth<0 !\n"; }
	    @lens_poly_z    = (-1*$fresnelLens_halfz, $fresnelLens_halfz-$dZ, $fresnelLens_halfz);
	}
	else {
	    $numOfLayer=2;
	    @lens_poly_z    = (-1*$fresnelLens_halfz, -1*$fresnelLens_halfz+$dZ);
	}
	#--------------------------------------------------------#
	my $repeat=1;
	my $draw=1;
	my $grooveName="$lens_holdbox_name\_groove$igroove";
	
	if ($iRmax1>=$fresnelLens_halfx) { $repeat=4; }   #4 edges
	for (my $i=0;$i<$repeat;$i++) {
	    if ($repeat==4) { $grooveName="$lens_holdbox_name\_groove$igroove\_edge$i"; }
	    
	    my $dimen;
	    $dimen = "$phi1*deg $deltaPhi*deg $numOfLayer*counts";                  #Ping
	    for(my $i = 0; $i <$numOfLayer; $i++) {$dimen = $dimen ." $lens_poly_rmin[$i]*mm";}
	    for(my $i = 0; $i <$numOfLayer; $i++) {$dimen = $dimen ." $lens_poly_rmax[$i]*mm";}
	    for(my $i = 0; $i <$numOfLayer; $i++) {$dimen = $dimen ." $lens_poly_z[$i]*mm";}
	    
	    %detector=init_det();
	    $detector{"name"} = "$grooveName";
	    $detector{"mother"} = "$lens_holdbox_name";
	    $detector{"description"} = "$grooveName";
	    $detector{"pos"} = "0*mm 0*mm 0*mm";   #w.r.t. lens hold box
	    $detector{"color"} = "2eb7ed";
	    $detector{"type"} = "Polycone";
	    $detector{"dimensions"} = "$dimen";
	    $detector{"material"} = "$lens_mat";
	    $detector{"style"} = "1";
	    $detector{"visible"} = "$draw";
	    $detector{"sensitivity"} = "no";
	    $detector{"hit_type"}    = "no";
	    $detector{"identifiers"} = "no";
	    print_det(\%configuration, \%detector);
	    
	    $phi1=$phi1+90;
	}
    }
}
#========================================#
#------- arc shape, spherical lens ------#
sub GetSagita
{
  my $Conic = -1.0;		      #original
  # my $lens_type = 3;
  my $lens_type = 5;                  #spherical Fresnel lens
  my $Curvature;
  my @Aspher = (0, 0, 0, 0, 0, 0, 0, 0 );
  my $n=1.49;                         #refractive index of Fresnel lens
  
  if ($lens_type == 1) {
      $Curvature = 0.00437636761488;
      $Aspher[0] = 4.206739256e-05;
      $Aspher[1] = 9.6440152e-10;
      $Aspher[2] = -1.4884317e-15;
  }
  
  if ($lens_type == 2) {	       #r=77mm, f~14cm
      $Curvature = 0.0132;
      $Aspher[0] = 32.0e-05;
      $Aspher[1] = -2.0e-7;
      $Aspher[2] =  1.2e-13;
  }
  
  if ($lens_type == 3) {	       #r=77mm, f~12.5cm
      $Curvature = 0.0150;
      $Aspher[0] = 42.0e-05;
      $Aspher[1] = -3.0e-7;
      $Aspher[2] =  1.2e-13;
  }
  if ($lens_type == 4) {	       #r=77mm, f~10cm
      $Curvature = 0.0175;
      $Aspher[0] = 72.0e-05;
      $Aspher[1] = -5.0e-7;
      $Aspher[2] =  1.2e-13;
  }
  if  ($lens_type == 5) {              #Ping: curvature=1/(fresnelLens_focalLength*(n-1))
      #$Curvature=0.0267;
      #$Curvature=0.0287;   
      $Curvature=1/($fresnelLens_focalLength*($n-1));
  }
  
  my $TotAspher = 0.0;
  for(my $k=1;$k<9;$k++){ $TotAspher += $Aspher[$k-1]*($_[0]**(2*$k)); }
  
  my $ArgSqrt = 1.0-(1.0+$Conic)*($Curvature**2)*($_[0]**2);
  if ($ArgSqrt < 0.0){ print "build_lens::Sagita: Square Root of <0 ! \n"; }
  
  my $Sagita_value = $Curvature*($_[0]**2)/(1.0+sqrt($ArgSqrt)) + $TotAspher;
  return $Sagita_value ;
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ reflection mirrors ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my $mirror_mat  = "Aluminum";
sub build_mirrors()
{
    print"building mirror set...\n";

    my $mirror_thickness=2;
    my @mirror_z=($lens_z+$lens_halfz+$mirror_gap_front,$lens_z+$lens_halfz+$mirror_gap_front+$mirror_halfz*2.0);
    my @mirror_router = ( $mirrorFront_halfx+$mirror_thickness, $mirrorBack_halfx+$mirror_thickness );
    my @mirror_rinner = ( $mirrorFront_halfx, $mirrorBack_halfx);
    
    print"\t",'mirror: length in z direction =',$mirror_z[1]-$mirror_z[0],"\n";

    my $idManual=3;
    my %detector=init_det();
    $detector{"name"} = "$DetectorName\_mirror";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$DetectorName\_mirror";
    $detector{"pos"} = "0*mm 0*mm 0*mm";
    $detector{"color"} = "ffff00";
    $detector{"type"} = "Pgon";
    $detector{"visible"} = "1";

    my $dimen = "45*deg 360*deg 4*counts 2*counts";
    for(my $i=0; $i<2; $i++) {$dimen = $dimen ." $mirror_rinner[$i]*mm";}
    for(my $i=0; $i<2; $i++) {$dimen = $dimen ." $mirror_router[$i]*mm";}
    for(my $i=0; $i<2; $i++) {$dimen = $dimen ." $mirror_z[$i]*mm";}

    $detector{"dimensions"} = "$dimen";
    $detector{"material"} = "$mirror_mat";
    $detector{"sensitivity"} = "mirror: rich_mirrors";
    $detector{"hit_type"}    = "no";
    $detector{"identifiers"} = "no";
    print_det(\%configuration, \%detector);
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ air gap~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my $airgap_name = "airgap";
my $airgap_mat="Air_Opt";
sub build_box_airgap()
{
    print"building airgap ...\n";

    my %detector=init_det();
    $detector{"name"} = "$DetectorName\_$airgap_name";
    $detector{"mother"} = "$DetectorMother";
    $detector{"description"} = "$DetectorName\_$airgap_name";
    $detector{"pos"} = "0*mm 0*mm $airgap_z*mm"; # w.r.t. mother volume
    $detector{"rotation"} = "0*deg 0*deg 0*deg";
    $detector{"color"} = "ff0002";
    # $detector{"color"} = "81f7f3";
    # $detector{"color"} = "ffffff";
    $detector{"type"} = "Box";
    $detector{"visible"} = "1";
    $detector{"dimensions"} = "$airgap_halfx*mm $airgap_halfy*mm $airgap_halfz*mm";
    $detector{"material"} = "$airgap_mat";
    $detector{"sensitivity"} = "no";
    $detector{"hit_type"}    = "no";
    $detector{"identifiers"} = "no";
    print_det(\%configuration, \%detector);
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ readout hardware ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my $readoutdet_name = "readout";
my $readout_mat  = "Aluminum";
my $readout_hollow_mat="Air_Opt";
sub build_box_readout()
{
    print"building readout holder box...\n";

    my %detector=init_det();
    $detector{"name"} = "$DetectorName\_$readoutdet_name";
    $detector{"mother"} = "$DetectorMother";
    $detector{"description"} = "$DetectorName\_$readoutdet_name";
    $detector{"pos"} = "0*mm 0*mm $readout_z*mm"; # w.r.t. mother volume
    $detector{"rotation"} = "0*deg 0*deg 0*deg";
    # $detector{"color"} = "81f7f3";
    $detector{"color"} = "00ff00";
    $detector{"type"} = "Box";
    $detector{"visible"} = "1";
    $detector{"dimensions"} = "$readout_halfx*mm $readout_halfy*mm $readout_halfz*mm";
    $detector{"material"} = "$readout_mat";
    $detector{"sensitivity"} = "no";
    $detector{"hit_type"}    = "no";
    $detector{"identifiers"} = "no";
    print_det(\%configuration, \%detector);
  
    %detector=init_det();
    $detector{"name"} = "$DetectorName\_readout_hollow";
    $detector{"mother"} = "$DetectorName\_$readoutdet_name";
    $detector{"description"} = "$DetectorName\_readout_hollow";
    $detector{"pos"} = "0*mm 0*mm $readout_hollow_z*mm";   #w.r.t. readout box
    $detector{"color"} = "ffffff";
    $detector{"type"} = "Box";
    $detector{"style"} = "0";
    $detector{"visible"} = "1";
    $detector{"dimensions"} = "$readout_hollow_halfx*mm $readout_hollow_halfy*mm $readout_hollow_halfz*mm";
    $detector{"material"} = "$readout_hollow_mat";
    $detector{"sensitivity"} = "no";
    $detector{"hit_type"}    = "no";
    $detector{"identifiers"} = "no";
    print_det(\%configuration, \%detector);
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ photon sensor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my $photondet_name = "Photondet";
#my $photondet_mat  = "Aluminum";
my $photondet_mat  = "Air_Opt";

my $last_x=$glassWindow_halfx+$sensorGap;
my $last_y=$last_x;   #1st quandrant
sub build_photondet()
{
    print"building photonsensor...\n";
    print"\t",'gap=',$last_x-$phodet_halfx,"\n";
    my $photondet_x;
    my $photondet_y;

    for (my $i=1;$i<5;$i++) {
	#--------------------------------------------------------#
	# change quandrant                                                                                                    
	if ($i==0) {
	    $photondet_x=$last_x;
	    $photondet_y=$last_y;
	}
	else {
	    $photondet_x=-$last_y;
	    $photondet_y=$last_x;
	}
	#--------------------------------------------------------#     

	#========================================#
	#--------- build glass window -----------#
	my %detector=init_det();
	$detector{"name"} = "$DetectorName\_glassWindow$i";
	$detector{"mother"} = "$DetectorName\_hollow";
	$detector{"description"} = "$DetectorName\_glassWindow$i";
	$detector{"pos"} = "$photondet_x*mm $photondet_y*mm $glassWindow_z*mm";
	$detector{"rotation"} = "0*deg 0*deg 0*deg";
	$detector{"color"} = "1ABC9C";
	$detector{"style"} = "0";
	$detector{"visible"} = "1";
	$detector{"type"} = "Box";
	$detector{"dimensions"} = "$glassWindow_halfx*mm $glassWindow_halfy*mm $glassWindow_halfz*mm";
	$detector{"material"} = "glass";
	#$detector{"material"} = "Air_Opt";
	$detector{"mfield"} = "no";
	$detector{"sensitivity"} = "no";
	$detector{"hit_type"}    = "no";
	$detector{"identifiers"} = "no";
	print_det(\%configuration, \%detector);
	
	#========================================#
	#------------ build sensor --------------#
	%detector=init_det();
	$detector{"name"} = "$DetectorName\_$photondet_name\_$i";
	$detector{"mother"} = "$DetectorName\_hollow";
	$detector{"description"} = "$DetectorName\_$photondet_name\_$i";
	$detector{"pos"} = "$photondet_x*mm $photondet_y*mm $phodet_z*mm";
	$detector{"rotation"} = "0*deg 0*deg 0*deg";
	$detector{"color"} = "0000A0";
	$detector{"style"} = "0";
	$detector{"visible"} = "1";
	$detector{"type"} = "Box";
	$detector{"dimensions"} = "$phodet_halfx*mm $phodet_halfy*mm $phodet_halfz*mm";
	$detector{"material"} = "$photondet_mat";
	$detector{"mfield"} = "no";
	$detector{"sensitivity"} = "$hittype";
	$detector{"hit_type"}    = "$hittype";
	$detector{"identifiers"} = "id manual 2";
	print_det(\%configuration, \%detector);
	
	#print'photondet_x=',$photondet_x,', photondet_y=',$photondet_y,"\n"; 

	#========================================#
	#--------- build metal sheet ------------#
	if ($build_copper) {
	    %detector=init_det();
	    $detector{"name"} = "$DetectorName\_sensorMetalSheet\_$i";
	    $detector{"mother"} = "$DetectorName\_hollow";
	    $detector{"description"} = "$DetectorName\_sensorMetalSheet\_$i";
	    $detector{"pos"} = "$photondet_x*mm $photondet_y*mm $metalSheet_z*mm";
	    $detector{"rotation"} = "0*deg 0*deg 0*deg";
	    $detector{"color"} = "ffcc00";
	    $detector{"type"} = "Box";
	    $detector{"dimensions"} = "$metalSheet_halfx*mm $metalSheet_halfy*mm $metalSheet_halfz*mm";
	    $detector{"material"} = "G4_Cu";
	    $detector{"mfield"} = "no";
	    $detector{"sensitivity"} = "$hittype";
	    $detector{"hit_type"}    = "$hittype";
	    $detector{"identifiers"} = "id manual 3";
	    print_det(\%configuration, \%detector);
	}

        $last_x=$photondet_x;
        $last_y=$photondet_y;
    }# end of for loop
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Main function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
sub build_detector()
{
    print_detector();
    print"=====================================================================\n";
    build_box_optical();
    build_foamHolder();
    build_aerogel();
    # build_fresnel_lens();
    build_mirrors();
    build_box_airgap();
    build_box_readout();
    # build_photondet();
    print"done\n";
    print"=====================================================================\n";
}
