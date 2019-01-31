Test:	hrtimer_test.sh

Usage:	hrtimer_test.sh [-n | --nominal] [-s | --stress][(-c | --custom) <loop>  <hrtimer_number>]
		-n | --nominal: create test threads according to cpu core numbers.
						open two hrtimers and run 1000 times for each core as default.
		-s | --stress: create test threads according to cpu core numbers and open 10 timer for each core.
						stress test about 1 hours.
		-c | --custom <loop> <hrtimer_number>: create test threads according to cpu core numbers.
						it will open <hrtimer_number> for each core and run <loop> times for each core.

Description:
hritmer test:
	1:	Auto create test threads according to cpu core numbers.
		And the number of threads for each core is tunable.
	2:	Can control the test time via test loops.
		Total=<loop>*20ms delay in kernel module+ 1s delay in sh to detect.
	3:	Test pass only when all timer is being correctly canceled or executed.
		For the normal test. Test will print "test failed". For the stress test,
		it is likely to crash the kernel or have the test pass printed.

For running tests, the hrtimer_test.sh script should be used.
The script loads hrtimer_test_module kernel module.

Target support: all

Notes:
A common usecase is to run this test in conjunction with the CPU hotplug test because:
	a. hrtimer test will create test threads for each CPU core.
	b. every hrtimer can switch between different cores.
	c. It is different cpu core threads hrtimer test, so cpu hot-plug test along with
	this test can strengthen the test.
mpdecision could in this target. In fact, it is better to have cpu hotplug along with this test.
Then it can find more crash issues related.

