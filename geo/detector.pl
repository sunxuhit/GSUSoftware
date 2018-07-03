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

my $BoxDelz = 2.0;   #gap ?
#========================================#
#---------------  Aerogel ---------------#
my $agel_halfx = 55.25;
my $agel_halfy = $agel_halfx;
my $agel_halfz = 16.5; #3.3cm thick agel

my $foamHolderThicknessX=10.0;
my $foamHolderThicknessZ=10.0;
my $foamHolder_halfx=$agel_halfx+$foamHolderThicknessX;
my $foamHolder_halfy=$foamHolder_halfx;
my $foamHolder_halfz=$foamHolderThicknessZ/2.0;

#========================================#
#---- lens dimension and info ---#

#---- Fresnel lens dimension and info ---#
my $fresnelLens_focalLength = 152.4;        #=6"
my $fresnelLensEffDiameter =152.4;   #effective diameter in mm.
my $grooveDensity=125/25.4;   #100 grooves per inch. converted to grooves per mm.
my $centerThickness=0.06*25.4;
my $lens_numOfGrooves = floor($grooveDensity*($fresnelLensEffDiameter/2));
my $GrooveWidth=1/$grooveDensity;
my $R_min = ($lens_numOfGrooves-1)*$GrooveWidth;
my $R_max = ($lens_numOfGrooves-0)*$GrooveWidth;

my $fresnelLens_halfx=66.675;   #beam test 2016
my $fresnelLens_halfy=$fresnelLens_halfx;
my $fresnelLens_halfz = (GetSagita($R_max)-GetSagita($R_min)+$centerThickness)/2.0; #dZ + center thickness
my $fresnelLensDiameter = 2.0*sqrt(2.0)*$fresnelLens_halfx;
#---- Fresnel lens dimension and info ---#

#---- Plano lens dimension and info ---#
my $planoLens_BFL = 189.0;
my $planoLens_D = 100.0;
my $planoLens_CT = 17.0;
my $planoLens_R = 103.5;
my $planoLens_ET = $planoLens_CT - $planoLens_R+sqrt($planoLens_R*$planoLens_R-0.5*$planoLens_D*0.5*$planoLens_D);
my $planoLens_ST = $planoLens_CT - $planoLens_ET;
my $planoLens_EFL = 200.0;

my $planoLens_halfx = $planoLens_D/2.0;
my $planoLens_halfy = $planoLens_halfx;
my $planoLens_halfz = $planoLens_CT/2.0;
#---- Plano lens dimension and info ---#

my $lensType = 1; # 0 for plano-convex lens | 1 for fresnel lens
my $lens_gap=25.4/8.0;

my $focalLength;
my $lens_halfz;

if($lensType == 1)
{
  $focalLength = $fresnelLens_focalLength; # 6"
  $lens_halfz = $fresnelLens_halfz;
}
if($lensType == 0)
{
  $focalLength = $planoLens_BFL; # 8"
  $lens_halfz = $planoLens_halfz;
}
#---- lens dimension and info ---#
#========================================#

#========================================#
#------------ Photon Sensor -------------#
my $glassWindow_halfx=52/2;
my $glassWindow_halfy= $glassWindow_halfx;
my $glassWindow_halfz= 0.75;  #glass window thickness=1.5mm

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
my $build_copper=1;            #1: build copper plate
#========================================#
#---------- Readout electronics ---------#
my $readout_halfz = 4.0;           # I don't like it.
my $readout_thickness=2.0;
#========================================#
#------------- Detector box -------------#
my @all_halfx=($foamHolder_halfx,$fresnelLens_halfx,$sensor_total_halfx+$readout_thickness);

my $box_thicknessX=(1.0/4.0)*25.4;    #1/4 inches aluminum sheet
my $box_thicknessZ1=(1.0/16.0)*25.4;
my $box_thicknessZ2=(1.0/4.0)*25.4;

