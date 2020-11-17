# libdft = library('dft',
#                   'dft.cpp',
#                   include_directories : lib_includes,
#                   library_includes : lib_includes,
#                   dependencies : [ compiler.find_library('utils'),
#                                    dependency('fftw3'), 
#                                    dependency('fftw3f') ])
 libdft = library('dft',
                   'dft_fftw.c',
                   'dft_precoding.c',
                   'ofdm.c',
                   include_directories : lib_includes,
                   library_includes : lib_includes,
                   dependencies : [ 
                                    dependency('fftw3'), 
                                    dependency('fftw3f') ])
