#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Passive Weather Radar test receiver
# Author: Peter F Bradshaw
# Description: This is a test receiver for the of the Passive Weather Radar
# Generated: Tue Jun 25 20:23:14 2019
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from optparse import OptionParser
import sip
import sys


class PWR_model(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Passive Weather Radar test receiver")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Passive Weather Radar test receiver")
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "PWR_model")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 8388608

        ##################################################
        # Blocks
        ##################################################
        self.target_stream_to_vector = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 1024)
        self.target_fft = fft.fft_vcc(1024, False, (window.rectangular(1024)), False, 1)
        self.refernce_stream_to_vector = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 1024)
        self.reference_fft = fft.fft_vcc(1024, False, (window.rectangular(1024)), False, 1)
        self.qtgui_sink_x_0 = qtgui.sink_c(
        	1024, #fftsize
        	firdes.WIN_RECTANGULAR, #wintype
        	0, #fc
        	samp_rate, #bw
        	"Correlation", #name
        	True, #plotfreq
        	True, #plotwaterfall
        	True, #plottime
        	True, #plotconst
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_sink_x_0_win)
        
        self.qtgui_sink_x_0.enable_rf_freq(False)
        
        
          
        self.digital_psk_mod_1 = digital.psk.psk_mod(
          constellation_points=4,
          mod_code="gray",
          differential=True,
          samples_per_symbol=2,
          excess_bw=0.35,
          verbose=False,
          log=False,
          )
        self.digital_psk_mod_0 = digital.psk.psk_mod(
          constellation_points=4,
          mod_code="gray",
          differential=True,
          samples_per_symbol=2,
          excess_bw=0.35,
          verbose=False,
          log=False,
          )
        self.digital_psk_demod_1 = digital.psk.psk_demod(
          constellation_points=8,
          differential=True,
          samples_per_symbol=2,
          excess_bw=0.35,
          phase_bw=6.28/100.0,
          timing_bw=6.28/100.0,
          mod_code="gray",
          verbose=False,
          log=False,
          )
        self.digital_psk_demod_0 = digital.psk.psk_demod(
          constellation_points=4,
          differential=True,
          samples_per_symbol=2,
          excess_bw=0.35,
          phase_bw=6.28/100.0,
          timing_bw=6.28/100.0,
          mod_code="gray",
          verbose=False,
          log=False,
          )
        self.digital_glfsr_source_x_0 = digital.glfsr_source_f(8, True, 0, 1)
        self.dc_blocker_xx_0 = filter.dc_blocker_ff(32, True)
        self.correlation_vector_to_stream = blocks.vector_to_stream(gr.sizeof_gr_complex*1, 1024)
        self.blocks_throttle_1 = blocks.throttle(gr.sizeof_float*1, samp_rate,True)
        self.blocks_multiply_conjugate_cc_0 = blocks.multiply_conjugate_cc(1024)
        self.blocks_float_to_complex_1 = blocks.float_to_complex(1)
        self.blocks_float_to_complex_0 = blocks.float_to_complex(1)
        self.blocks_float_to_char_0 = blocks.float_to_char(1, 1)
        self.blocks_delay_0 = blocks.delay(gr.sizeof_gr_complex*1, 0)
        self.blocks_char_to_float_1 = blocks.char_to_float(1, 1)
        self.blocks_char_to_float_0 = blocks.char_to_float(1, 1)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_char_to_float_0, 0), (self.blocks_float_to_complex_0, 0))    
        self.connect((self.blocks_char_to_float_1, 0), (self.blocks_float_to_complex_1, 0))    
        self.connect((self.blocks_delay_0, 0), (self.digital_psk_demod_0, 0))    
        self.connect((self.blocks_float_to_char_0, 0), (self.digital_psk_mod_0, 0))    
        self.connect((self.blocks_float_to_char_0, 0), (self.digital_psk_mod_1, 0))    
        self.connect((self.blocks_float_to_complex_0, 0), (self.refernce_stream_to_vector, 0))    
        self.connect((self.blocks_float_to_complex_1, 0), (self.target_stream_to_vector, 0))    
        self.connect((self.blocks_multiply_conjugate_cc_0, 0), (self.correlation_vector_to_stream, 0))    
        self.connect((self.blocks_throttle_1, 0), (self.dc_blocker_xx_0, 0))    
        self.connect((self.correlation_vector_to_stream, 0), (self.qtgui_sink_x_0, 0))    
        self.connect((self.dc_blocker_xx_0, 0), (self.blocks_float_to_char_0, 0))    
        self.connect((self.digital_glfsr_source_x_0, 0), (self.blocks_throttle_1, 0))    
        self.connect((self.digital_psk_demod_0, 0), (self.blocks_char_to_float_1, 0))    
        self.connect((self.digital_psk_demod_1, 0), (self.blocks_char_to_float_0, 0))    
        self.connect((self.digital_psk_mod_0, 0), (self.digital_psk_demod_1, 0))    
        self.connect((self.digital_psk_mod_1, 0), (self.blocks_delay_0, 0))    
        self.connect((self.reference_fft, 0), (self.blocks_multiply_conjugate_cc_0, 0))    
        self.connect((self.refernce_stream_to_vector, 0), (self.reference_fft, 0))    
        self.connect((self.target_fft, 0), (self.blocks_multiply_conjugate_cc_0, 1))    
        self.connect((self.target_stream_to_vector, 0), (self.target_fft, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "PWR_model")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)
        self.blocks_throttle_1.set_sample_rate(self.samp_rate)


def main(top_block_cls=PWR_model, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
