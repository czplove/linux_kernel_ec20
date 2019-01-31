Test:	Testing super spinlock bug

Usage:	spinlock_test.sh [-n | --nominal] [-s | --stress][(-c | --custom) <iterations> <test time>]
		-n | --nominal:
			iterations: 1
			time per test: 5 (seconds)
		-s | --stress:
			iterations: 5
			time per test: 10 (seconds)
		-c | --custom:
			iterations: <iterations>
			time per test: <test time>

Example:
	#./spinlock_test.sh -n
	#./spinlock_test.sh -s
	#./spinlock_test.sh -c 3 10

Description:
	1: spinlock test module would schedule the test work function onto
	   each CPU according to number of cores.
	2: Spinlock test time and iteration value are elastic. Spinlock debugfs
	   would be available once the spinlock_test_module.ko is insmoded.
	   Spinlock Debugfs path: /sys/kernel/debug/spinlock-test/
		- inner_test_time: time per test (maximum: 100)
		- start: iterations (maximum: 100)
		  echo the iterations to 'start' to start the test and it would
		  be set as 0 when the test is done.
	3: CPU hotplug refcounter would be held during the test period, cores
	   are not allowed to be offline. It might hit thermal issue if the core
	   temperature goes very high.

For running tests, the spinlock_test.sh script should be used.

Target support: 8939, 8916
