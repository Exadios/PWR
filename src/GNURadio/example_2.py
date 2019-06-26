#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: A Givern Example
# Author: Peter F Bradshaw
# Description: Example of cross correlation
# Generated: Tue Jun 25 20:44:12 2019
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
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from optparse import OptionParser
import sip
import sys
import threading
import time


class example_2(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "A Givern Example")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("A Givern Example")
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

        self.settings = Qt.QSettings("GNU Radio", "example_2")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 32000
        self.delay = delay = 0
        self.d_sq = d_sq = 0

        ##################################################
        # Blocks
        ##################################################
        self.probe_sq = blocks.probe_signal_f()
        self._delay_range = Range(-125, 125, 1, 0, 200)
        self._delay_win = RangeWidget(self._delay_range, self.set_delay, "delay", "counter_slider", int)
        self.top_layout.addWidget(self._delay_win)
        self.sig_delay = blocks.delay(gr.sizeof_gr_complex*1, delay)
        self.qtgui_waterfall_sink_x_0 = qtgui.waterfall_sink_c(
        	1024, #size
        	firdes.WIN_RECTANGULAR, #wintype
        	0, #fc
        	samp_rate, #bw
        	'Target', #name
                1 #number of inputs
        )
        self.qtgui_waterfall_sink_x_0.set_update_time(0.10)
        self.qtgui_waterfall_sink_x_0.enable_grid(False)
        self.qtgui_waterfall_sink_x_0.enable_axis_labels(True)
        
        if not True:
          self.qtgui_waterfall_sink_x_0.disable_legend()
        
        if "complex" == "float" or "complex" == "msg_float":
          self.qtgui_waterfall_sink_x_0.set_plot_pos_half(not True)
        
        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        colors = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_waterfall_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_waterfall_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_waterfall_sink_x_0.set_color_map(i, colors[i])
            self.qtgui_waterfall_sink_x_0.set_line_alpha(i, alphas[i])
        
        self.qtgui_waterfall_sink_x_0.set_intensity_range(-140, 50)
        
        self._qtgui_waterfall_sink_x_0_win = sip.wrapinstance(self.qtgui_waterfall_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_waterfall_sink_x_0_win)
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
        
        
          
        self.fft_vxx_1 = fft.fft_vcc(1024, False, (window.rectangular(1024)), False, 1)
        self.fft_vxx_0 = fft.fft_vcc(1024, False, (window.rectangular(1024)), False, 1)
        self.dc_blocker_xx_0 = filter.dc_blocker_cc(32, True)
        
        def _d_sq_probe():
            while True:
                val = self.probe_sq.level()
                try:
                    self.set_d_sq(val)
                except AttributeError:
                    pass
                time.sleep(1.0 / (100))
        _d_sq_thread = threading.Thread(target=_d_sq_probe)
        _d_sq_thread.daemon = True
        _d_sq_thread.start()
            
        self.blocks_vector_to_stream_0 = blocks.vector_to_stream(gr.sizeof_gr_complex*1, 1024)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_stream_to_vector_1 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 1024)
        self.blocks_stream_to_vector_0 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 1024)
        self.blocks_multiply_conjugate_cc_0 = blocks.multiply_conjugate_cc(1024)
        self.analog_sig_source_x_1 = analog.sig_source_f(samp_rate, analog.GR_SQR_WAVE, 1000, 250, -125)
        self.analog_fastnoise_source_x_0 = analog.fastnoise_source_c(analog.GR_GAUSSIAN, 1, 0, 8192)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_fastnoise_source_x_0, 0), (self.dc_blocker_xx_0, 0))    
        self.connect((self.analog_sig_source_x_1, 0), (self.probe_sq, 0))    
        self.connect((self.blocks_multiply_conjugate_cc_0, 0), (self.blocks_vector_to_stream_0, 0))    
        self.connect((self.blocks_stream_to_vector_0, 0), (self.fft_vxx_0, 0))    
        self.connect((self.blocks_stream_to_vector_1, 0), (self.fft_vxx_1, 0))    
        self.connect((self.blocks_throttle_0, 0), (self.blocks_stream_to_vector_1, 0))    
        self.connect((self.blocks_throttle_0, 0), (self.sig_delay, 0))    
        self.connect((self.blocks_vector_to_stream_0, 0), (self.qtgui_sink_x_0, 0))    
        self.connect((self.blocks_vector_to_stream_0, 0), (self.qtgui_waterfall_sink_x_0, 0))    
        self.connect((self.dc_blocker_xx_0, 0), (self.blocks_throttle_0, 0))    
        self.connect((self.fft_vxx_0, 0), (self.blocks_multiply_conjugate_cc_0, 1))    
        self.connect((self.fft_vxx_1, 0), (self.blocks_multiply_conjugate_cc_0, 0))    
        self.connect((self.sig_delay, 0), (self.blocks_stream_to_vector_0, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "example_2")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.qtgui_waterfall_sink_x_0.set_frequency_range(0, self.samp_rate)
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.analog_sig_source_x_1.set_sampling_freq(self.samp_rate)

    def get_delay(self):
        return self.delay

    def set_delay(self, delay):
        self.delay = delay
        self.sig_delay.set_dly(self.delay)

    def get_d_sq(self):
        return self.d_sq

    def set_d_sq(self, d_sq):
        self.d_sq = d_sq


def main(top_block_cls=example_2, options=None):

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