my $box_halfx = max(@all_halfx) + $box_thicknessX+1.0;
my $box_halfy=$box_halfx;
my $box_halfz = ($BoxDelz+2*$foamHolder_halfz+2*$agel_halfz
		 +$lens_gap+2*$lens_halfz+$focalLength+$phodet_gapz
		 +2*$glassWindow_halfz+2*$phodet_halfz+(2*$readout_halfz+$BoxDelz)
		 +$box_thicknessZ1+$box_thicknessZ2)/2.0;

if ($build_copper) { $box_halfz = $box_halfz+(1*$metalSheet_halfz+$insulation)/2.0;}

my $offset = $box_halfz+50;     #detector box pos_z

my $hollow_halfx=$box_halfx-$box_thicknessX;
my $hollow_halfy=$hollow_halfx;
my $hollow_halfz=(2.0*$box_halfz-$box_thicknessZ1-$box_thicknessZ2)/2.0;
#========================================#

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Print detector information  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my $hollow_z=-$box_halfz+$hollow_halfz+$box_thicknessZ1;

my $foamHolder_posz=-$hollow_halfz+$BoxDelz+$foamHolder_halfz;
my $agel_posz=$foamHolder_posz+$foamHolder_halfz+$agel_halfz;

my $lens_z=$agel_posz+$agel_halfz+$lens_gap+$lens_halfz;
my $glassWindow_z;
if($lensType == 1)
{
  $glassWindow_z= $lens_z-$lens_halfz+$focalLength+$phodet_gapz+$glassWindow_halfz;
}
if($lensType == 0)
{
  $glassWindow_z= $lens_z+$lens_halfz+$focalLength+$phodet_gapz+$glassWindow_halfz;
}

my $phodet_z =$glassWindow_z+$glassWindow_halfz+$phodet_halfz;
my $metalSheet_z=$phodet_z-$phodet_halfz+$insulation-$metalSheet_halfz;
		     
my @readout_z= ($glassWindow_z-$glassWindow_halfz, $phodet_z+$phodet_halfz);
if ($build_copper) {$readout_z[1]=$metalSheet_z+$metalSheet_halfz+$readout_halfz;}

my $hollowOffset=$hollow_z+$offset;   #accumulated offset due to asymmetric detector walls (z-direction)
my @detposZ = ( $offset, $hollowOffset,$hollowOffset+$agel_posz, $hollowOffset+$lens_z, $hollowOffset+$phodet_z);

