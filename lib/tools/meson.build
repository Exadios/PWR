libsoapy   = c_compiler.find_library('libSoapySDR')
libpthread = c_compiler.find_library('libpthread')
libfftw3   = c_compiler.find_library('libfftw3f')
libmath    = c_compiler.find_library('libm')
extralibs  = [ libsoapy, libpthread, libfftw3, libmath ]
srslibs = [ libcommon, 
            libphy,
            libsync, 
          ]

print_band_data = executable('print_band_data', 'print_band_data.cpp',
                             include_directories : lib_includes,
                             link_with : srslibs,
                             dependencies : extralibs,
                             include_directories : [ lib_includes, soapy_includes ]
                            )

cell_track = executable('cell_track', 'cell_track.c',
                        include_directories : lib_includes,
                        link_with : srslibs,
                        dependencies : extralibs,
                        include_directories : [ lib_includes, soapy_includes ]
                       )
