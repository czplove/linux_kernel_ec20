Test: iommutest

Usage: iommutest [OPTIONS]...

OPTIONS can be (defaults in parenthesis):
  -n,       --nominal		nominal test cases
  -a,       --adversarial	adversarial test cases
  -s,       --stress		stress test cases
  -r <n>,   --repeatability <n>	repeatability test cases. Specify # iterations
  -f,       --force		force to run on all IOMMUs even if secure.
  -b,       --basic		run basic VA2PA test instead of full VA2PA test
  -v        --verbose		run with debug messages on (off)
  -i <str>, --iommu <str>	specify which iommu to test (jpeg_iommu, mdp_iommu etc)

Description:
IOMMU is a hardware block that allows bus masters without an intergrated
MMU to use discontigous memory. The IOMMU provides virtual to physical
translation of addresses for the masters.
This test IOMMU page fault interrupts and local VA2PA translations.

For running tests, the iommutest.sh script should be used.
The script loads msm_iommu_test_module kernel module.

Target support: 8610, 8974v2, 8226, 8084
