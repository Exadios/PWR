librfutils = static_library('rf_utils', 'rf_utils.c')

librf = static_library('rf',
                       'rf_imp.c',
                       'rf_soapy_imp.c',
                       cpp_args : '-DENABLE_SOAPYSDR',
                       c_args : '-DENABLE_SOAPYSDR')
