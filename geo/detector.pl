#############################################################
#############################################################
#
#  Define the geometry for the EIC RICH detector
#  
#  All dimensions are in mm
#  Use the hit type eic_rich 
#  
#  The geometry are divided in 5 parts: 
#  Aerogel, Fresnel lens, Mirrors, Photonsensor, and Readout
#
#############################################################
#############################################################
#  Ping 11-11-2015
#  Changed detector dimension to fit the 3cm Agel
#############################################################
# Ping 02-04-2016
# flip the orientation of the Fresnel Lens.
# The grooves of the Fresnel lens are now facing the aerogel,
# the smooth side of the lens is facing the photon sensor.
#############################################################
#############################################################
# Ping 02-13-2016
# 1. change focal length & curvature to match Edmund Optics 
#    Fresnel lens #32-683
# 2. make corner flat (polycone) if inner radius of groove
#    greater or equal to effective radius of Fresnel lens
#############################################################
# Ping 02-17-2015
# modify detector hold box to a acrylic box
#############################################################
# Ping 05-10-2016
# 1. change # of grooves of Fresnel lens from 100 to 200
# 2. change dimension of photon sensor to fit beam test 2016
#    prototype
#############################################################
# Ping 05-21-2016
# Lens modification to match beam test 2016 prototype
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

my $BoxDelz = -2.0;   #gap ?
#========================================#
#---------------  Aerogel ---------------#
my $agel_halfx = 55.25;
my $agel_halfy = $agel_halfx;
my $agel_halfz = 16.5; #3.3cm thick agel
my $agel_maxz =35;
#========================================#
#---- Fresnel lens dimension and info ---#
my $lens_z = -25.0;      #Ping: fix the value of lens_z

my $lens_halfz = 3.0;    #redundent? lens holder halfz?
my $lens_halfx=66.675;   #beam test 2016
my $lens_halfy=$lens_halfx;
my $LensDiameter = 2.0*sqrt(2.0)*$lens_halfx;

my $focalLength=76.2;         #Edmund Optics stock#32-593 
my $LensEffDiameter =152.4;   #effective diameter in mm. Edmund Optics stock#32-593
my $grooveDensity=100/25.4;   #100 grooves per inch. converted to grooves per mm. Edmund Optics stock#32-683 &#32-593
my $halfThickness=1.02;       #type in manually after configuration, and then recongfig
#========================================#
#------------ Photon Sensor -------------#
my $glassWindow_halfx= 52;
my $glassWindow_halfy= $glassWindow_halfx;
my $glassWindow_halfz= 0.75;  #glass window thickness=1.5mm
my $glassWindow_z= $lens_z+$focalLength-$glassWindow_halfz;

my $phodet_halfx = 48.0;      #1/2 effective area of Hamamatsu HA12700a
my $phodet_halfy = 48.0;
my $phodet_halfz = 1.0;
#my $phodet_z = $lens_z+$focalLength-$phodet_halfz;
my $phodet_z =$glassWindow_z+$glassWindow_halfz+$phodet_halfz;
#========================================#
#---------- Readout electronics ---------#
my $readout_halfz = 4.0;
my @readout_z = ($phodet_z-$phodet_halfz, $phodet_z-$phodet_halfz+2.0*$readout_halfz+$BoxDelz); #modified by Ping
#========================================#
#------------- Detector box -------------#
my @all_halfx=($agel_halfx,$lens_halfx,$phodet_halfx); # to find the max. halfx                                                     
my $box_thickness=(3/8)*25.4;   #3/8 inches convert to mm

my $box_halfx = max(@all_halfx) + $box_thickness+1.0;
my $box_halfy=$box_halfx;
my $box_halfz = (-1.0*$lens_z+2.0*$lens_halfz+$readout_z[1]+2*$readout_halfz+$agel_maxz )/2.0+$box_thickness;

my $offset = $box_halfz+50;     #detector box pos_z

