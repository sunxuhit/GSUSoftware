
AUTOMAKE_OPTIONS = foreign

AM_LDFLAGS = -L$(libdir) `root-config --evelibs` -L${OFFLINE_MAIN}/lib -L${MYINSTALL}/lib

AM_CXXFLAGS = `root-config --cflags`

AM_CPPFLAGS = -I$(ROOTSYS)/include -I$(OFFLINE_MAIN)/include -I$(OPT_PHENIX)/include -I$(MYINSTALL)/include

dist_pkgdata_DATA = \
runIndex.txt

lib_LTLIBRARIES = libPhVecMesonAna.la

noinst_HEADERS = \
PhDiMuonCutLinkDef.h \
PhDiMuonEventLinkDef.h \
PhDiMuonHistoManagerLinkDef.h \
PhDiMuonUtilityLinkDef.h \
PhVecMesonAnaLinkDef.h

##############################################
# please add new classes in alphabetical order
pkginclude_HEADERS = \
	PhDiMuonCut.h \
	PhDiMuonEvent.h \
	PhDiMuonHistoManager.h \
	PhDiMuonUtility.h \
	PhVecMesonAna.h

##############################################
# please add new classes in alphabetical order
libPhVecMesonAna_la_SOURCES = \
  PhDiMuonCut_Dict.C \
  PhDiMuonCut.cxx \
  PhDiMuonEvent_Dict.C \
  PhDiMuonEvent.cxx \
  PhDiMuonHistoManager_Dict.C \
  PhDiMuonHistoManager.cxx \
  PhDiMuonUtility_Dict.C \
  PhDiMuonUtility.cxx \
  PhVecMesonAna_Dict.C \
  PhVecMesonAna.cxx

##############################################
# libraries used in the package

libPhVecMesonAna_la_LIBADD = \
  -L$(libdir) \
  -L$(OFFLINE_MAIN)/lib \
  -lTOAD

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = \
  testexternals.C

testexternals_LDADD = \
  libPhVecMesonAna.la

testexternals.C:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f *Dict* $(BUILT_SOURCES)

PhDiMuonCut_Dict.C: \
	PhVecMesonCons.h \
	PhDiMuonCut.h \
	PhDiMuonCutLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

PhDiMuonEvent_Dict.C: \
	PhDiMuonEvent.h \
	PhDiMuonEventLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

PhDiMuonHistoManager_Dict.C: \
	PhVecMesonCons.h \
	PhDiMuonHistoManager.h \
	PhDiMuonHistoManagerLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

PhDiMuonUtility_Dict.C: \
	Run14AuAuList.h \
	PhVecMesonCons.h \
	PhDiMuonUtility.h \
	PhDiMuonUtilityLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

PhVecMesonAna_Dict.C: \
	PhVecMesonAna.h \
	PhVecMesonAnaLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

