
AUTOMAKE_OPTIONS = foreign

AM_LDFLAGS = -L$(libdir) `root-config --evelibs` -L${OFFLINE_MAIN}/lib -L${MYINSTALL}/lib

AM_CXXFLAGS = `root-config --cflags`

AM_CPPFLAGS = -I$(ROOTSYS)/include -I$(OFFLINE_MAIN)/include -I$(OPT_PHENIX)/include -I$(MYINSTALL)/include

dist_pkgdata_DATA = \
runIndex.txt

lib_LTLIBRARIES = libPhVecMesonAna.la

noinst_HEADERS = \
PhVecMesonAnaLinkDef.h \
PhDiMuonEventLinkDef.h \
PhDiMuonUtilityLinkDef.h

##############################################
# please add new classes in alphabetical order

pkginclude_HEADERS = \
	PhVecMesonAna.h \
	PhDiMuonEvent.h \
	PhDiMuonUtility.h

##############################################
# please add new classes in alphabetical order

libPhVecMesonAna_la_SOURCES = \
  PhVecMesonAna_Dict.C \
  PhVecMesonAna.cxx \
  PhDiMuonEvent_Dict.C \
  PhDiMuonEvent.cxx \
  PhDiMuonUtility_Dict.C \
  PhDiMuonUtility.cxx

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

PhVecMesonAna_Dict.C: \
	PhVecMesonAna.h \
	PhVecMesonAnaLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

PhDiMuonEvent_Dict.C: \
	PhDiMuonEvent.h \
	PhDiMuonEventLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

PhDiMuonUtility_Dict.C: \
	Run14AuAuList.h \
	PhVecMesonCons.h \
	PhDiMuonUtility.h \
	PhDiMuonUtilityLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

