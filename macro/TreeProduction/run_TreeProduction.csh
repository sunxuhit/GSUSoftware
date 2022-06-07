#!/bin/csh
setenv HOME /phenix/u/$LOGNAME
source /etc/csh.login
foreach i (/etc/profile.d/*.csh)
  source $i
end
source $HOME/.login
source /opt/phenix/bin/phenix_setup.csh ana
setenv TSEARCHPATH $HOME/WorkSpace/PHENIX/install/

printenv

set numOfEvent = $1
echo set number of events to $numOfEvent

set mode = $2
echo set ANA_Mode to $mode 

set jobId = $3
echo set JOBID to $jobId

root.exe -q -b Fun4All_PhVecMesonMaker.C\($numOfEvent,$mode,$jobId\)

echo all done
