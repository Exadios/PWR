#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Passive Weather Radar test receiver
# Author: Peter F Bradshaw
# Description: This is a test receiver for the of the Passive Weather Radar
# Generated: Tue Jun 25 10:38:53 2019
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
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from optparse import OptionParser
import osmosdr
import sip
import sys
import time


class PWR_test_receiver(gr.top_block, Qt.QWidget):

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

        self.settings = Qt.QSettings("GNU Radio", "PWR_test_receiver")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 8388608

        ##################################################
        # Blocks
        ##################################################
        self.test_display_1 = qtgui.time_sink_c(
        	1024, #size
        	samp_rate, #samp_rate
        	"", #name
        	1 #number of inputs
        )
        self.test_display_1.set_update_time(0.10)
        self.test_display_1.set_y_axis(-1, 1)
        
        self.test_display_1.set_y_label('Correlation', "")
        
        self.test_display_1.enable_tags(-1, True)
        self.test_display_1.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.test_display_1.enable_autoscale(False)
        self.test_display_1.enable_grid(False)
        self.test_display_1.enable_axis_labels(True)
        self.test_display_1.enable_control_panel(False)
        
        if not True:
          self.test_display_1.disable_legend()
        
        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "blue"]
        styles = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
                   -1, -1, -1, -1, -1]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        
        for i in xrange(2*1):
            if len(labels[i]) == 0:
                if(i % 2 == 0):
                    self.test_display_1.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.test_display_1.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.test_display_1.set_line_label(i, labels[i])
            self.test_display_1.set_line_width(i, widths[i])
            self.test_display_1.set_line_color(i, colors[i])
            self.test_display_1.set_line_style(i, styles[i])
            self.test_display_1.set_line_marker(i, markers[i])
            self.test_display_1.set_line_alpha(i, alphas[i])
        
        self._test_display_1_win = sip.wrapinstance(self.test_display_1.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._test_display_1_win)
        self.target_stream_to_vector = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 1024)
        self.target_fft = fft.fft_vcc(1024, True, (window.rectangular(1024)), True, 1)
        self.rtlsdr_target_source = osmosdr.source( args="numchan=" + str(1) + " " + '' )
        self.rtlsdr_target_source.set_sample_rate(samp_rate)
        self.rtlsdr_target_source.set_center_freq(970e6, 0)
        self.rtlsdr_target_source.set_freq_corr(0, 0)
        self.rtlsdr_target_source.set_dc_offset_mode(0, 0)
        self.rtlsdr_target_source.set_iq_balance_mode(0, 0)
        self.rtlsdr_target_source.set_gain_mode(False, 0)
        self.rtlsdr_target_source.set_gain(10, 0)
        self.rtlsdr_target_source.set_if_gain(20, 0)
        self.rtlsdr_target_source.set_bb_gain(20, 0)
        self.rtlsdr_target_source.set_antenna('', 0)
        self.rtlsdr_target_source.set_bandwidth(0, 0)
          
        self.rtlsdr_reference_source = osmosdr.source( args="numchan=" + str(1) + " " + '' )
        self.rtlsdr_reference_source.set_sample_rate(samp_rate)
        self.rtlsdr_reference_source.set_center_freq(970e6, 0)
        self.rtlsdr_reference_source.set_freq_corr(0, 0)
        self.rtlsdr_reference_source.set_dc_offset_mode(0, 0)
        self.rtlsdr_reference_source.set_iq_balance_mode(0, 0)
        self.rtlsdr_reference_source.set_gain_mode(False, 0)
        self.rtlsdr_reference_source.set_gain(10, 0)
        self.rtlsdr_reference_source.set_if_gain(20, 0)
        self.rtlsdr_reference_source.set_bb_gain(20, 0)
        self.rtlsdr_reference_source.set_antenna('', 0)
        self.rtlsdr_reference_source.set_bandwidth(0, 0)
          
        self.refernce_stream_to_vector = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 1024)
        self.reference_fft = fft.fft_vcc(1024, True, (window.rectangular(1024)), True, 1)
        self.correlation_vector_to_stream = blocks.vector_to_stream(gr.sizeof_gr_complex*1, 1024)
        self.correlation_fft = fft.fft_vcc(1024, False, (window.rectangular(1024)), True, 1)
        self.blocks_multiply_conjugate_cc_0 = blocks.multiply_conjugate_cc(1024)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_multiply_conjugate_cc_0, 0), (self.correlation_fft, 0))    
        self.connect((self.correlation_fft, 0), (self.correlation_vector_to_stream, 0))    
        self.connect((self.correlation_vector_to_stream, 0), (self.test_display_1, 0))    
        self.connect((self.reference_fft, 0), (self.blocks_multiply_conjugate_cc_0, 0))    
        self.connect((self.refernce_stream_to_vector, 0), (self.reference_fft, 0))    
        self.connect((self.rtlsdr_reference_source, 0), (self.refernce_stream_to_vector, 0))    
        self.connect((self.rtlsdr_target_source, 0), (self.target_stream_to_vector, 0))    
        self.connect((self.target_fft, 0), (self.blocks_multiply_conjugate_cc_0, 1))    
        self.connect((self.target_stream_to_vector, 0), (self.target_fft, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "PWR_test_receiver")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.test_display_1.set_samp_rate(self.samp_rate)
        self.rtlsdr_target_source.set_sample_rate(self.samp_rate)
        self.rtlsdr_reference_source.set_sample_rate(self.samp_rate)


def main(top_block_cls=PWR_test_receiver, options=None):

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
