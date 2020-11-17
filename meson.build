project('PWR', ['cpp', 'c'], version : '0.0.0', license : 'GPL')

cpp_compiler = meson.get_compiler('cpp')
c_compiler   = meson.get_compiler('c')
lib_includes = include_directories(['lib/include', 'lib/include/srslte'])
src_includes = include_directories('src/Receiver')
soapy_includes = include_directories('/usr/include/SoapySDR')
subdir('lib')
#subdir('src')
