
AUTOMAKE_OPTIONS = foreign

AM_LDFLAGS = -L$(libdir) `root-config --evelibs` -L${OFFLINE_MAIN}/lib -L${MYINSTALL}/lib

AM_CXXFLAGS = `root-config --cflags`

AM_CPPFLAGS = -I$(ROOTSYS)/include -I$(OFFLINE_MAIN)/include -I$(OPT_PHENIX)/include -I$(MYINSTALL)/include

dist_pkgdata_DATA = BbcRecalConsts.txt

lib_LTLIBRARIES = libRecoEventPlane.la

noinst_HEADERS = \
RecoEventPlaneLinkDef.h \
RecoEPHistoManagerLinkDef.h

##############################################
# please add new classes in alphabetical order

pkginclude_HEADERS = \
	RecoEventPlane.h \
	RecoEPHistoManager.h

##############################################
# please add new classes in alphabetical order

libRecoEventPlane_la_SOURCES = \
  RecoEventPlane_Dict.C \
  RecoEventPlane.cxx \
  RecoEPHistoManager_Dict.C \
  RecoEPHistoManager.cxx

##############################################
# libraries used in the package

libRecoEventPlane_la_LIBADD = \
  -lphool \
  -lpicodst_object

noinst_PROGRAMS = \
  testexternals

testexternals_SOURCES = \
  testexternals.C

testexternals_LDADD = \
  libRecoEventPlane.la

testexternals.C:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f *Dict* $(BUILT_SOURCES)

RecoEventPlane_Dict.C: \
	RecoEventPlane.h \
	RecoEventPlaneLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

RecoEPHistoManager_Dict.C: \
	RecoEPHistoManager.h \
	RecoEPHistoManagerLinkDef.h
	  rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^
