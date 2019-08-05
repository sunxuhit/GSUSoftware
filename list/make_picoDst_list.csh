#!/bin/csh
touch picoDst_Run14AuAu200GeV.list
cd /gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/taxi/Run14AuAu200MuonsMBPro109/picoDst/data/
ls -d ${PWD}/*.root >! /direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/list/picoDst_Run14AuAu200GeV.list
cd -
