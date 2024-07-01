project('pandaresampler', 'cpp',
        version : '0.2.1',
        default_options : [
          'cpp_std=c++11',
          'warning_level=3',
          'buildtype=release'
        ])

cxx = meson.get_compiler('cpp')

if get_option('debug_cxx')
  add_project_arguments(['-D_GLIBCXX_DEBUG', '-D_GLIBCXX_DEBUG_PEDANTIC' ], language : 'cpp')
endif

add_project_arguments(
  cxx.get_supported_arguments([ '-Wno-vla' ]),
  language : 'cpp'
)

# Includes
incdir = include_directories('include')
install_subdir('include', install_dir : get_option('includedir'), strip_directory : true)

# Library
libpandaresampler = library('pandaresampler', 'lib/libpandaresampler.cc', include_directories : incdir, install : true)

# Generate the pkg-config file
pkg = import('pkgconfig')
pkg.generate(
  name: 'PandaResampler',
  description: 'PandaResampler fast factor 2 resampler',
  version: meson.project_version(),
  libraries: libpandaresampler,
  filebase: 'pandaresampler'
)

# Developer Mode
if get_option('devel')
  subdir('filter-design')
  subdir('tests')
  run_target('rebuild-api-docs', command : ['misc/rebuild-api-docs.sh', meson.project_version()])
endif
summary({ 'Developer Mode (use -Ddevel=true to enable)' : get_option ('devel') })
