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
extralibs  = [ libsoapy, libpthread, libfftw3, libmath ]
srslibs = [ libcommon, 
            libphy,
            libsync, 
          ]

logger = executable('logger_test', 'logger_test.cpp',
                    include_directories : lib_includes,
                    link_with : srslibs,
                    dependencies : extralibs,
                    include_directories : [ lib_includes, soapy_includes ],
                   )
#msg_queue = executable('msg_queue_test', 'msg_queue_test.cpp',
#                       include_directories : lib_includes,
#                       link_with : srslibs,
#                       dependencies : extralibs,
#                       include_directories : [ lib_includes, soapy_includes ],
#                      )
#eia1 = executable('test_eia1', 'test_eia1.cpp',
#                  include_directories : lib_includes,
#                  link_with : srslibs,
#                  dependencies : extralibs,
#                  include_directories : [ lib_includes, soapy_includes ],
#                 )
#eea1 = executable('test_eea1', 'test_eea1.cpp',
#                  include_directories : lib_includes,
#                  link_with : srslibs,
#                  dependencies : extralibs,
#                  include_directories : [ lib_includes, soapy_includes ],
#                 )
#eea2 = executable('test_eea2', 'test_eea2.cpp',
#                  include_directories : lib_includes,
#                  link_with : srslibs,
#                  dependencies : extralibs,
#                  include_directories : [ lib_includes, soapy_includes ],
#                 )
#f12345 = executable('test_f12345', 'test_f12345.cpp',
#                    include_directories : lib_includes,
#                    link_with : srslibs,
#                    dependencies : extralibs,
#                    include_directories : [ lib_includes, soapy_includes ],
#                   )
log_filter = executable('log_filter_test', 'log_filter_test.cpp',
                        include_directories : lib_includes,
                        link_with : srslibs,
                        dependencies : extralibs,
                        include_directories : [ lib_includes, soapy_includes ],
                       )
timeout = executable('timeout_test', 'timeout_test.cpp',
                     include_directories : lib_includes,
                     link_with : srslibs,
                     dependencies : extralibs,
                     include_directories : [ lib_includes, soapy_includes ],
                    )
bcd_helpers = executable('bcd_helpers_test', 'bcd_helpers_test.cpp',
                         include_directories : lib_includes,
                         link_with : srslibs,
                         dependencies : extralibs,
                         include_directories : [ lib_includes, soapy_includes ]
                        )       
