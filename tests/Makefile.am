PANDARESAMPLER_LIBS = $(top_builddir)/lib/libpandaresampler.la

AM_CXXFLAGS = -I$(top_srcdir)/lib

noinst_PROGRAMS = testresampler testsimple testheaderonly1 testheaderonly2 \
		  testdistort testdownmulti testmultiperf testmultidelay

testresampler_SOURCES = testresampler.cc
testresampler_LDADD = $(PANDARESAMPLER_LIBS) $(FFTW_LIBS)

testsimple_SOURCES = testsimple.cc
testsimple_LDADD = $(PANDARESAMPLER_LIBS)

testdistort_SOURCES = testdistort.cc
testdistort_LDADD = $(PANDARESAMPLER_LIBS)

testdownmulti_SOURCES = testdownmulti.cc
testdownmulti_LDADD = $(PANDARESAMPLER_LIBS) $(FFTW_LIBS)

testmultiperf_SOURCES = testmultiperf.cc
testmultiperf_LDADD = $(PANDARESAMPLER_LIBS)

testmultidelay_SOURCES = testmultidelay.cc
testmultidelay_LDADD = $(PANDARESAMPLER_LIBS)

testheaderonly1_SOURCES = testheaderonly1.cc

testheaderonly2_SOURCES = testheaderonly1.cc testheaderonly2.cc

check: testresampler-check

testresampler-check: testresampler
	@./testresampler check
