project('PWR', ['cpp', 'c'],
        version : '0.0.0',
        license : 'GPL',
        meson_version : '1.0.1')

cpp_compiler = meson.get_compiler('cpp')
c_compiler   = meson.get_compiler('c')

srslte_includes = include_directories(
                                      [
                                      '/usr/local/include/srslte/',
                                      ],
                                     )

srslte_libraries = [
                   '-L /usr/local/lib',
                   '-l rrc_asn1',
                   '-l srslte_asn1',
                   '-l srslte_common',
                   '-l srslte_phy',
                   '-l srslte_upper',
                   ]

cpp = meson.get_compiler('cpp')
rrc_asn1 = cpp.find_library('rrc_asn1',
                            dirs : '/usr/local/lib',
                            has_headers : 'asn1/rrc_asn1.h',
                            header_include_directories : srslte_includes,
                            required : true,
                            static   : true,
                           )
srslte_asn1 = cpp.find_library('srslte_asn1',
                               dirs : '/usr/local/lib',
                               has_headers : 'asn1/rrc_asn1.h',
                               header_include_directories : srslte_includes,
                               required : true,
                               static   : true,
                              )
srslte       = declare_dependency(
                                  include_directories : '/usr/local/include/srslte/',
                                 )
                          
src_includes = include_directories('src/Receiver')
soapy_includes = include_directories('/usr/include/SoapySDR')
#subdir('lib')
subdir('src')
