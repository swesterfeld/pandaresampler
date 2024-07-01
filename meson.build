project('pandaresampler', 'cpp',
        version : '0.2.1',
        default_options : [
          'cpp_std=c++11',
          'warning_level=3',
          'buildtype=release'
        ])

cxx = meson.get_compiler('cpp')

add_project_arguments(
  cxx.get_supported_arguments([ '-Wno-vla' ]),
  language : 'cpp'
)

incdir = include_directories('include')

libpandaresampler = library('pandaresampler', 'lib/libpandaresampler.cc', include_directories : incdir, install : true)

subdir('filter-design')

if get_option('tests')
  subdir('tests')
endif
summary({ 'tests' : get_option ('tests') })

install_subdir('include', install_dir : get_option('includedir'), strip_directory : true)

# Generate the pkg-config file
pkg = import('pkgconfig')
pkg.generate(
  name: 'PandaResampler',
  description: 'PandaResampler fast factor 2 resampler',
  version: meson.project_version(),
  libraries: libpandaresampler,
  filebase: 'pandaresampler'
)
