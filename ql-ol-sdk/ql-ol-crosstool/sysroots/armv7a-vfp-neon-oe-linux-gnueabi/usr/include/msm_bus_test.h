/*
 * Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __MSM_BUS_TEST__
#define __MSM_BUS_TEST__

#include <linux/ioctl.h>
#define MSM_BUS_IOC_MAGIC 0x80
#define TEST_SLAVE_ID_KEY 512
#define NUM_USECASES 10
#define NUM_VECTORS 5

/**
 * The test enums do not contain the gateway master/slave IDs. Hence, they
 * differ from the enums used in boards file.
 * This is done so that random master/slave pairs can be created for
 * testing.
 */
enum msm_bus_test_fabric_master_type {
	MSM_BUS_TEST_MASTER_FIRST = 1,
	MSM_BUS_TEST_MASTER_AMPSS_M0 = 1,

	MSM_BUS_TEST_MASTER_SPS = 6,
	MSM_BUS_TEST_MASTER_ADM0_PORT0,
	MSM_BUS_TEST_MASTER_ADM0_PORT1,
	MSM_BUS_TEST_MASTER_LPASS_PROC = 11,
	MSM_BUS_TEST_MASTER_LPASS = 15,
	MSM_BUS_TEST_MASTER_MDP_PORT0 = 22,
	MSM_BUS_TEST_MASTER_HD_CODEC_PORT0 = 34,

	MSM_BUS_TEST_MASTER_QDSS_BAM = 53,
	MSM_BUS_TEST_MASTER_CRYPTO_CORE0 = 55,
	MSM_BUS_TEST_MASTER_SDCC_1 = 78,
};

enum msm_bus_test_fabric_slave_type {
	MSM_BUS_TEST_SLAVE_FIRST = TEST_SLAVE_ID_KEY,
	MSM_BUS_TEST_SLAVE_EBI_CH0 = TEST_SLAVE_ID_KEY,
};


struct msm_bus_test_vectors {
	int src; /* Master */
	int dst; /* Slave */
	unsigned int ab; /* Arbitrated bandwidth */
	unsigned int ib; /* Instantaneous bandwidth */
};

struct msm_bus_test_paths {
	int num_paths;
	struct msm_bus_test_vectors vectors[NUM_VECTORS];
};

struct msm_bus_test_scale_pdata {
	struct msm_bus_test_paths usecase[NUM_USECASES];
	int num_usecases;
	const char *name;
	unsigned int active_only;
};

struct msm_bus_test_cldata {
	struct msm_bus_test_scale_pdata pdata;
	uint32_t clid;
	uint32_t pdatah;
};

struct msm_bus_test_update_req_data {
	uint32_t clid;
	uint32_t index;
};
#define MSM_BUS_TEST_IOC_MAGIC 0x80

#define MSM_BUS_TEST_REG_CL _IOWR(MSM_BUS_TEST_IOC_MAGIC, 2, \
	struct msm_bus_test_cldata)

#define MSM_BUS_TEST_UNREG_CL _IOWR(MSM_BUS_TEST_IOC_MAGIC, 3, \
	struct msm_bus_test_cldata)

#define MSM_BUS_TEST_UPDATE_REQ _IOWR(MSM_BUS_TEST_IOC_MAGIC, 4, \
	struct msm_bus_test_update_req_data)

#define MSM_BUS_TEST_GET_CLID _IOWR(MSM_BUS_TEST_IOC_MAGIC, 5, uint32_t)
#define MSM_BUS_TEST_GET_RET _IOWR(MSM_BUS_TEST_IOC_MAGIC, 6, int)


#endif /*__MSM_BUS_TEST__*/
