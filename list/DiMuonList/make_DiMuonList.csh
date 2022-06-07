#!/bin/csh
rm DiMuon_Run14AuAu200GeV.list
touch DiMuon_Run14AuAu200GeV.list
cd /gpfs/mnt/gpfs02/phenix/plhf/plhf1/xusun/condor/Run14AuAu200MuonsMBPro109/DiMuonTree 
ls -d *.root >! /direct/phenix+u/xusun/WorkSpace/PHENIX/VecMesonAnalysis/list/DiMuonList/DiMuon_Run14AuAu200GeV.list
cd -
