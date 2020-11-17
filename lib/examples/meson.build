#libcommon  = c_compiler.find_library('libsrslte_common', 
#                                     dirs : '/usr/local/lib')
#libphy     = c_compiler.find_library('libsrslte_phy', dirs : '/usr/local/lib')
#libradio   = c_compiler.find_library('libsrslte_radio', dirs : '/usr/local/lib')
#librf      = c_compiler.find_library('libsrslte_rf', dirs : '/usr/local/lib')
#librfutils = c_compiler.find_library('libsrslte_rf_utils',
#                                     dirs : '/home/pfb/Work/srsLTE/build/lib/src/phy/rf/')
#libcommon  = c_compiler.find_library('common', 
#                                     dirs : '/home/pfb/Work/PWR/lib/src/common')
#libphy     = c_compiler.find_library('phy',
#                                     dirs : '/home/pfb/Work/PWR/lib/src/phy')
#libradio   = c_compiler.find_library('radio',
#                                     dirs : '/home/pfb/Work/PWR/lib/src/radio')
#librf      = c_compiler.find_library('rf',
#                                     dirs : '/home/pfb/Work/PWR/lib/src/phy/rf')
#librfutils = c_compiler.find_library('rf_utils',
#                                     dirs : '/home/pfb/Work/srsLTE/build/lib/src/phy/rf')

libsoapy   = c_compiler.find_library('libSoapySDR')
libpthread = c_compiler.find_library('libpthread')
libfftw3   = c_compiler.find_library('libfftw3f')
libmath    = c_compiler.find_library('libm')
extralibs  = [ libpthread, libfftw3, libmath ]
srslibs = [ libcommon, 
            libphy,
            libsync, 
          ]

#cell_measurement = executable('cell_measurement', 'cell_measurement.cpp',
#                              include_directories : lib_includes,
#                              link_with : srslibs,
#                             )
cell_search = executable('cell_search', 'cell_search.c',
                         include_directories : lib_includes,
                         link_with : srslibs,
                         dependencies : extralibs,
                         include_directories : [ lib_includes, soapy_includes ]
                        )
#pdsch_enodeb = executable('pdsch_enodeb', 'pdsch_enodeb.c',
#                          include_directories : lib_includes,
#                          link_with : srslibs,
#                         )
#pdsch_ue = executable('pdsch_ue', 'pdsch_ue.c',
#                      include_directories : lib_includes,
#                      link_with : srslibs
#                     )
synch_file = executable('synch_file', 'synch_file.c',
                        include_directories : lib_includes,
                        link_with : srslibs,
                        dependencies : extralibs,
                       )
#usrp_capture = executable('usrp_capture', 'usrp_capture.c',
#                          include_directories : lib_includes,
#                          link_with : srslibs,
#                         )
#usrp_capture_sync = executable('usrp_capture_sync', 'usrp_capture_sync.c',
#                               include_directories : lib_includes,
#                               link_with : srslibs,
#                              )
#usrp_txrx = executable('usrp_txrx', 'usrp_txrx.c',
#                       include_directories : lib_includes,
#                       link_with : srslibs,
#                      )       
