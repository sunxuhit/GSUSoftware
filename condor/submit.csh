#!/bin/csh -f
if ( $#argv != 4 ) then
echo "enter script name, number of events per job, analysis mode, and number of jobs!"
exit
endif

set script = $1
set out =  ` pwd `

echo "script = $script"

set numOfEvent = $2

set mode = $3

set jobId = 0

while ( $jobId < $4 )

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
