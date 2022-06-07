#!/bin/csh -f
if ( $#argv != 2 ) then
echo "enter script name, number of events per job (-1 for all events)!"
exit
endif

set script = $1
set out =  ` pwd `

echo "script = $script"

set numOfEvent = $2
set mode = 0 # 0 for event plane resolution | 1 for di-muon TTree
set jobId = 0

set numOfJobs = 1023 # total number of runs => 1023
# set numOfJobs = 23 # total number of runs => 1023

while ( $jobId < $numOfJobs )

cp example.job ${out}/tmp/${script}_AnaMode${mode}_$jobId.job

echo "Executable     = "${script} >>${out}/tmp/${script}_AnaMode${mode}_$jobId.job
echo "Output         = /gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/Log/${script}_AnaMode${mode}_$jobId.out" >>${out}/tmp/${script}_AnaMode${mode}_$jobId.job
echo "Error          = /gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/Log/${script}_AnaMode${mode}_$jobId.err" >>${out}/tmp/${script}_AnaMode${mode}_$jobId.job
echo "Log            = /gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/Log/${script}_AnaMode${mode}_$jobId.log" >>${out}/tmp/${script}_AnaMode${mode}_$jobId.job
echo "Arguments      = $numOfEvent $mode $jobId" >>${out}/tmp/${script}_AnaMode${mode}_$jobId.job
echo "Queue"        >>${out}/tmp/${script}_AnaMode${mode}_$jobId.job
echo "     "        >>${out}/tmp/${script}_AnaMode${mode}_$jobId.job

echo "submitting: ${script}_AnaMode${mode}_$jobId"
condor_submit ${out}/tmp/${script}_AnaMode${mode}_$jobId.job

@ jobId++

end
