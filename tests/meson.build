# we use these extra libraries for testing, but not for the resampler itself
zitaresampler_dep = cxx.find_library('zita-resampler')
fftw_dep = dependency('fftw3')
fftwf_dep = dependency('fftw3f')

# tests programs using the library
foreach t : [ 'testsimple', 'testdistort', 'testdownmulti', 'testmultiperf', 'testsawquality' ]
  executable(t,
             sources: files(t + '.cc'),
             include_directories : incdir,
             dependencies: [fftw_dep, fftwf_dep, zitaresampler_dep],
             link_with: [libpandaresampler])
endforeach

# tests programs using the header only mode
testheaderonly1 = executable('testheaderonly1',
           sources: files('testheaderonly1.cc'),
           include_directories : incdir)

testheaderonly2 = executable('testheaderonly2',
           sources: files('testheaderonly1.cc', 'testheaderonly2.cc'),
           include_directories : incdir)

# tests for meson test suite
testresampler = executable('testresampler',
                           sources: files('testresampler.cc'),
                           include_directories : incdir,
                           dependencies: [fftw_dep],
                           link_with: [libpandaresampler])

testmultidelay = executable('testmultidelay',
                            sources: files('testmultidelay.cc'),
                            include_directories : incdir,
                            link_with: [libpandaresampler])

testaddr = executable('testaddr',
                      sources: files('testaddr.cc'),
                      include_directories : incdir,
                      link_with: [libpandaresampler])

test('testresampler', testresampler, args : [ 'check' ], timeout : 0)
test('testmultidelay', testmultidelay)
test('testaddr', testaddr)
test('testheaderonly1', testheaderonly1)
test('testheaderonly2', testheaderonly2)
