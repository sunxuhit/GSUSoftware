#!/bin/csh -f
if ( $#argv != 1 ) then
echo "enter script name!"
exit
endif

set script = $1
set out =  ` pwd `

echo "script = $script"

set pid = 0 # 0 for J/Psi | 1 for phi
set jobId = 0
set mode = 1 # 0 for US | 1 for LS | 2 for ME
set start_event = 0
# set stop_event = 10000000
set stop_event = -1 # < 0 for all events

set numOfList = 26 # total number of list => 26
# set numOfList = 2 # test

while ( $jobId < $numOfList )

cp example.job ${out}/tmp/Yields_Pid_${pid}_EventType_${mode}_$jobId.job

echo "Executable     = "${script} >>${out}/tmp/Yields_Pid_${pid}_EventType_${mode}_$jobId.job
echo "Output         = /gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/Log/Yields_Pid_${pid}_EventType_${mode}_$jobId.out" >>${out}/tmp/Yields_Pid_${pid}_EventType_${mode}_$jobId.job
echo "Error          = /gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/Log/Yields_Pid_${pid}_EventType_${mode}_$jobId.err" >>${out}/tmp/Yields_Pid_${pid}_EventType_${mode}_$jobId.job
echo "Log            = /gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/Log/Yields_Pid_${pid}_EventType_${mode}_$jobId.log" >>${out}/tmp/Yields_Pid_${pid}_EventType_${mode}_$jobId.job
echo "Arguments      = $pid $jobId $start_event $stop_event $mode" >>${out}/tmp/Yields_Pid_${pid}_EventType_${mode}_$jobId.job
echo "Queue"        >>${out}/tmp/Yields_Pid_${pid}_EventType_${mode}_$jobId.job
echo "     "        >>${out}/tmp/Yields_Pid_${pid}_EventType_${mode}_$jobId.job

echo "submitting: Yields_Pid_${pid}_EventType_${mode}_$jobId"
condor_submit ${out}/tmp/Yields_Pid_${pid}_EventType_${mode}_$jobId.job

@ jobId++

end
