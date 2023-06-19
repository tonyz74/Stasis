project(
  'Stasis', ['cpp', 'c', 'objc'],
  default_options: ['cpp_rtti=false', 'cpp_eh=none', 'cpp_std=c++11']
)

cc = meson.get_compiler('cpp')
libdir = meson.source_root() + '/obj'
incdirs = [
  'src',
  'deps/glfw/include',
  'deps/wgpu-native/ffi',
  'deps/wgpu-native/ffi/webgpu-headers'
]

libglfw = cc.find_library('glfw3', dirs: libdir + '/glfw/src')
libwgpu = cc.find_library('wgpu_native', dirs: libdir + '/wgpu-native/release')

frameworks = [
  dependency('CoreFoundation'),
  dependency('Foundation'),
  dependency('AppKit'),
  dependency('Cocoa'),
  dependency('IOKit'),
  dependency('CoreVideo'),
  dependency('QuartzCore'),
]

sources = [
  # window
  'src/window/window.cpp',
  'src/window/glfw3webgpu/glfw3webgpu.m',

  # driver
  'src/main.cpp',
  'src/core/log.cpp',
  'src/platform/fs.cpp',
  'src/memory/system_allocator.cpp',
]

executable(
  'Stasis',
  sources,
  dependencies: [libglfw, libwgpu, frameworks],
  include_directories: incdirs
)
