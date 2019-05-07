
AUTOMAKE_OPTIONS = foreign

AM_LDFLAGS = -L$(libdir) `root-config --evelibs` -L${OFFLINE_MAIN}/lib -L${MYINSTALL}/lib

AM_CXXFLAGS = `root-config --cflags`

AM_CPPFLAGS = -I$(ROOTSYS)/include -I$(OFFLINE_MAIN)/include -I$(OPT_PHENIX)/include -I$(MYINSTALL)/include

dist_pkgdata_DATA = \
runIndex.txt \
BbcRecalConsts.txt \
BbcPhiWeightCorrection.txt \
file_AuAu200GeV_ReCenterPar.root \
file_AuAu200GeV_ShiftPar.root

lib_LTLIBRARIES = libPhVecMesonMaker.la

noinst_HEADERS = \
PhVecMesonMakerLinkDef.h \
RecoEPHistoManagerLinkDef.h \
RecoEPProManagerLinkDef.h \
RecoEPUtilityLinkDef.h \
RecoEventPlaneLinkDef.h

##############################################
# please add new classes in alphabetical order

pkginclude_HEADERS = \
	PhVecMesonMaker.h \
	RecoEPHistoManager.h \
	RecoEPProManager.h \
	Run14AuAuList.h \
	PhVecMesonCons.h \
	RecoEPUtility.h \
	RecoEventPlane.h

##############################################
# please add new classes in alphabetical order

libPhVecMesonMaker_la_SOURCES = \
  PhVecMesonMaker_Dict.C \
  PhVecMesonMaker.cxx \
  RecoEPHistoManager_Dict.C \
  RecoEPHistoManager.cxx \
  RecoEPProManager_Dict.C \
  RecoEPProManager.cxx \
  RecoEPUtility_Dict.C \
  RecoEPUtility.cxx \
  RecoEventPlane_Dict.C \
  RecoEventPlane.cxx

##############################################
# libraries used in the package

libPhVecMesonMaker_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lfun4all \
  -lSubsysReco \
  -lfun4allfuncs \
  -lert \
  -lrecal \
  -lTOAD \
  -lphool

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = \
  testexternals.C

testexternals_LDADD = \
  libPhVecMesonMaker.la

testexternals.C:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f *Dict* $(BUILT_SOURCES)

PhVecMesonMaker_Dict.C: \
	PhVecMesonMaker.h \
	PhVecMesonMakerLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

RecoEPHistoManager_Dict.C: \
	PhVecMesonCons.h \
	RecoEPHistoManager.h \
	RecoEPHistoManagerLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

RecoEPProManager_Dict.C: \
	PhVecMesonCons.h \
	RecoEPProManager.h \
	RecoEPProManagerLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

RecoEPUtility_Dict.C: \
	Run14AuAuList.h \
	PhVecMesonCons.h \
	RecoEPUtility.h \
	RecoEPUtilityLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

RecoEventPlane_Dict.C: \
	PhVecMesonCons.h \
	RecoEventPlane.h \
	RecoEventPlaneLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

