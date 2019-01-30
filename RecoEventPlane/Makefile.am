
AUTOMAKE_OPTIONS = foreign

# List of shared libraries to produce

lib_LTLIBRARIES = libRecoEventPlane.la

AM_LDFLAGS = -L$(libdir) `root-config --evelibs` -L${OFFLINE_MAIN}/lib

AM_CXXFLAGS = `root-config --cflags`

INCLUDES = -I$(ROOTSYS)/include -I$(OFFLINE_MAIN)/include -I$(OPT_PHENIX)/include

##############################################
# please add new classes in alphabetical order

include_HEADERS = \
	RecoEventPlane.h

##############################################
# please add new classes in alphabetical order

libRecoEventPlane_la_SOURCES = \
  RecoEventPlaneDict.C \
  RecoEventPlane.cxx

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

RecoEventPlaneDict.C: \
	RecoEventPlane.h \
  RecoEventPlaneLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^
