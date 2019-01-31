Test: Abnormal reset Documentation

Usage: abnormal_reset_test.sh [-s | --software-reset] [-w | --wdog-timer]\
        [-p | --power-key] [-t | --tsensor-reset] [-h | --help]

Options:
The following options can be used:
-s | --pmic-software-reset                  trigger a PMIC software reset
-w | --pmic-wdt-reset                       trigger a PMIC wdog timer reset
-p | --power-key-reset                      trigger a power key press reset
-t | --tsensor-reset                        trigger a tsensor reset

Targets: 8916

Test Behavior:
	These tests are used for testing abnormal reset functionality on msm8916 device.
	Each test will force an abnormal reset, which will reboot the system.
	After obtaining RAM dumps from debug mode, the value of GCC_RESET_STATUS and
	PMIC PON/POFF registers can be extracted in order to indicate the reason for the system reset.


    ----------------------------------------------------------------------------------------------------
    | Test                 | Description                    |Reason                                    |
    |                      |                                |                                          |
    |----------------------|--------------------------------|------------------------------------------|
    |pmic-software-reset   | trigger a PMIC software reset( |PMIC_ABNORMAL_RESIN(GCC_RESET_STATUS)     |
    |                      | warm boot)                     |SOFT(WARM_RESET_REASON1)                  |
    |-------------------------------------------------------|------------------------------------------|
    |pmic-wdt-reset        | trigger a PMIC wdt bite after  |PMIC_ABNORMAL_RESIN(GCC_RESET_STATUS)     |
    |                      | the time specified in timer    |PMIC_WD(WARM_RESET_REASON1)               |
    |                      | (warm boot)                    |                                          |
    |-------------------------------------------------------|------------------------------------------|
    |power-key-reset       | trigger a warm reset after     |PMIC_ABNORMAL_RESIN(GCC_RESET_STATUS)     |
    |                      | press power key for the time   |KPDPWR_N(WARM_RESET_REASON1)              |
    |                      | specified in PMIC timer (warm  |                                          |
    |                      | boot)                          |                                          |
    |-------------------------------------------------------|------------------------------------------|
    |temperature senseor   | trigger a abnormal reset by    |TSENSE_RESET(GCC_RESET_STATUS)            |
    |reset                 | sensor when the temperature is |                                          |
    |                      | out of range (warm boot)       |                                          |
    |-------------------------------------------------------|------------------------------------------|
