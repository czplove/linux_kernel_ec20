Test: coresight

Usage: cs_test.sh [-n | --nominal] [-a | --adversarial] [-r | --repeatability]\
       [-s | stress ]
       Runs the tests for CoreSight trace drivers.


Options:
The following options can be used:
-n | --nominal                                 nominal test, runs by default
-a | --adversarial                             adversarial test
-r | --repeatability iterations                repeatability test, runs 10
                                               iterations by default
-s | --stress                                  stress test
--source trace_source                          trace source, supported
                                               trace_source values are 'etm'
                                               and 'stm'

Requirement:
CoreSight ETM and STM sources are initially disabled before running any test.

Description:
Nominal Test:
All CoreSight tests are invoked.

Adversarial Test:
Runs cs_adversary.sh. This test enables all trace sources, all ftrace events and
toggles the cpu cores on/off.

Repeatability Tests:
All test cases except probe test are invoked for user specified number of iterations,
or for 10 iterations by default.

Stress Test:
All test cases except probe test are invoked for 100 iterations.

Test case description:
1. platform/platform.sh
Tests that CoreSight trace drivers are probed without failures.

2. etm/etm_enable.sh
Tests ETM tracing gets properly enabled.

3. etm/etm_disable.sh
Tests ETM tracing get properly disabled.

4. stm/stm_enable.sh
Tests STM tracing gets properly enabled.

5. stm/stm_disable.sh
Tests STM tracing gets properly disabled.

6. stm/stm_etf_dump.sh
Tests ETF can be dumped with good data while STM is the only trace source. All ftrace,
hardware events are disabled while single string data is sent over STM. Once trailing
zeros of output file are truncated, size is compared with reference size for same
input data.

7. stm/stm_etr_dump.sh
Please refer to stm/stm_etf_dump.sh. Here ETR is the trace sink instead of ETM.

8. mult_trace/mult_source_enable.sh
Tests that multiple CoreSight trace sources (ETM and STM) can be successfully enabled
at the same time.

9. mult_trace/mult_source_disable.sh
Tests that multiple CoreSight trace sources (ETM and STM) can be successfully disabled
at the same time.

10. sink_switch/sinkswitch.sh
Tests that the device can switch between multiple CoreSight tracesinks. Assumes etf,
etr and tpiu sinks are present.

11. sink_switch/etr_modes.sh
Tests that the modes of ETR can be changed from usb to memory and vice versa.

12. cs_adversary.sh
While having trace sources enabled, enables and disables all cores in a loop.

13. profile/cs_profile.sh
This test enables STM tracing and echoes fixed amount of data(4KB) over stm. It
calculates the time taken to send that data.

14. byte-cntr/byte_cntr.sh
Enables the byte counter feature and tests if data can be streamed to a file
using the feature. Sends 2MB of data and checks if the data collected is
greater than the sent data. Target supported: 8x26

Targets supported: 8974

Test output:
All tests prints:
"PASS: <message>" if the test case passes
 or
"FAIL: <message>" otherwise

Note: Tests assume /data partition exists
