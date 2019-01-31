	Kernel Module Signature Test Documentation

Usage:
./modsign_test.sh [-n | --nominal] [-r | --repeatability] [-a | --adversarial] [-s | --stress]
Runs the basic module signature verification test.

TEST DESCRITION:
The test verifies that for a kernel with module signature configs enabled:
* only signed modules can get loaded into the kernel & not the unsigned ones.
* signed module fails to load after being edited.
* signed module gets loaded successfully after reverting any edits made to it.

TARGETS:
The test is supported on all targets with the module signature configs enabled.

NOTES:
On Android adb shell, busybox installation is required for test to run,
since the test uses utilities like grep, sed, etc.
