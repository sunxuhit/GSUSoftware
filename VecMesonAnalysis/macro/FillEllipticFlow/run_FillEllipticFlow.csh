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

set pid = $1
echo set pid to $pid

set list = $2
echo set list numer to $list

set start_event = $3
echo set start_event to $start_event

set stop_event = $4
echo set stop_event to $stop_event

set mode = $5
echo set mode to $mode

root.exe -q -b PhVecMesonAna.C\($pid,$list,$start_event,$stop_event,$mode\)

echo all done
