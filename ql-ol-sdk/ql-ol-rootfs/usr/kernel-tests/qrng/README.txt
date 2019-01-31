=======================================================
qrng test program (qrngtest)
=======================================================

Description:
The qrngtest test program is used to verify that the hardware,
kernel driver and hwrng daemon are working correctly. By default
it opens the "/dev/random" device and does complete testing in a very
short period of time around ~3-5 seconds.

It can be run interactively for more test options.

It requires msm RNG driver (/dev/hw_random) for daemon.
It requires the system random device (/dev/random) for testing.

Parameters:
It will accept the following optional arguments:
	-n                 runs 100 iterations, each iteration reads 8KB of data.
	-a                 runs the adversarial tests
	-r                 runs the nominal and adversarial tests
	-s                 runs the nominal tests with 10 threads simultatiously.
	-c                 runs infinitely.
	-v                 provides extra debug prints.
	-d <device name>   random input device (default: /dev/random),
                           by default runs the nominal tests when this option
                           is selected.
	-h                 help (this page)\n";

The default parameters should work fine for automated testing.

Return:
It will return 0 if all test cases succeed otherwise it
returns a value < 0:

EXIT_NO_ERRROR			 0
EXIT_BAD_PARAMETER		-1
EXIT_COULD_NOT_OPEN_DEVICE	-2
EXIT_COULD_NOT_READ_DEVICE	-3
EXIT_TIMED_OUT_READING_DEVICE	-4
EXIT_RANDOM_TEST_FAILED		-5

Usage:
The qrngtest program by default does not require any parameters for automatic
testing. By default the program opens "/dev/random", this will test the
hardware, driver daemon and random number subsystem for Linux.

If for some reason the test fails, the user can specify,
"qrngtest -d /dev/hw_random" and this will only test the driver and hardware
to help troubleshoot any issues that may arise.

Details:
By default the test runs 100 iterations of the test. Each iteration reads in 8KB of
random data and checks it for randomness. The test will fail if the
"/dev/random" device or any other valid specified device is read blocked,
fails to give random numbers or just cannot open the device. Everything
must work correctly to get a return of 0 (zero).
