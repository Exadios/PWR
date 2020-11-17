
agc_srcs = 'agc/agc.c'
channel_srcs = [ 'channel/ch_awgn.c', 'channel/gauss.c' ]
ch_estimation_scrs = [ 'ch_estimation/chest_common.c',
                       'ch_estimation/chest_dl.c',
                       'ch_estimation/chest_ul.c',
                       'ch_estimation/refsignal_dl.c',
                       'ch_estimation/refsignal_ul.c',
                     ]
common_scrs = [ 'common/phy_common.c',
                'common/sequence.c',
                'common/timestamp.c',
                'common/phy_logger.c' 
              ]
dft_srcs = [ 'dft/dft_fftw.c', 'dft/dft_precoding.c', 'dft/ofdm.c' ]
enb_srcs = [ 'enb/enb_dl.c', 'enb/enb_ul.c' ]
fec_srcs = [ 'fec/cbsegm.c',
             'fec/convcoder.c',
             'fec/crc.c',
             'fec/parity.c',
             'fec/rm_conv.c',
             'fec/rm_turbo.c',
             'fec/softbuffer.c',
             'fec/tc_interl_lte.c',
             'fec/tc_interl_umts.c',
             'fec/turbocoder.c',
             'fec/turbodecoder.c',
             'fec/turbodecoder_gen.c',
             'fec/turbodecoder_sse.c',
             'fec/viterbi37_avx2_16bit.c',
             'fec/viterbi37_avx2.c',
             'fec/viterbi37_neon.c',
             'fec/viterbi37_port.c',
             'fec/viterbi37_sse.c',
             'fec/viterbi.c',
           ]
io_srcs = [ 'io/binsource.c',
            'io/filesink.c',
            'io/filesource.c',
            'io/netsink.c',
            'io/netsource.c',
          ]
mimo_srcs = [ 'mimo/layermap.c', 'mimo/precoding.c', ]
modem_srcs = [ 'modem/demod_hard.c',
               'modem/demod_soft.c',
               'modem/hard_demod_lte.c',
               'modem/lte_tables.c',
               'modem/mod.c',
               'modem/modem_table.c',
             ]
phch_srcs = [ 'phch/cqi.c',
              'phch/dci.c',
              'phch/pbch.c',
              'phch/pcfich.c',
              'phch/pdcch.c',
              'phch/pdsch.c',
              'phch/phich.c',
              'phch/pmch.c',
              'phch/prach.c',
              'phch/prb_dl.c',
              'phch/pucch.c',
              'phch/pusch.c',
              'phch/ra.c',
              'phch/regs.c',
              'phch/sch.c',
              'phch/sequences.c',
              'phch/uci.c',
            ]
resampling_srcs = [ 'resampling/decim.c',
                    'resampling/interp.c',
                    'resampling/resample_arb.c',
                  ]
rf_srcs = [ 'rf/rf_soapy_imp.c',
            'rf/rf_utils.c',
            'rf/rf_imp.c',
          ]
scrambling_srcs = [ 'scrambling/scrambling.c', ]
sync_srcs = [ 'sync/cfo.c',
              'sync/cp.c',
              'sync/find_sss.c',
              'sync/gen_sss.c',
              'sync/pss.c',
              'sync/sfo.c',
              'sync/sss.c',
              'sync/sync.c',
            ]
ue_srcs = [ 'ue/ue_cell_search.c',
            'ue/ue_dl.c',
            'ue/ue_mib.c',
            'ue/ue_sync.c',
            'ue/ue_ul.c',
          ]
utils_srcs = [ 'utils/bit.c',
               'utils/cexptab.c',
               'utils/convolution.c',
               'utils/debug.c',
               'utils/filter.c',
               'utils/mat.c',
               'utils/ringbuffer.c',
               'utils/vector.c',
               'utils/vector_simd.c',
             ]

threads = dependency('threads')
fftwf   = dependency('fftw3f')
fftw    = dependency('fftw3')
libphy = static_library('phy',
                          agc_srcs,
                          channel_srcs,
                          ch_estimation_scrs,
                          common_scrs,
                          dft_srcs,
                          enb_srcs,
                          fec_srcs,
                          io_srcs,
                          mimo_srcs,
                          modem_srcs,
                          phch_srcs,
                          resampling_srcs,
                          rf_srcs,
                          scrambling_srcs,
                          sync_srcs,
                          ue_srcs,
                          utils_srcs,
                        include_directories : [ lib_includes, soapy_includes ],
                        dependency : [ threads, fftw, fftwf ],
                       )
#subdir('rf')
