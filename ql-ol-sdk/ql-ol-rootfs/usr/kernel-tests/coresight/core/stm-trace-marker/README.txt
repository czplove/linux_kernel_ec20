	STM Testing Documentation

Usage: stm_test.sh [runs] [run-size] [delay] [usb|mem]
       Runs the basic stm test.

OPTIONS:
  The following options can be used:

  - runs: Number of runs to be performed
  - run-size: Number of trace_marker entries in each run
  - delay: The delay between runs, specified in seconds
  - usb/mem: If you want the coresight sink to be usb or etr mem, default is etf

  Not specifying any of these will default runs=50, run-size=10 and
  delay=0.001s, that is 1ms. Also coresight sink will be etf.

TEST BEHAVIOR:
	* Test will use trace_marker to test stm. It will start by enabling STM.
	  It will then echo an increasing counter to the trace_marker
	  and will continue doing so depending on the number of runs and
	  run-size specified, The total entries echoed will be run*run-size.
	  The echoed text will be visible on the screen
	  It will then disable the STM and dump the ETF dump to the
	  /data/ftace/stm_trace.bin file. It will also collect the trace.txt
	  file from debugfs and dump it under /data/coresight alongwith the
	  format file.

	  After obtaining the stm_trace.bin file from the device, the qht
	  tool can be used to parse it and observe the text echoed to the
	  trace_marker event.
	  This parsed data can be compared with the data for the trace_marker
	  event observed in the trace.txt file previously dumped and should
	  match that exactly.

NOTE:
	* Assumes /data partition exists
