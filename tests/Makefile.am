PANDARESAMPLER_LIBS = $(top_builddir)/lib/libpandaresampler.la

AM_CXXFLAGS = -I$(top_srcdir)/lib

noinst_PROGRAMS = testresampler testheaderonly1 testheaderonly2

testresampler_SOURCES = testresampler.cc
testresampler_LDADD = $(PANDARESAMPLER_LIBS)

testheaderonly1_SOURCES = testheaderonly1.cc

testheaderonly2_SOURCES = testheaderonly1.cc testheaderonly2.cc