my $hollow_halfx=$box_halfx-$box_thickness;
my $hollow_halfy=$box_halfy-$box_thickness;
my $hollow_halfz=$box_halfz-$box_thickness;
#========================================#

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Print detector information  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my @detposZ = ( $offset, $lens_z-$agel_halfz-1.5+$BoxDelz+$offset, $lens_z+$BoxDelz+$offset, $phodet_z+$offset );
my @freslens = ( 2.0*sqrt(2.0)*$LensDiameter/8.0, 2.0*sqrt(2.0)*$LensDiameter/8.0 );
my @readoutposZ = ( $readout_z[0]+$offset, $readout_z[1]+$offset );
sub print_detector()
{
  my $agelThickness=2*$agel_halfz/10;
  
  print "========================== $agelThickness cm thick Aerogel =========================\n";
  print "Printing detector positions and sizes ...\n\n";
  
  print "hold box position: ( 0.0, 0.0, $detposZ[0] mm),  half size in XYZ: ( $box_halfx mm, $box_halfy mm, $box_halfz mm )\n";
  print "aerogel position: ( 0.0, 0.0, $detposZ[1] mm),  half size in XYZ: ( $agel_halfx mm, $agel_halfy mm, $agel_halfz mm )\n";
  print "fresnel lens position: ( 0.0, 0.0, $detposZ[2] mm), half  size in XY: ( $freslens[0] mm, $freslens[1] mm )\n";
  print "photon detector position: ( 1.0, 0.0, $detposZ[3] mm), half size in XYZ: ($phodet_halfx mm, $phodet_halfy mm, $phodet_halfz mm )\n";
  print "readout position: ( 0.0, 0.0, $readoutposZ[0] mm, and $readoutposZ[1] mm )\n";
  print "=====================================================================\n";
  
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~ Define the holder Box for Detectors ~~~~~~~~~~~~~~~~~~~~~~~~~#
my $box_name = "detector_holder";
#my $box_mat = "G4_AIR";
#my $box_mat = "Air_Opt";
my $box_mat = "holder_acrylic";

my $hollow_mat="Air_Opt";

sub build_box()
{
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
    $detector{"pos"} = "0*mm 0*mm 0*mm";   #w.r.t. detector
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
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Aerogel ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my $agel_name = "Aerogel";
my $agel_mat  = "aerogel";
#my $agel_mat  = "RichAerogel3";

my $agel_posx=0;
my $agel_posy=0;
my $agel_posz=$lens_z - $agel_halfz + $BoxDelz;

sub build_aerogel()
{
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
#my $lens_mat  = "holder_acrylic"; #testing
my $lens_numOfGrooves = floor($grooveDensity*($LensEffDiameter/2));
my $max_numOfGrooves=1000;

print"Fresnel Lens: num. of grooves = $lens_numOfGrooves\n";
print"Fresnel Lens: Lens diameter = $LensDiameter\n";
print"Fresnel Lens: effective diameter = $LensEffDiameter mm \n";
#========================================#
#---------- build Fresnel Lens ----------#
sub build_lens()
{
    my $count=0;
    #========================================#
    #---- Properites of the fresnel lens ----#
    my $GrooveWidth=1/$grooveDensity;
    if($GrooveWidth<=0) { print "build_lens::GrooveWidth <= 0\n" }
    else {print"Fresnel Lens: groove width= $GrooveWidth\n";}
    
    #inner & outer radius of the groove at the adge of the lens
    my $Rmin1 = ($lens_numOfGrooves-1)*$GrooveWidth;
    my $Rmax1 = ($lens_numOfGrooves-0)*$GrooveWidth;
    my $LensThickness = GetSagita($Rmax1)-GetSagita($Rmin1)+0.06*25.4; #dZ + center thickness
    print"Fresnel Lens: Lens thickness = $LensThickness\n";
    #========================================#
    #----- holder box for fresnel lens ------#
    my $lens_holdbox_halfz=$LensThickness/2.0+0.25;
    print "holdbox half thickness= ",$lens_holdbox_halfz,"\n";
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
    $detector{"dimensions"} = "$lens_halfx*mm $lens_halfy*mm $lens_holdbox_halfz*mm";
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

	if ($iRmax1>$LensDiameter/2.0) { last; }    #if iRmin1>Lens radius, outside the lens, break
	#--------------------------------------------------------#
	#phi angle (from Ping)
	my $phi1;
	my $phi2;
	my $deltaPhi;
	
	if ($iRmax1<$lens_halfx) {   #1/4 of a full circle
	    $phi1=0;                 #in degree
	    $deltaPhi=360;           #in degree
	}
	else {
	    $phi1=acos($lens_halfx/$iRmax1)*180/pi;   #in degree
	    $phi2=asin($lens_halfy/$iRmax1)*180/pi;   #in degree
	    $deltaPhi=$phi2-$phi1;
	    #if ($iholdbox==0) {print "this is ", $igroove, "th grooves, phi1=",$phi1,", delPhi=",$deltaPhi,"\n";}
	}
	#--------------------------------------------------------#
	#grooves profile
	my $dZ=0.06*25.4;                     #center thickness=0.06 inches
	my @lens_poly_z;
	my $numOfLayer;

	if ($iRmin1<$LensEffDiameter/2.0) {   #if iRmin>=effective radius, dZ=0, i.e. flat
	    $numOfLayer=3;
	    $dZ = GetSagita($iRmax1) - GetSagita($iRmin1);
	    if($dZ<=0) { print "build_lens::Groove depth<0 !\n"; }
	    @lens_poly_z    = (-1*$LensThickness/2.0, $LensThickness/2.0-$dZ, $LensThickness/2.0);
	}
	else {
	    $numOfLayer=2;
	    @lens_poly_z    = (-1*$LensThickness/2.0, -1*$LensThickness/2.0+$dZ);
	}
	#--------------------------------------------------------#
	my $repeat=1;
	my $draw=1;
	my $grooveName="$lens_holdbox_name\_groove$igroove";
	
	if ($iRmax1>=$lens_halfx) { $repeat=4; }   #4 edges
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
#------- arc shape, Aspheric lens -------#
sub GetSagita
{
  my $Conic = -1.0;		      #original
  #my $lens_type = 3;
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
  if  ($lens_type == 5) {              #Ping: curvature=1/(focalLength*(n-1))
      #$Curvature=0.0267;
      #$Curvature=0.0287;   
      $Curvature=1/($focalLength*($n-1));
  }
  
  my $TotAspher = 0.0;
  for(my $k=1;$k<9;$k++){ $TotAspher += $Aspher[$k-1]*($_[0]**(2*$k)); }
  
  my $ArgSqrt = 1.0-(1.0+$Conic)*($Curvature**2)*($_[0]**2);
  if ($ArgSqrt < 0.0){ print "build_lens::Sagita: Square Root of <0 ! \n"; }
  
  my $Sagita_value = $Curvature*($_[0]**2)/(1.0+sqrt($ArgSqrt)) + $TotAspher;
  return $Sagita_value ;
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ photon detector ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my $photondet_name = "Photondet";
#my $photondet_mat  = "Aluminum";
my $photondet_mat  = "Air_Opt";
sub build_photondet()
{
    #my @photondet_pos  = ( 0.0, 0.0, $phodet_z );
    #my @photondet_size = ( $phodet_halfx, $phodet_halfy, $phodet_halfz );
    #print 'Photon Sensor: z position from ', $photondet_pos[2]-$photondet_size[2],' to ',$photondet_pos[2]+$photondet_size[2],'\n';

    #========================================#
    #--------- build glass window -----------#
    my %detector=init_det();
    $detector{"name"} = "$DetectorName\_glassWindow";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$DetectorName\_$photondet_name";
    $detector{"pos"} = "0*mm 0*mm $glassWindow_z*mm";
    $detector{"rotation"} = "0*deg 0*deg 0*deg";
    $detector{"color"} = "1ABC9C";
    $detector{"type"} = "Box";
    $detector{"dimensions"} = "$glassWindow_halfx*mm $glassWindow_halfy*mm $glassWindow_halfz*mm";
    $detector{"material"} = "glass";
    $detector{"mfield"} = "no";
    $detector{"sensitivity"} = "no";
    $detector{"hit_type"}    = "no";
    $detector{"identifiers"} = "no";
    print_det(\%configuration, \%detector);

    #========================================#                                                                                                    #------------ build sensor --------------#
    %detector=init_det();
    $detector{"name"} = "$DetectorName\_$photondet_name";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$DetectorName\_$photondet_name";
    $detector{"pos"} = "0*mm 0*mm $phodet_z*mm";
    $detector{"rotation"} = "0*deg 0*deg 0*deg";
    $detector{"color"} = "0000A0";
    $detector{"type"} = "Box";
    $detector{"dimensions"} = "$phodet_halfx*mm $phodet_halfy*mm $phodet_halfz*mm";
    $detector{"material"} = "$photondet_mat";
    $detector{"mfield"} = "no";
    $detector{"sensitivity"} = "$hittype";
    $detector{"hit_type"}    = "$hittype";
    $detector{"identifiers"} = "id manual 2";
    print_det(\%configuration, \%detector);
    
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ reflection mirrors ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my $mirror_mat  = "Aluminum";
sub build_mirrors()
{
    my $dx1 = $agel_halfx;          # modified by Ping
    #my $dx1 = $lens_halfx;
    #my $dx2 =$phodet_halfx;
    my $dx2 =$glassWindow_halfx;

    my $dy1 = 0.1;
    my $dy2 = 0.1;
    #my $dz = ($phodet_z - $lens_z - $phodet_halfz - 3.0)/2.0;
    my $dz = ($glassWindow_z - $lens_z - $glassWindow_halfz - 3.0)/2.0;
    #my $phi = atan2($agel_halfx-$phodet_halfx, 2.0*$dz);
    my $phi = atan2($agel_halfx-$glassWindow_halfx, 2.0*$dz);
    #my $phi = atan2($lens_halfx-$phodet_halfx, 2.0*$dz);
    my $delxy = $dz*sin($phi) + 1.0;
    #my $dz_update = sqrt( $dz**2 + ($agel_halfx-$phodet_halfx)**2 );
    #my $dz_update = sqrt( $dz**2 + ($lens_halfx-$phodet_halfx)**2 );
    my $dz_update = sqrt( $dz**2 + ($agel_halfx-$glassWindow_halfx)**2 );
    
    my $mirror_halfx = $agel_halfx;
    #my $mirror_halfx = $lens_halfx;
    my $mirror_halfy = 1.0;
    #my $mirror_halfz = ($phodet_z-$lens_z-$phodet_halfz-1.0)/2.0;
    my $mirror_halfz = ($glassWindow_z-$lens_z-$glassWindow_halfz-1.0)/2.0;

    ####### back mirror
    my $phi_back = $phi*180.0/pi;
    #my @mirror_back_pos  = ( $agel_halfy+$mirror_halfy-$delxy, 0.0, ($lens_z+$lens_halfz+($phodet_z-$phodet_halfz))/2.0 ); #Ping: checked
    my @mirror_back_pos  = ( $agel_halfy+$mirror_halfy-$delxy, 0.0, ($lens_z+$lens_halfz+($glassWindow_z-$glassWindow_halfz))/2.0 );
    #my @mirror_back_pos  = ( $lens_halfy+$mirror_halfy-$delxy, 0.0, ($lens_z+$lens_halfz+($phodet_z-$phodet_halfz))/2.0 );
    my $mirror_back_name = "mirror_back";
    my %detector=init_det();
    $detector{"name"} = "$DetectorName\_$mirror_back_name";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$DetectorName\_$mirror_back_name";
    $detector{"pos"} = "$mirror_back_pos[0]*mm $mirror_back_pos[1]*mm $mirror_back_pos[2]*mm"; #Ping: checked
    $detector{"rotation"} = "0.0*deg $phi_back*deg 90*deg";
    $detector{"color"} = "ffff00";
    $detector{"type"} = "Trd";
    $detector{"dimensions"} = "$dx1*mm $dx2*mm $dy1*mm $dy2*mm $dz_update*mm";
    $detector{"material"} = "$mirror_mat";
    $detector{"sensitivity"} = "mirror: rich_mirrors";
    $detector{"hit_type"} = "no";
    $detector{"identifiers"} = "id manual 3";
    print_det(\%configuration, \%detector);
    
    
    ####### front mirror
    my $phi_front = -1.0*$phi*180.0/pi;
    #my @mirror_front_pos  = ( -1.*($agel_halfy+$mirror_halfy-$delxy), 0.0, ($lens_z+$lens_halfz+($phodet_z-$phodet_halfz))/2.0); #modified by Ping
    my @mirror_front_pos  = ( -1.*($agel_halfy+$mirror_halfy-$delxy), 0.0, ($lens_z+$lens_halfz+($glassWindow_z-$glassWindow_halfz))/2.0);
    #my @mirror_front_pos  = ( -1.*($lens_halfx+$mirror_halfy-$delxy), 0.0, ($lens_z+$lens_halfz+($phodet_z-$phodet_halfz))/2.0);
    my $mirror_front_name = "mirror_front";
    %detector=init_det();
    $detector{"name"} = "$DetectorName\_$mirror_front_name";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$DetectorName\_$mirror_front_name";
    $detector{"pos"} = "$mirror_front_pos[0]*mm $mirror_front_pos[1]*mm $mirror_front_pos[2]*mm";
    $detector{"rotation"} = "0*deg $phi_front*deg 90*deg";
    $detector{"color"} = "ffff00";
    $detector{"type"} = "Trd";
    $detector{"dimensions"} = "$dx1*mm $dx2*mm $dy1*mm $dy2*mm $dz_update*mm";
    $detector{"material"} = "$mirror_mat";
    $detector{"sensitivity"} = "mirror: rich_mirrors";
    $detector{"hit_type"} = "no";
    $detector{"identifiers"} = "id manual 4";
    print_det(\%configuration, \%detector);
    
    ####### top mirror
    my $phi_top = -1.0*$phi*180.0/pi;
    #my @mirror_top_pos  = ( 0.0, $agel_halfy+$mirror_halfy-$delxy, ($lens_z+$lens_halfz+($phodet_z-$phodet_halfz))/2.0 ); #modified by Ping
    my @mirror_top_pos  = ( 0.0, $agel_halfy+$mirror_halfy-$delxy, ($lens_z+$lens_halfz+($glassWindow_z-$glassWindow_halfz))/2.0 );
    #my @mirror_top_pos  = ( 0.0, $lens_halfx+$mirror_halfy-$delxy, ($lens_z+$lens_halfz+($phodet_z-$phodet_halfz))/2.0 );
    my $mirror_top_name = "mirror_top";
    %detector=init_det();
    $detector{"name"} = "$DetectorName\_$mirror_top_name";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$DetectorName\_$mirror_top_name";
    $detector{"pos"} = "$mirror_top_pos[0]*mm $mirror_top_pos[1]*mm $mirror_top_pos[2]*mm";
    $detector{"rotation"} = "$phi_top*deg 0*deg 0*deg";
    $detector{"color"} = "ffff00";
    $detector{"type"} = "Trd";
    $detector{"dimensions"} = "$dx1*mm $dx2*mm $dy1*mm $dy2*mm $dz_update*mm";
    $detector{"material"} = "$mirror_mat";
    $detector{"sensitivity"} = "mirror: rich_mirrors";
    $detector{"hit_type"} = "no";
    $detector{"identifiers"} = "id manual 5";
    print_det(\%configuration, \%detector);

    ####### bottom mirror
    my $phi_bottom = $phi*180.0/pi;
    #my @mirror_bottom_pos  = ( 0.0, -1.0*($agel_halfy+$mirror_halfy-$delxy), ($lens_z+$lens_halfz+($phodet_z-$phodet_halfz))/2.0 ); #modified by Ping
    my @mirror_bottom_pos  = ( 0.0, -1.0*($agel_halfy+$mirror_halfy-$delxy), ($lens_z+$lens_halfz+($glassWindow_z-$glassWindow_halfz))/2.0 );
    #my @mirror_bottom_pos  = ( 0.0, -1.0*($lens_halfx+$mirror_halfy-$delxy), ($lens_z+$lens_halfz+($phodet_z-$phodet_halfz))/2.0 );
    my $mirror_bottom_name = "mirror_bottom";
    %detector=init_det();
    $detector{"name"} = "$DetectorName\_$mirror_bottom_name";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$DetectorName\_$mirror_bottom_name";
    $detector{"pos"} = "$mirror_bottom_pos[0]*mm $mirror_bottom_pos[1]*mm $mirror_bottom_pos[2]*mm";
    $detector{"rotation"} = "$phi_bottom*deg 0*deg 0*deg";
    $detector{"color"} = "ffff00";
    $detector{"type"} = "Trd";
    $detector{"dimensions"} = "$dx1*mm $dx2*mm $dy1*mm $dy2*mm $dz_update*mm";
    $detector{"material"} = "$mirror_mat";
    $detector{"sensitivity"} = "mirror: rich_mirrors";
    $detector{"hit_type"} = "no";
    $detector{"identifiers"} = "id manual 6";
    print_det(\%configuration, \%detector);
}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ readout hardware ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
my $readoutdet_name = "readout";
my $readout_mat  = "Aluminum";
my @readoutdet_pos  = ( 0.0, 0.0, 0.0 );
my @readout_rinner = ( $phodet_halfx+1, $phodet_halfx+1 );
my @readout_router = ( $agel_halfx, $agel_halfy );

sub build_readout()
{
    my %detector=init_det();
    $detector{"name"} = "$DetectorName\_$readoutdet_name";
    $detector{"mother"} = "$DetectorName\_hollow";
    $detector{"description"} = "$DetectorName\_$readoutdet_name";
    $detector{"pos"} = "$readoutdet_pos[0]*mm $readoutdet_pos[1]*mm $readoutdet_pos[2]*mm"; #Ping : checked
    $detector{"rotation"} = "0*deg 0*deg 0*deg";
    $detector{"color"} = "ff0000";
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
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Main function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
sub build_detector()
{
    print_detector();
    build_box();
    build_aerogel();
    build_lens();
    build_photondet();
    build_mirrors();
    #build_readout();
}
