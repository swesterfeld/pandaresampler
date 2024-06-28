project('pandaresampler', 'cpp', default_options : ['cpp_std=c++11', 'warning_level=3'])

cxx = meson.get_compiler('cpp')

zitaresamplerdep = cxx.find_library('zita-resampler')

install_headers('include/pandaresampler.hh')
install_headers('include/pandaresampler/pandaresampler.cc', 'pandaresampler')
install_headers('include/pandaresampler/hiir/def.h', 'pandaresampler/hiir')
