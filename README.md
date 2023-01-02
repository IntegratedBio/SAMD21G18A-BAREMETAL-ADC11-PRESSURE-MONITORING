Board: Arduino Nano 33 IoT Compatibility: Nano 33's / Zero / MKR1000 / Seeed Studio XIAO or most [SAMD21G18A] mcu's Program: Baremetal ADC Chanel 11 Setup Debugging Hardware: Atmel ICE via soldered SWD pins Sensor Hardwarer: DC Signal Generator Generic CLock 0: 8 MHZ Generic Clock 2: 32 Khz (High Accuracy) Resolution: 16 Bit w/ 16 averaged samples.

ADC SETUP. This code demonstrates how to configure "ADC 11" in singles-ended mode on Pin [D21|A7|PB02] for (+) positive signal measured against and internal ground. The reference voltage is 1/1.48 VDDANA, or 2.23 VDC.

The value can be watched and ADC values vary betwen [1000 - 65,520] with a signal from [0 - 2.23 VDC]. This is calibrated to readout pressure [0 - 150 psi]

BaudRate @ 5000000 on TX/RX pins
