	Watchdog Testing Documentation

Usage: msm_watchdog_test.sh TEST
       Runs the basic watchdog test

       TEST can be one of: apps_wdog_bite, sec_wdog_bite,
       sec_wdog_scm, apps_wdog_bark


OPTIONS:
  The module knows about the following parameters

  - apps_wdog_bite
  - sec_wdog_bite
  - sec_wdog_scm
  - apps_wdog_bark

  Setting any of these to 1 at insmod time will cause the
  corresponding test to be executed. For example, the following will
  run the apps watchdog bark test:

  # insmod msm_watchdog_test_module.ko apps_wdog_bark=1

  However, rather than inserting the module directly and worrying
  about supplying the correct parameters, you should use the wrapper
  script: msm_watchdog_test.sh.

TEST BEHAVIOR:
	* Test will force a watchdog timeout, which will reboot the system.
	  After obtaining rampdumps from debug mode, the ramdump parser
	  will produce the following results:

| Test           | Outcome                                                 |
|----------------+---------------------------------------------------------|
| sec_wdog_scm   | No watchdog (FIQ) message should be present.            |
|                | The CPU context should be dumped as part of             |
|                | the debug image output.                                 |
|----------------+---------------------------------------------------------|
| sec_wdog_bite  | Same as sec_wdog_scm.                                   |
|----------------+---------------------------------------------------------|
| apps_wdog_bark | No watchdog (FIQ) message should be present.            |
|----------------+---------------------------------------------------------|
| apps_wdog_bite | Watchdog/FIQ message should be present. The CPU context |
|                | should be dumped as part of the debug image output.     |


TARGETS:
	* 8974

NOTES:
If the script successfully returns and gives you back a shell prompt, the
test failed.
