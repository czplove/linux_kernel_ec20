
===============================================================================
IPI testing Documentation
===============================================================================


Usage: ipi_test.sh [OPTIONS]
       Runs the basic test for inter-processor interrupts (IPI)
       TEST: Call smp_call_function on each online CPU to test if all other CPUs
             could receive the interrupts.

OPTIONS:
  The kernel test support about the following parameters

  -t | --times <interation count> Sets how many times to test IPI between every
               two CPUs. For example, the following command will test IPI 10000
               times. It will call smp_call_function on each online CPU to test
               if all other CPUs could receive the interrupts.
               e.g.
               #./ipi_test.sh -t 10000
               If no option, the default value would be 10000.

  -r | --repeatability Sets 1000000 times as a large interation number to test
                       IPI.

TEST BEHAVIOR:
    * Online CPUs and ensure all the CPUs are not removed. Test will call
    * smp_call_function on each online CPU to test if all other CPUs could
    * receive the interrupts.

target support: all

Notes:
If the script returns 0 and prints "Test passed", then the test was successful.
If the script returns nonzero and prints "Test failed", then the test failed.
