project(
  'Stasis', 'cpp',
  default_options: ['cpp_rtti=false', 'cpp_eh=none']
)

cc = meson.get_compiler('cpp')
libdir = meson.source_root() + '/obj'
incdirs = ['deps/glfw/include']

libglfw = cc.find_library('glfw3', dirs: libdir + '/glfw/src')
frameworks = [
  dependency('CoreFoundation'),
  dependency('Foundation'),
  dependency('AppKit'),
  dependency('Cocoa'),
  dependency('IOKit'),
]

executable(
  'Stasis',
  'src/main.cpp',
  dependencies: [libglfw, frameworks],
  include_directories: incdirs
)