my @freslens = ( 2.0*sqrt(2.0)*$fresnelLensDiameter/8.0, 2.0*sqrt(2.0)*$fresnelLensDiameter/8.0, $fresnelLens_halfz );
my @planolens = ( $planoLens_halfx, , $planoLens_halfy, $planoLens_halfz);
my @readoutposZ = ( $hollowOffset+$readout_z[0], $hollowOffset+$readout_z[1]);
sub print_detector()
{
  my $agelThickness=2*$agel_halfz/10;
 
  print"=====================================================================\n"; 
  print'                   ',$agelThickness,' cm thick Aerogel',"\n";
  print"=====================================================================\n";
  print"Printing detector positions and sizes ...\n\n";
  
  print"hold box       position=(0.0, 0.0, $detposZ[0])mm, half size in XYZ=($box_halfx, $box_halfy, $box_halfz)mm\n";
  print"hollowVol      position=(0.0, 0.0, $detposZ[1])mm, half size in XYZ=($hollow_halfx, $hollow_halfy, $hollow_halfz)mm\n";
  print"aerogel        position=(0.0, 0.0, $detposZ[2])mm, half size in XYZ=($agel_halfx, $agel_halfy, $agel_halfz)mm\n";
  if($lensType == 1) 
  {
    print"fresnel lens   position=(0.0, 0.0, $detposZ[3])mm, half size in XYZ=($freslens[0], $freslens[1], $freslens[2])mm\n";
  }
  if($lensType == 0) 
  {
    print"plano-convex lens   position=(0.0, 0.0, $detposZ[3])mm, half size in XYZ=($planolens[0], $planolens[1], $planolens[2])mm\n";
  }
  print"photon sensor  position=(0.0, 0.0, $detposZ[4])mm, half size in XYZ=($phodet_halfx, $phodet_halfy, $phodet_halfz)mm\n";
  print'glass window   pos_z=',$hollowOffset+$glassWindow_z,'mm, half_z=',$glassWindow_halfz,'mm',"\n";
  print"readout        position=(0.0, 0.0, $readoutposZ[0] mm, and $readoutposZ[1] mm )\n";
  print"=====================================================================\n";
  print"sensor_total_halfx= $sensor_total_halfx mm\n";
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~ Define the holder Box for Detectors ~~~~~~~~~~~~~~~~~~~~~~~~~#
my $box_name = "detector_holder";
my $box_mat = "G4_Al";
#my $box_mat = "Air_Opt";
#my $box_mat = "holder_acrylic";

my $hollow_mat="Air_Opt";

sub build_box()
{
    print"building detector holder box...\n";

    my %detector=init_det();
    $detector{"name"} = "$DetectorName\_$box_name";
    $detector{"mother"} = "$DetectorMother";
    $detector{"description"} = "$DetectorName\_$box_name";
    $detector{"pos"} = "0*mm 0*mm $offset*mm";
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
    $detector{"description"} = "$DetectorName\_foamHolder_name";
    $detector{"pos"} = "0*mm 0*mm 0*mm";
    $detector{"rotation"} = "0*deg 0*deg 0*deg";
    $detector{"color"} = "00ff00";
    #$detector{"style"} = "1";
    $detector{"type"} = "Pgon";    ### Polyhedra                                                                            
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

my $agel_posx=0;
my $agel_posy=0;

sub build_aerogel()
{
    print"building aerogel block...\n";

    my %detector=init_det();
    $detector{"name"} = "$DetectorName\_$agel_name";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$DetectorName\_$agel_name";
    $detector{"pos"} = "$agel_posx*mm $agel_posy*mm $agel_posz*mm";
    $detector{"color"} = "ffa500";
    $detector{"type"} = "Box";
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

#---------- build plano convex Lens ----------#

my $planoLens_holdbox_name="$DetectorName\_planoLensHoldBox";
my $planoLens_holdbox_mat  = "Air_Opt";

sub build_plano_lens()
{
  print"building Plano Convex Lens... \n";
  print"\tPlano Convex Lens: Lens Diameter = $planoLens_D\n";
  print"\tPlano Convex Lens: Lens Center Thickness = $planoLens_CT\n";
  print"\tPlano Convex Lens: Lens Back Focal Length = $planoLens_BFL\n";
  # print"\tPlano Convex Lens: Lens Effictive Thickness = $planoLens_ET\n";
  # print"\tholdbox half thickness= ",$planoLens_holdbox_halfz,"\n";

  my $planoLens_holdbox_halfx = $planoLens_halfx;
  my $planoLens_holdbox_halfy = $planoLens_halfy;
  my $planoLens_holdbox_halfz = $planoLens_halfz;

  #----------- build holder box -----------#
  my %detector; 
  %detector=init_det();
  $detector{"name"} = "$planoLens_holdbox_name";
  $detector{"mother"} = "$DetectorName\_hollow";
  $detector{"description"} = "$planoLens_holdbox_name";
  $detector{"pos"} = "0*mm 0*mm $lens_z*mm";
  $detector{"color"} = "ff0000";
  $detector{"type"} = "Box";
  $detector{"dimensions"} = "$planoLens_holdbox_halfx*mm $planoLens_holdbox_halfy*mm $planoLens_holdbox_halfz*mm";
  $detector{"material"} = $planoLens_holdbox_mat;
  $detector{"rotation"} = "0*deg 0*deg 0*deg";
  $detector{"style"} = "0";
  $detector{"visible"} = "1";
  $detector{"sensitivity"} = "no";
  $detector{"hit_type"}    = "no";
  $detector{"identifiers"} = "no";
  print_det(\%configuration, \%detector);
  #----------- build holder box -----------#

  # #--------- build plano convex lens -----------#
  my $pos_z_planoLens_sphere = $planoLens_R - $planoLens_holdbox_halfz;
  %detector=init_det();
  $detector{"name"} = "$DetectorName\_planoLens_sphere";
  $detector{"mother"} = "$planoLens_holdbox_name";
  $detector{"description"} = "$DetectorName\_planoLens_sphere";
  $detector{"pos"} = "0*mm 0*mm $pos_z_planoLens_sphere*mm"; # w.r.t plano lens holder box
  $detector{"rotation"} = "0*deg 0*deg 0*deg";
  $detector{"color"} = "2eb7ed";
  $detector{"type"} = "Sphere";
  $detector{"dimensions"} = "0.0*mm $planoLens_R*mm 0.0*deg 360.0*deg 0.0*deg 180.0*deg";
  $detector{"material"} = "Component";
  $detector{"style"} = "0";
  $detector{"visible"} = "1";
  $detector{"sensitivity"} = "no";
  $detector{"hit_type"}    = "no";
  $detector{"identifiers"} = "no";
  print_det(\%configuration, \%detector);

  my $pos_z_planoLens_box = $pos_z_planoLens_sphere + $planoLens_ST;
  %detector=init_det();
  $detector{"name"} = "$DetectorName\_planoLens_box";
  $detector{"mother"} = "$planoLens_holdbox_name";
  $detector{"description"} = "$DetectorName\_planoLens_box";
  $detector{"pos"} = "0*mm 0*mm $pos_z_planoLens_box*mm"; # w.r.t plano lens holder box
  $detector{"rotation"} = "0*deg 0*deg 0*deg";
  $detector{"color"} = "2eb7ed";
  $detector{"type"} = "Box";
  $detector{"dimensions"} = "$planoLens_R*mm $planoLens_R*mm $planoLens_R*mm";
  $detector{"material"} = "Component";
  $detector{"style"} = "0";
  $detector{"visible"} = "0";
  $detector{"sensitivity"} = "no";
  $detector{"hit_type"}    = "no";
  $detector{"identifiers"} = "no";
  print_det(\%configuration, \%detector);

  %detector=init_det();
  $detector{"name"} = "$DetectorName\_planoLens_convex";
  $detector{"mother"} = "$planoLens_holdbox_name";
  $detector{"description"} = "$DetectorName\_planoLens_convex";
  $detector{"pos"} = "0*mm 0*mm $pos_z_planoLens_sphere*mm"; # w.r.t plano lens holder box
  $detector{"rotation"} = "0*deg 0*deg 0*deg";
  $detector{"color"} = "2eb7ed";
  $detector{"type"} = "Operation:@ $DetectorName\_planoLens_sphere - $DetectorName\_planoLens_box";
  $detector{"dimensions"} = "0";
  $detector{"material"} = "Component";
  # $detector{"material"} = "glass";
  $detector{"style"} = "1";
  $detector{"visible"} = "1";
  $detector{"sensitivity"} = "no";
  $detector{"hit_type"}    = "no";
  $detector{"identifiers"} = "no";
  print_det(\%configuration, \%detector);
  # #--------- build plano convex lens -----------#

  # #--------- build flat lens -----------#
  my $planoLens_tube_R = $planoLens_D/2.0;
  my $planoLens_tube_halfz = $planoLens_ET/2.0;
  my $pos_z_planoLens_tube =  $planoLens_ST - $planoLens_holdbox_halfz + $planoLens_tube_halfz;
  %detector=init_det();
  $detector{"name"} = "$DetectorName\_planoLens_tube";
  $detector{"mother"} = "$planoLens_holdbox_name";
  $detector{"description"} = "$DetectorName\_planoLens_tube";
  $detector{"pos"} = "0*mm 0*mm $pos_z_planoLens_tube*mm";
  $detector{"rotation"} = "0*deg 0*deg 0*deg";
  $detector{"color"} = "2eb7ed";
  $detector{"type"} = "Tube";
  $detector{"dimensions"} = "0.0*mm $planoLens_tube_R*mm $planoLens_tube_halfz*mm 0.0*deg 360.0*deg";
  $detector{"material"} = "Component";
  $detector{"style"} = "1";
  $detector{"visible"} = "1";
  $detector{"sensitivity"} = "no";
  $detector{"hit_type"}    = "no";
  $detector{"identifiers"} = "no";
  print_det(\%configuration, \%detector);
  # #--------- build flat lens -----------#

  %detector=init_det();
  $detector{"name"} = "$DetectorName\_planoLens";
  $detector{"mother"} = "$planoLens_holdbox_name";
  $detector{"description"} = "$DetectorName\_planoLens";
  $detector{"pos"} = "0*mm 0*mm $pos_z_planoLens_sphere*mm"; # w.r.t plano lens holder box
  $detector{"rotation"} = "0*deg 0*deg 0*deg";
  $detector{"color"} = "2eb7ed";
  $detector{"type"} = "Operation:@ $DetectorName\_planoLens_convex + $DetectorName\_planoLens_tube";
  $detector{"dimensions"} = "0";
  # $detector{"material"} = "glass";
  # $detector{"material"} = "$planoLens_holdbox_mat";
  $detector{"material"} = "acrylic";
  $detector{"style"} = "1";
  $detector{"visible"} = "1";
  $detector{"sensitivity"} = "no";
  $detector{"hit_type"}    = "no";
  $detector{"identifiers"} = "no";
  print_det(\%configuration, \%detector);
}

sub build_lens()
{
if($lensType == 1) { build_fresnel_lens() }
if($lensType == 0) { build_plano_lens() }
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
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ reflection mirrors ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my $mirror_mat  = "Aluminum";
sub build_mirrors()
{
    print"building mirror set...\n";

    my $mirror_thickness=2;
    my @mirror_z=($lens_z+$lens_halfz+$lens_gap,$glassWindow_z-$glassWindow_halfz);
    my @mirror_router = ( $agel_halfx+$mirror_thickness, $sensor_total_halfx+$mirror_thickness );
    my @mirror_rinner = ( $agel_halfx, $sensor_total_halfx);
    
    print"\t",'mirror: length in z direction =',$mirror_z[1]-$mirror_z[0],"\n";

    my $idManual=3;
    my %detector=init_det();
    $detector{"name"} = "$DetectorName\_mirror";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$DetectorName\_mirror";
    $detector{"pos"} = "0*mm 0*mm 0*mm";
    $detector{"color"} = "ffff00";
    $detector{"type"} = "Pgon";

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
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ readout hardware ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my $readoutdet_name = "readout";
my $readout_mat  = "Aluminum";
my @readoutdet_pos  = ( 0.0, 0.0, 0.0 );
my @readout_rinner = ( $sensor_total_halfx, $sensor_total_halfx ); 
my @readout_router = ( $sensor_total_halfx+$readout_thickness, $sensor_total_halfx+$readout_thickness );


sub build_readout()
{
    print"building readout hardware...\n";
   
    my %detector=init_det();
    $detector{"name"} = "$DetectorName\_$readoutdet_name";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$DetectorName\_$readoutdet_name";
    $detector{"pos"} = "$readoutdet_pos[0]*mm $readoutdet_pos[1]*mm $readoutdet_pos[2]*mm"; #Ping : checked
    $detector{"rotation"} = "0*deg 0*deg 0*deg";
    $detector{"color"} = "ff0000";
    $detector{"style"} = "0";
    $detector{"visible"} = "0";
    $detector{"type"} = "Pgon";    ### Polyhedra
    my $dimen = "45*deg 360*deg 4*counts 2*counts";
    for(my $i=0; $i<2; $i++) {$dimen = $dimen ." $readout_rinner[$i]*mm";}
    for(my $i=0; $i<2; $i++) {$dimen = $dimen ." $readout_router[$i]*mm";}
    for(my $i=0; $i<2; $i++) {$dimen = $dimen ." $readout_z[$i]*mm";}
    $detector{"dimensions"} = "$dimen";
    $detector{"material"} = "$readout_mat";
    $detector{"sensitivity"} = "no";
    $detector{"hit_type"}    = "no";
    $detector{"identifiers"} = "no";
    print_det(\%configuration, \%detector);
    
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Plano convex lens ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Main function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
sub build_detector()
{
    print_detector();
    print"=====================================================================\n";
    build_box();
    build_foamHolder();
    build_aerogel();
    build_lens();
    build_photondet();
    build_mirrors();
    build_readout();
    print"done\n";
    print"=====================================================================\n";
}
