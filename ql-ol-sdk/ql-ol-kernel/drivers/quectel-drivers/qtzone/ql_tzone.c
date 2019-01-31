/* 
 * Copyright (c) 2017-2018, The Quectel Wireless Solutions Co., Ltd.
 *
 * Authors
 *  carl.yin,       carl.yin@quectel.com
 *  running.qian    running.qian@quectel.com
 *
 * Fixs:
 *  2017-07-14      carl        initial
 *  2017-11-14      running     redisgned
 */
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <asm/cacheflush.h>
#include <asm/uaccess.h>

#include <soc/qcom/scm.h>

#include <linux/quectel/ql_tzone.h>
#include "ql_tzbsp_crypto_api.h"


struct ql_get_prng_data {
    uint8 *out_buf;
    uint32 out_buf_sz;
} __packed;

struct tz_scm_cmd {
    uint32 addr;
    uint32 size;
    uint32 resp_addr;
    uint32 resp_size;
} __packed;


//following taken from tz_syscall_pub.h
#define TZ_SVC_CRYPTO   10
#define PRNG_CMD_ID     0x01
#define TZ_SVC_SYM_ID   0x02
#define TZ_SVC_RSA_ID   0x03

#define TZ_KMALLOC_K(type) (type *)(kmalloc(sizeof(type), GFP_KERNEL))
#define TZ_KMALLOC_A(type) (type *)(kmalloc(sizeof(type), GFP_ATOMIC))


static void tz_flush_range(const void *p, unsigned int size)
{
    if ((unsigned int)p < PAGE_OFFSET)
        p = (const void *)phys_to_virt((unsigned int)p);
    //pr_info("%s(%p, %d)\n", __func__, p, size);
    dmac_flush_range(p, p + size);
}

static void tz_inv_range(const void *p, unsigned int size)
{
    if ((unsigned int)p < PAGE_OFFSET)
        p = (const void *)phys_to_virt((unsigned int)p);
    //pr_info("%s(%p, %d)\n", __func__, p, size);
    dmac_inv_range(p, p + size);
}


static void tz_clean_range(const void *p, unsigned int size)
{
    if ((unsigned int)p < PAGE_OFFSET)
        p = (const void *)phys_to_virt((unsigned int)p);
    //pr_info("%s(%p, %d)\n", __func__, p, size);
    dmac_clean_range(p, p + size);
}

/*** not support now 
static int scm_svc_version(uint32 *major)
{
    typedef struct{
        uint32 feature_id;
    } feature_version_req;

    typedef struct{
        uint32 version;
    } feature_version_rsp;
    #define TZBSP_GET_FEATURE_VERSION(major) ((major >> 22)& 0x3FF)

    feature_version_req feature_req;
    feature_version_rsp feature_rsp;
    int  ret = -1;
    uint32 tmp;

    feature_req.feature_id = 5;//TZBSP_FVER_SSD;

    ret = scm_call(6, 3, &feature_req, sizeof(feature_req), &feature_rsp, sizeof(feature_rsp));

    pr_info("---SVC VERION:scm:%d----\n", ret);
    if(!ret) {
        tmp = TZBSP_GET_FEATURE_VERSION(feature_rsp.version);
        pr_info("---SVC VERION:%d, %d----\n", tmp, feature_rsp.version);
        ret = copy_to_user(major, (void *)&tmp, sizeof(tmp));
    }

    return ret;
}


#ifdef TZ_FUSE_SUPPORT
static int tz_qfprom_read(tz_qfprom_read_row_t *usr_read_row)
{
    int rc, *user_data_vir, *api_status_vir, tmp_data;
    tz_qfprom_read_row_t *read_row, usr_read_row_k;


    pr_info("---FUSE READ11----\n");
    read_row = (tz_qfprom_read_row_t *)kmalloc(sizeof(tz_qfprom_read_row_t), GFP_ATOMIC);
    user_data_vir = (uint32_t *)kmalloc(sizeof(uint32_t) * 4, GFP_ATOMIC);
    api_status_vir = (uint32_t *)kmalloc(sizeof(uint32_t), GFP_ATOMIC);
    
    pr_info("---FUSE READ22----\n");
    if (!read_row->row_data || !read_row->row_data || !api_status_vir) {
        return -ENOMEM;
    }
    
    pr_info("---FUSE READ33----\n");
    rc = copy_from_user(&usr_read_row_k, usr_read_row, sizeof(tz_qfprom_read_row_t));
    rc = copy_from_user(&tmp_data, usr_read_row_k.row_data, sizeof(int32_t));

    read_row->row_address = usr_read_row_k.row_address;
    read_row->addr_type = 0;//usr_read_row->addr_type;
    read_row->row_data =  (uint32_t *)SCM_BUFFER_PHYS(user_data_vir);
    read_row->qfprom_api_status = (uint32_t *)SCM_BUFFER_PHYS(api_status_vir);
    
    pr_info("%s(): FUSE READ: read fuse row_address = %d, addr_type=%d, row_data=%d\n",
            __func__, usr_read_row_k.row_address, usr_read_row_k.addr_type, tmp_data);

    tz_flush_range(read_row, sizeof(tz_qfprom_read_row_t));
    tz_flush_range(user_data_vir, sizeof(uint32_t) * 4);

    rc = scm_call(8, 5, read_row, SCM_BUFFER_SIZE(tz_qfprom_read_row_t), NULL, 0);
    if (rc) {
        pr_info("%s(): scm_call failed to read fuse row: rc = %d\n", __func__, rc);
        goto out;
    }
    
    tz_inv_range(read_row, sizeof(tz_qfprom_read_row_t));
    tz_inv_range(user_data_vir, sizeof(uint32_t) * 4);

    pr_info("%s rc = %d row_data = %d qfprom_api_status = %d\n", __func__, rc, *(read_row->row_data), *(read_row->qfprom_api_status));
    rc = copy_to_user(usr_read_row->row_data, read_row->row_data, sizeof(uint32_t));

out:
    kfree(read_row);
    kfree(user_data_vir);
    kfree(api_status_vir);
    return rc;
}
****/


static int ql_tz_crypto_random_gen(ql_tz_random_data_t *user_data)
{
    struct ql_get_prng_data data;
    int rc;
    uint32 resp;
    uint8 *rnd;
    ql_tz_random_data_t user_data_k;

    rc = copy_from_user(&user_data_k, user_data, sizeof(ql_tz_random_data_t));
    rnd = kzalloc(user_data_k.data_size, GFP_KERNEL);   
    if (rnd == NULL) {
        return -ENOMEM;
    }

    data.out_buf = (uint8_t *)SCM_BUFFER_PHYS(rnd);
    data.out_buf_sz = user_data_k.data_size;//requset random len, don't over 512

    tz_flush_range(data.out_buf, user_data_k.data_size);

    rc = scm_call(TZ_SVC_CRYPTO, PRNG_CMD_ID, &data, sizeof(data), &resp, sizeof(resp));
    if (!rc) {
        printk("%s ----GOT random data: resp=%d\n", __func__, resp);
        tz_inv_range(data.out_buf, user_data_k.data_size);    
        rc = copy_to_user(user_data_k.data_ptr, rnd, user_data_k.data_size);
    }

    kfree(rnd);
    
    printk("%s ret=%d\n", __func__, rc);
    return rc;
}


static int ql_tz_crypto_aes_gen_key(ql_tz_aes_generate_key_t *user_data)
{
    int rc;
    ql_tz_aes_generate_key_t user_data_k;
    struct tz_scm_cmd scm_cmd_buf;
    uint32  key_material_len;
    uint8   *key_material;
    tz_storage_service_gen_key_cmd_t  *key_gen_req;
    tz_storage_service_gen_key_resp_t *key_gen_resp;

    rc = copy_from_user(&user_data_k, user_data, sizeof(ql_tz_aes_generate_key_t));

    key_material_len = user_data_k.key_size;//bit nums
    key_material = (uint8 *)(kmalloc(key_material_len, GFP_ATOMIC));
    key_gen_req  = TZ_KMALLOC_A(tz_storage_service_gen_key_cmd_t);
    key_gen_resp = TZ_KMALLOC_A(tz_storage_service_gen_key_resp_t);
    if (!key_material) {
        pr_info("%s(): GEN AES KEY: malloc failed2\n", __func__);
        rc = -ENOMEM;
        goto out;
    }
    
    key_gen_req->cmd_id = TZ_STOR_SVC_GENERATE_KEY;
    key_gen_req->key_blob.key_material = (void *)SCM_BUFFER_PHYS(key_material);
    key_gen_req->key_blob.key_material_len = key_material_len;

    pr_info("GENKEY req key_material= %p key_material_len = %d\n", key_material, key_material_len);

    memset(key_material, 0x00, key_material_len);
    memset(key_gen_resp, 0x00, sizeof(tz_storage_service_gen_key_resp_t));

    scm_cmd_buf.addr = SCM_BUFFER_PHYS(key_gen_req);
    scm_cmd_buf.size = SCM_BUFFER_SIZE(tz_storage_service_gen_key_cmd_t);
    scm_cmd_buf.resp_addr = SCM_BUFFER_PHYS(key_gen_resp);
    scm_cmd_buf.resp_size = SCM_BUFFER_SIZE(tz_storage_service_gen_key_resp_t);

    tz_flush_range(key_material, key_material_len);
    tz_flush_range(key_gen_req, sizeof(tz_storage_service_gen_key_cmd_t));
    tz_flush_range(key_gen_resp, sizeof(tz_storage_service_gen_key_resp_t));

    rc = scm_call(TZ_SVC_CRYPTO, TZ_SVC_SYM_ID, (void *)&scm_cmd_buf,
                  SCM_BUFFER_SIZE(scm_cmd_buf), NULL, 0);
    if (rc) {
        pr_info("%s(): GEN KEY: scm_call failed rc = %d\n", __func__, rc);
        goto out;
    }
    
    tz_inv_range(key_gen_resp, sizeof(tz_storage_service_gen_key_resp_t));
    tz_inv_range(key_material, key_material_len);

    pr_info("%s rc = %d status = %d key_blob_size = %d\n", __func__,
            rc, key_gen_resp->status, key_gen_resp->key_blob_size);

    if (key_gen_resp->status != 0) {
        pr_info("%s(): GEN AES Key : resp->status = %d\n",
            __func__, key_gen_resp->status);
        rc = -1;
        goto out;
    }
        
    if (key_gen_req->cmd_id != key_gen_resp->cmd_id) {
        pr_info("%s(): GEN KEY: ERROR - req cmd_id: %d, resp cmd_id: %d\n",
            __func__, key_gen_req->cmd_id, key_gen_resp->cmd_id);
        rc = -1;
        goto out;
    }

out:
    if (!rc) {
        put_user(key_gen_resp->key_blob_size, &user_data->key_size);
        rc = copy_to_user(user_data_k.key_ptr, key_material, key_gen_resp->key_blob_size);
    }

    if (key_material) kfree(key_material);
    if (key_gen_req)  kfree(key_gen_req);
    if (key_gen_resp) kfree(key_gen_resp);

    return rc;
}


static int ql_tz_crypto_aes_import_key(ql_tz_aes_import_key_t *user_data)
{
    struct tz_scm_cmd scm_cmd_buf;
    ql_tz_aes_import_key_t user_data_k;
    int rc = 0;

    uint32 key_material_len;
    uint8  *key_material;
    uint8  *aes_key_p;
    uint32 aes_key_len; 
    tz_storage_service_import_key_cmd_t *key_import_cmd;
    tz_storage_service_import_key_resp_t *key_import_resp;

    rc = copy_from_user(&user_data_k, user_data, sizeof(ql_tz_aes_import_key_t));

    key_material_len = user_data_k.key_size;//TZ_CRYPTO_SVC_AES256_KEY_SIZE + TZ_CRYPTO_SVC_NONCE_LEN + TZ_CRYPTO_SVC_MAC_LEN;
    aes_key_len = user_data_k.input_aes_size;
    key_material = kmalloc(key_material_len, GFP_ATOMIC);    
    aes_key_p = kmalloc(aes_key_len, GFP_ATOMIC);
    key_import_cmd = TZ_KMALLOC_A(tz_storage_service_import_key_cmd_t);
    key_import_resp = TZ_KMALLOC_A(tz_storage_service_import_key_resp_t);

    pr_info("%s key_material_len = %d, aes_key_len = %d\n", __func__,
            key_material_len, aes_key_len);
    if (key_material == NULL || aes_key_p == NULL) {
        pr_info("tz import key, kmalloc failed\n");
        rc = -ENOMEM;
    }

    rc = copy_from_user(aes_key_p, user_data_k.input_aes_ptr, aes_key_len);

    key_import_cmd->cmd_id = TZ_STOR_SVC_IMPORT_KEY;
    key_import_cmd->key_blob.key_material = (void*)SCM_BUFFER_PHYS(key_material);
    key_import_cmd->key_blob.key_material_len = key_material_len;
    key_import_cmd->aes_key_p = (void*)SCM_BUFFER_PHYS(aes_key_p);
    key_import_cmd->aes_key_len = aes_key_len;

    memset(key_import_resp, 0x0, sizeof(tz_storage_service_import_key_resp_t));

    scm_cmd_buf.addr = SCM_BUFFER_PHYS(key_import_cmd);
    scm_cmd_buf.size = SCM_BUFFER_SIZE(tz_storage_service_import_key_cmd_t);
    scm_cmd_buf.resp_addr = SCM_BUFFER_PHYS(key_import_resp);
    scm_cmd_buf.resp_size = SCM_BUFFER_SIZE(tz_storage_service_import_key_resp_t);

    tz_flush_range(key_import_cmd->aes_key_p, key_import_cmd->aes_key_len);
    tz_flush_range(key_import_cmd, sizeof(tz_storage_service_import_key_cmd_t));
    tz_flush_range(key_import_resp, sizeof(tz_storage_service_import_key_resp_t));
    tz_flush_range(key_import_cmd->key_blob.key_material, key_import_cmd->key_blob.key_material_len);

    rc = scm_call(TZ_SVC_CRYPTO, TZ_SVC_SYM_ID, (void *)&scm_cmd_buf, SCM_BUFFER_SIZE(scm_cmd_buf), NULL, 0);
    if (rc) {
        pr_info("%s(): scm_call failed to import AES key , ret = %d\n", __func__, rc);
        goto out;
    }
    
    tz_inv_range(key_import_resp, sizeof(tz_storage_service_import_key_resp_t));
    tz_inv_range(key_import_cmd->key_blob.key_material, key_import_resp->key_blob_size);

    pr_info("%s rc = %d status = %d key_blob_size = %d\n", __func__, rc,
            key_import_resp->status, key_import_resp->key_blob_size);
    
    if (key_import_resp->status != 0) {
        pr_info("%s(): scm_call did not return correct status = %d\n",
                __func__, key_import_resp->status);
        rc = -1;
        goto out;
    }
    
    if (key_import_cmd->cmd_id != key_import_resp->cmd_id) {
        pr_info("%s(): req cmd_id: %d, resp cmd_id: %d. ERROR.\n",
            __func__, key_import_cmd->cmd_id, key_import_resp->cmd_id);
        rc = -1;
        goto out;
    }

    if (!rc && key_import_resp->key_blob_size) {
        put_user(key_import_resp->key_blob_size, &user_data->key_size);
        rc = copy_to_user(user_data_k.key_ptr, key_material, key_import_resp->key_blob_size);
    }

out:
    if (key_material)    kfree(key_material);
    if (aes_key_p)       kfree(aes_key_p);
    if (key_import_cmd)  kfree(key_import_cmd);
    if (key_import_resp) kfree(key_import_resp);

    return rc;
}


static int ql_tz_crypto_aes_endec_data
(
    tz_storage_service_cmd_t cmd,
    ql_tz_aes_endecrypt_data_t *user_data
)
{
    int rc = 0;
    struct tz_scm_cmd scm_cmd_buf;
    ql_tz_aes_endecrypt_data_t user_data_k;

    void *input_data_ptr;
    void *output_data_ptr;
    void *key_material;
    tz_storage_service_seal_data_cmd_t *seal_data_cmd;
    tz_storage_service_seal_data_resp_t *seal_data_resp;
    

    rc = copy_from_user(&user_data_k, user_data, sizeof(ql_tz_aes_endecrypt_data_t));

    input_data_ptr  = kmalloc(user_data_k.input_data_size, GFP_ATOMIC);
    output_data_ptr = kmalloc(user_data_k.output_data_size, GFP_ATOMIC);
    key_material    = kmalloc(user_data_k.key_size, GFP_ATOMIC);
    
    seal_data_cmd = TZ_KMALLOC_A(tz_storage_service_seal_data_cmd_t);
    seal_data_resp = TZ_KMALLOC_A(tz_storage_service_seal_data_resp_t);

    if (!input_data_ptr || !output_data_ptr || !seal_data_cmd
        || !seal_data_resp || !key_material) {
        rc = -ENOMEM;
        goto out;
    }

    rc = copy_from_user(key_material, user_data_k.key_ptr, user_data_k.key_size);
    rc = copy_from_user(input_data_ptr, user_data_k.input_data_ptr, user_data_k.input_data_size);

    seal_data_cmd->cmd_id = cmd;
    seal_data_cmd->key_blob.key_material = (void *)SCM_BUFFER_PHYS(key_material);
    seal_data_cmd->key_blob.key_material_len = user_data_k.key_size;
    seal_data_cmd->plain_data = (void *)SCM_BUFFER_PHYS(input_data_ptr);
    seal_data_cmd->plain_data_len = user_data_k.input_data_size;
    seal_data_cmd->output_buffer = (void *)SCM_BUFFER_PHYS(output_data_ptr);
    seal_data_cmd->output_len = user_data_k.output_data_size;

    pr_info("%s: key_material_len= %d, plain_data_len = %d\n", __func__,
          user_data_k.key_size, user_data_k.input_data_size);
    memset(seal_data_resp, 0x00, sizeof(struct tz_storage_service_seal_data_resp));

    scm_cmd_buf.addr = SCM_BUFFER_PHYS(seal_data_cmd);
    scm_cmd_buf.size = SCM_BUFFER_SIZE(tz_storage_service_seal_data_cmd_t);
    scm_cmd_buf.resp_addr = SCM_BUFFER_PHYS(seal_data_resp);
    scm_cmd_buf.resp_size = SCM_BUFFER_SIZE( tz_storage_service_seal_data_resp_t);

    tz_flush_range(seal_data_cmd->key_blob.key_material, seal_data_cmd->key_blob.key_material_len);
    tz_flush_range(seal_data_cmd->plain_data, seal_data_cmd->plain_data_len);
    tz_flush_range(seal_data_cmd->output_buffer, seal_data_cmd->output_len);
    tz_flush_range(seal_data_cmd, sizeof(tz_storage_service_seal_data_cmd_t));
    tz_flush_range(seal_data_resp, sizeof(tz_storage_service_seal_data_resp_t));

    rc = scm_call(TZ_SVC_CRYPTO, TZ_SVC_SYM_ID, (void *)&scm_cmd_buf, SCM_BUFFER_SIZE(scm_cmd_buf), NULL, 0);
    if (rc) {
        pr_info("%s(): DATA SEAL: scm_call failed to Seal Data: rc = %d\n", __func__, rc);
        goto out;
    }

    tz_inv_range(seal_data_resp, sizeof(tz_storage_service_seal_data_resp_t));
    tz_inv_range(seal_data_cmd->output_buffer, seal_data_cmd->output_len);

    pr_info("%s: rc = %d status = %d sealed_data_len = %d\n", __func__,
        rc, seal_data_resp->status, seal_data_resp->sealed_data_len);
        
    if (seal_data_resp->status != 0) {
        pr_info("%s(): DATA SEAL: scm_call return incorrect status = %d\n",
            __func__, seal_data_resp->status);
        rc = -1;
        goto out;
    }
    
    if (seal_data_cmd->cmd_id != seal_data_resp->cmd_id) {
        pr_info("%s(): DATA SEAL: ERROR - req cmd_id: %d, resp cmd_id: %d\n",
            __func__, seal_data_cmd->cmd_id, seal_data_resp->cmd_id);
        rc = -1;
        goto out;
    }
    
    if (!rc) {
        put_user(seal_data_resp->sealed_data_len, &user_data->output_data_size);
        rc = copy_to_user(user_data_k.output_data_ptr, output_data_ptr, seal_data_resp->sealed_data_len);
    }

out: 
    if (input_data_ptr)     kfree(input_data_ptr);
    if (output_data_ptr)    kfree(output_data_ptr);
    if (key_material)       kfree(key_material);
    if (seal_data_cmd)      kfree(seal_data_cmd);
    if (seal_data_resp)     kfree(seal_data_resp);

    return rc;
}


static int ql_tz_crypto_rsa_gen_keypair(ql_tz_rsa_generate_key_t *user_data)
{
    int rc = 0;
    ql_tz_rsa_generate_key_t user_key_k;

    uint32 key_material_len;
    crypto_rsa_key_type *key_material;
    struct tz_scm_cmd scm_cmd_buf;
    struct crypto_asym_gen_keypair_cmd *gen_keypair_cmd;
    struct crypto_asym_gen_keypair_resp *gen_keypair_resp;

    rc = copy_from_user(&user_key_k, user_data, sizeof(ql_tz_rsa_generate_key_t));
    
    key_material_len = sizeof(crypto_rsa_key_type);//user_key_k.rsa_key_blob.key_material_len;
    key_material = TZ_KMALLOC_A(crypto_rsa_key_type);
    gen_keypair_cmd = TZ_KMALLOC_A(crypto_asym_gen_keypair_cmd_t);
    gen_keypair_resp = TZ_KMALLOC_A(crypto_asym_gen_keypair_resp_t);

    if (!key_material || !gen_keypair_cmd || !gen_keypair_resp) {
        return -ENOMEM;
        goto out;
    }

    gen_keypair_cmd->cmd_id = CRYPTO_STORAGE_GENERATE_KEY;
    gen_keypair_cmd->key_blob.key_material = (void *) SCM_BUFFER_PHYS(key_material);
    gen_keypair_cmd->key_blob.key_material_len = key_material_len;

    gen_keypair_cmd->rsa_params.modulus_size = user_key_k.modulus_size;
    gen_keypair_cmd->rsa_params.public_exponent = user_key_k.public_exponent;
    gen_keypair_cmd->rsa_params.digest_pad_type = user_key_k.digest_pad_type;
    memset(gen_keypair_resp, 0x00, sizeof(*gen_keypair_resp));

    scm_cmd_buf.addr = SCM_BUFFER_PHYS(gen_keypair_cmd);
    scm_cmd_buf.size = SCM_BUFFER_SIZE(crypto_asym_gen_keypair_cmd_t);
    scm_cmd_buf.resp_addr = SCM_BUFFER_PHYS(gen_keypair_resp);
    scm_cmd_buf.resp_size = SCM_BUFFER_SIZE(crypto_asym_gen_keypair_resp_t);

    tz_flush_range(key_material, key_material_len);
    tz_flush_range(gen_keypair_cmd, sizeof(crypto_asym_gen_keypair_cmd_t));
    tz_flush_range(gen_keypair_resp, sizeof(crypto_asym_gen_keypair_resp_t));
    
    rc = scm_call(TZ_SVC_CRYPTO, TZ_SVC_RSA_ID,
         (void *)&scm_cmd_buf, SCM_BUFFER_SIZE(scm_cmd_buf), NULL, 0);
    if (rc) {
        pr_info("%s(): scm_call failed to gen RSA key : rc = %d\n", __func__, rc);
        goto out;
    }
    pr_info("%s(): RSA GEN KEY: scm_call SUCCESS!\n", __func__);
        
    tz_inv_range(gen_keypair_resp, sizeof(*gen_keypair_resp));
    tz_inv_range(key_material, key_material_len);

    pr_info("%s: rc = %d status = %d key_blob_size = %d\n", __func__,
        rc, gen_keypair_resp->status, gen_keypair_resp->key_blob_size);

    pr_info("magic=%d, ver=%d, dig_padding=%d, mod_size=%d, pub_expsize=%d, enc_priexp_size=%d\n",
        key_material->magic_num, key_material->version,
        key_material->digest_padding, key_material->modulus_size,
        key_material->public_exponent_size,
        key_material->encrypted_private_exponent_size);
        
    if (gen_keypair_resp->status != 0) {
        pr_info("%s(): scm_call did not return correct status status = %d\n",
            __func__, gen_keypair_resp->status);
        rc = -1;
        goto out;
    }
    
    if (gen_keypair_cmd->cmd_id != gen_keypair_resp->cmd_id) {
        pr_info("%s(): req cmd_id: %d, resp cmd_id: %d. ERROR.\n", __func__,
            gen_keypair_cmd->cmd_id, gen_keypair_resp->cmd_id);
        rc = -1;
        goto out;
    }

    if (!rc) {
        rc = copy_to_user(user_key_k.rsa_key_blob, key_material, gen_keypair_resp->key_blob_size);
        pr_info("%s(): ##### RSA GEN KEY END #####\n", __func__);
    }

out:
    if (key_material)     kfree(key_material);
    if (gen_keypair_cmd)  kfree(gen_keypair_cmd);
    if (gen_keypair_resp) kfree(gen_keypair_resp);

    return rc;
}


static int ql_tz_crypto_rsa_sign_data(ql_tz_rsa_sign_verify_data_t *user_data)
{
    int rc = 0;
    struct tz_scm_cmd scm_cmd_buf;
    ql_tz_rsa_sign_verify_data_t user_data_k;

    uint32 key_material_len;
    crypto_rsa_key_type *key_material;
    void *input_data_ptr;
    void *signature_ptr;
    crypto_asym_sign_data_cmd_t  *sign_data_cmd;
    crypto_asym_sign_data_resp_t *sign_data_resp;

    rc = copy_from_user(&user_data_k, user_data, sizeof(ql_tz_rsa_sign_verify_data_t));

    key_material_len = sizeof(crypto_rsa_key_type);

    key_material    = TZ_KMALLOC_A(crypto_rsa_key_type);
    sign_data_cmd   = TZ_KMALLOC_A(crypto_asym_sign_data_cmd_t);
    sign_data_resp  = TZ_KMALLOC_A(crypto_asym_sign_data_resp_t);
    input_data_ptr  = kmalloc(user_data_k.input_data_size, GFP_ATOMIC);
    signature_ptr   = kmalloc(user_data_k.signature_length, GFP_ATOMIC);

    if (!key_material || !input_data_ptr || !signature_ptr
        || !sign_data_cmd || !sign_data_resp) {
        rc = -ENOMEM;
        goto out;
    }    

    rc = copy_from_user(key_material, user_data_k.rsa_key_blob, key_material_len);
    rc = copy_from_user(input_data_ptr, user_data_k.input_data_ptr, user_data_k.input_data_size);
    
    sign_data_cmd->cmd_id = CRYPTO_STORAGE_SIGN_DATA;
    sign_data_cmd->key_blob.key_material = (void *)SCM_BUFFER_PHYS(key_material);
    sign_data_cmd->key_blob.key_material_len = key_material_len;
    sign_data_cmd->data = (uint8_t *)SCM_BUFFER_PHYS(input_data_ptr);
    sign_data_cmd->data_len = user_data_k.input_data_size;
    sign_data_cmd->signeddata = (uint8_t *)SCM_BUFFER_PHYS(signature_ptr);
    sign_data_cmd->signeddata_len = user_data_k.signature_length;

    memset(sign_data_resp, 0x00, sizeof(crypto_asym_sign_data_resp_t));

    /* Clean and invalidate the specified virtual address range. */
    tz_flush_range(input_data_ptr, user_data_k.input_data_size);
    tz_flush_range(signature_ptr, user_data_k.signature_length);
    tz_flush_range(key_material, key_material_len);
    tz_flush_range(sign_data_cmd, sizeof(crypto_asym_sign_data_cmd_t));
    tz_flush_range(sign_data_resp, sizeof(crypto_asym_sign_data_resp_t));

    scm_cmd_buf.addr = SCM_BUFFER_PHYS(sign_data_cmd);
    scm_cmd_buf.size = SCM_BUFFER_SIZE(crypto_asym_sign_data_cmd_t);
    scm_cmd_buf.resp_addr = SCM_BUFFER_PHYS(sign_data_resp);
    scm_cmd_buf.resp_size = SCM_BUFFER_SIZE(crypto_asym_sign_data_resp_t);
    
    rc = scm_call(TZ_SVC_CRYPTO, TZ_SVC_RSA_ID,
        (void *)&scm_cmd_buf, SCM_BUFFER_SIZE(scm_cmd_buf), NULL, 0);
    if (rc) {
        pr_info("%s(): scm_call failed to sign data using RSA key: rc = %d\n",
            __func__, rc);
        goto out;
    }
    
    tz_inv_range(input_data_ptr, user_data_k.input_data_size);
    //tz_clean_range(input_data_ptr, user_data_k.input_data_size);
    tz_inv_range(sign_data_resp, sizeof(crypto_asym_sign_data_resp_t));
    //tz_clean_range(sign_data_resp, sizeof(crypto_asym_sign_data_resp_t));
    tz_inv_range(signature_ptr, user_data_k.signature_length);
    //tz_clean_range(signature_ptr, user_data_k.signature_length);

    pr_info("%s: rc = %d status = %d sig_len = %d\n", __func__,
            rc, sign_data_resp->status, sign_data_resp->sig_len);
    
    if (sign_data_resp->status != 0) {
        pr_info("%s(): scm_call did not return correct status = %d\n",
                __func__, sign_data_resp->status);
        rc = -1;
        goto out;
    }
            
    if (sign_data_cmd->cmd_id != sign_data_resp->cmd_id) {
        pr_info("%s(): req cmd_id: %d, resp cmd_id: %d. ERROR.\n",
            __func__, sign_data_cmd->cmd_id, sign_data_resp->cmd_id);
        rc = -1;
        goto out;
    }

    if (!rc) {
        put_user(sign_data_resp->sig_len, &user_data->signature_length);
        rc = copy_to_user(user_data_k.signature_ptr, signature_ptr, sign_data_resp->sig_len);
    }

out:
    if (input_data_ptr) kfree(input_data_ptr);
    if (signature_ptr)  kfree(signature_ptr);
    if (key_material)   kfree(key_material);
    if (sign_data_cmd)  kfree(sign_data_cmd);
    if (sign_data_resp) kfree(sign_data_resp);
    
    return rc;
}


static int ql_tz_crypto_rsa_verify_data(ql_tz_rsa_sign_verify_data_t *user_data)
{
    int rc = 0;
    struct tz_scm_cmd scm_cmd_buf;
    ql_tz_rsa_sign_verify_data_t user_data_k;

    crypto_asym_verify_data_cmd_t  *verify_data_cmd;
    crypto_asym_verify_data_resp_t *verify_data_resp;
    uint32 key_material_len;
    crypto_rsa_key_type *key_material;
    void *input_data_ptr;
    void *signature_ptr;


    rc = copy_from_user(&user_data_k, user_data, sizeof(ql_tz_rsa_sign_verify_data_t));

    key_material_len = sizeof(crypto_rsa_key_type);//user_data_k.key_size;
    key_material =  TZ_KMALLOC_A(crypto_rsa_key_type);
    verify_data_cmd = TZ_KMALLOC_A(crypto_asym_verify_data_cmd_t);
    verify_data_resp = TZ_KMALLOC_A(crypto_asym_verify_data_resp_t);
    input_data_ptr = kmalloc(user_data_k.input_data_size, GFP_ATOMIC);
    signature_ptr = kmalloc(user_data_k.signature_length, GFP_ATOMIC);

    if (!key_material || !verify_data_cmd || !verify_data_resp ||
        !input_data_ptr || !signature_ptr) {
        rc = -ENOMEM;
        goto out;
    }    

    pr_info("%s: key_material_len = %d, input_data_size =%d, signature_length =%d\n", 
        __func__, key_material_len, user_data_k.input_data_size,
        user_data_k.signature_length);

    rc = copy_from_user(key_material, user_data_k.rsa_key_blob, key_material_len);
    rc = copy_from_user(input_data_ptr, user_data_k.input_data_ptr, user_data_k.input_data_size);
    rc = copy_from_user(signature_ptr, user_data_k.signature_ptr, user_data_k.signature_length);
    
    verify_data_cmd->cmd_id = CRYPTO_STORAGE_VERIFY_DATA;
    verify_data_cmd->key_blob.key_material = (crypto_rsa_key_type *)SCM_BUFFER_PHYS(key_material);
    verify_data_cmd->key_blob.key_material_len = key_material_len;
    verify_data_cmd->signed_data = (uint8_t *)SCM_BUFFER_PHYS(input_data_ptr);
    verify_data_cmd->signed_dlen = user_data_k.input_data_size;
    verify_data_cmd->signature = (uint8_t *)SCM_BUFFER_PHYS(signature_ptr);
    verify_data_cmd->slen = user_data_k.signature_length;

    memset(verify_data_resp, 0x00, sizeof(struct crypto_asym_verify_data_resp));

    tz_flush_range(key_material, key_material_len);
    tz_flush_range(input_data_ptr, user_data_k.input_data_size);
    tz_flush_range(signature_ptr, user_data_k.signature_length);
    tz_flush_range(verify_data_cmd, sizeof(crypto_asym_sign_data_cmd_t));
    tz_flush_range(verify_data_resp, sizeof(crypto_asym_sign_data_resp_t));

    scm_cmd_buf.addr = SCM_BUFFER_PHYS(verify_data_cmd);
    scm_cmd_buf.size = SCM_BUFFER_SIZE(crypto_asym_verify_data_cmd_t);
    scm_cmd_buf.resp_addr = SCM_BUFFER_PHYS(verify_data_resp);
    scm_cmd_buf.resp_size = SCM_BUFFER_SIZE(crypto_asym_verify_data_resp_t);
    
    rc = scm_call(TZ_SVC_CRYPTO, TZ_SVC_RSA_ID, (void *)&scm_cmd_buf, SCM_BUFFER_SIZE(scm_cmd_buf), NULL, 0);
    if (rc) {
        pr_info("%s(): scm_call failed to sign data using RSA key: rc = %d\n", __func__, rc);
        goto out;
    }
    
    tz_inv_range(verify_data_resp, sizeof(crypto_asym_verify_data_resp_t));
    pr_info("%s: rc = %d status = %d\n", __func__, rc, verify_data_resp->status);

    if (verify_data_resp->status != 0) {
        pr_info("%s(): scm_call did not return correct status = %d\n",
            __func__, verify_data_resp->status);
        rc = -1;
        goto out;
    }
            
    if (verify_data_cmd->cmd_id != verify_data_resp->cmd_id) {
        pr_info("%s(): req cmd_id: %d, resp cmd_id: %d. ERROR.\n",
            __func__, verify_data_cmd->cmd_id, verify_data_resp->cmd_id);
        rc = -1;
        goto out;
    }
out:
    if (input_data_ptr)   kfree(input_data_ptr);
    if (signature_ptr)    kfree(signature_ptr);
    if (key_material)     kfree(key_material);
    if (verify_data_cmd)  kfree(verify_data_cmd);
    if (verify_data_resp) kfree(verify_data_resp);
    
    return rc;
}


static int ql_tz_crypto_rsa_import_keypair(ql_tz_rsa_keypair_import_t *user_data)
{
    int rc;
    ql_tz_rsa_keypair_import_t user_data_k;
    struct tz_scm_cmd scm_cmd_buf;

    crypto_storage_rsa_import_key_cmd_t *rsa_import_key_req;
    crypto_storage_rsa_import_key_resp_t *rsa_import_key_resp;
    crypto_rsa_key_type *key_material;
    uint32 key_material_len;
    uint32 modulus_data_size;  
    uint32 public_exponent_size;
    uint32 private_exponent_size;

    rc = copy_from_user(&user_data_k, user_data, sizeof(ql_tz_rsa_keypair_import_t));

    public_exponent_size = user_data_k.in_pubExpSize;
    modulus_data_size = user_data_k.in_modulusSize;
    private_exponent_size = user_data_k.in_privExpSize;
    key_material_len = sizeof(crypto_rsa_key_type);//user_data_k.rsa_key_blob.key_material_len;

    rsa_import_key_req = TZ_KMALLOC_A(crypto_storage_rsa_import_key_cmd_t);
    rsa_import_key_resp = TZ_KMALLOC_A(crypto_storage_rsa_import_key_resp_t);
    key_material = kmalloc(key_material_len, GFP_ATOMIC);

    if (!rsa_import_key_req || !rsa_import_key_resp || !key_material)  {
        pr_err("%s(): GEN KEY: Memory allocation failed\n", __func__);
        rc = -ENOMEM;
        goto out;
    }

    rc = copy_from_user(key_material, user_data_k.rsa_key_blob, key_material_len);
    rc = copy_from_user(rsa_import_key_req->modulus, user_data_k.in_modulus, modulus_data_size);
    rc = copy_from_user(rsa_import_key_req->public_exponent, user_data_k.in_pubExp, public_exponent_size);
    rc = copy_from_user(rsa_import_key_req->private_exponent, user_data_k.in_privExp, private_exponent_size);

    rsa_import_key_req->cmd_id = CRYPTO_STORAGE_IMPORT_KEY;
    rsa_import_key_req->key_blob.key_material = (void *)SCM_BUFFER_PHYS(key_material);
    rsa_import_key_req->key_blob.key_material_len = key_material_len;   
    rsa_import_key_req->modulus_size = modulus_data_size;
    rsa_import_key_req->public_exponent_size = public_exponent_size;
    rsa_import_key_req->private_exponent_size = private_exponent_size;
    rsa_import_key_req->digest_pad_type = user_data_k.in_padding;

    scm_cmd_buf.addr = SCM_BUFFER_PHYS(rsa_import_key_req);
    scm_cmd_buf.size = SCM_BUFFER_SIZE(crypto_storage_rsa_import_key_cmd_t);
    scm_cmd_buf.resp_addr = SCM_BUFFER_PHYS(rsa_import_key_resp);
    scm_cmd_buf.resp_size = SCM_BUFFER_SIZE(crypto_storage_rsa_import_key_resp_t);

    memset(rsa_import_key_resp, 0x00, sizeof(crypto_storage_rsa_import_key_resp_t));

    tz_flush_range(key_material, key_material_len);
    tz_flush_range(rsa_import_key_req, sizeof(crypto_storage_rsa_import_key_cmd_t));
    tz_flush_range(rsa_import_key_resp, sizeof(crypto_storage_rsa_import_key_resp_t));

    rc = scm_call(TZ_SVC_CRYPTO, TZ_SVC_RSA_ID,
        (void *)&scm_cmd_buf, SCM_BUFFER_SIZE(scm_cmd_buf), NULL, 0);
    if (rc) {
        pr_info("%s(): scm_call failed to export public RSA key: rc = %d\n", __func__, rc);
        goto out;
    }
    
    tz_inv_range(rsa_import_key_resp, sizeof(crypto_storage_rsa_import_key_resp_t));
    tz_inv_range(rsa_import_key_req->key_blob.key_material, key_material_len);

    pr_info("%s: rc = %d status = %d, key_material_len = %d\n", __func__, rc,
        rsa_import_key_resp->status, rsa_import_key_req->key_blob.key_material_len);

    if (rsa_import_key_resp->status != 0) {
        pr_info("%s(): scm_call did not return correct status = %d\n",
            __func__, rsa_import_key_resp->status);
        rc = -1;
        goto out;
    }
    
    if (rsa_import_key_req->cmd_id != rsa_import_key_resp->cmd_id) {
        pr_info("%s(): req cmd_id: %d, resp cmd_id: %d. ERROR.\n",
            __func__, rsa_import_key_req->cmd_id, rsa_import_key_resp->cmd_id);
        rc = -1;
        goto out;
    }

    if (!rc) {
        rc = copy_to_user(user_data_k.rsa_key_blob, key_material, key_material_len);
    }
    
out:
    if (rsa_import_key_req)  kfree(rsa_import_key_req);
    if (rsa_import_key_resp) kfree(rsa_import_key_resp);
    if (key_material)        kfree(key_material);
        
    return rc;
}


static int ql_tz_crypto_rsa_import_pkcs8(ql_tz_rsa_pkcs8_import_t *user_data)
{
#if 0
    int rc;
    ql_tz_rsa_pkcs8_import_t user_data_k;
    struct tz_scm_cmd scm_cmd_buf;

    crypto_asym_import_keypair_cmd_t  *rsa_import_key_req;
    crypto_asym_import_keypair_resp_t *rsa_import_key_resp;
    crypto_rsa_key_type *key_material;
    uint32 key_material_len;
    uint32 *pkcs8_key;
    uint32 pkcs8_key_len;

    rc = copy_from_user(&user_data_k, user_data, sizeof(ql_tz_rsa_pkcs8_import_t));

    pkcs8_key_len = user_data_k.in_pkcs8_key_len;
    key_material_len = sizeof(crypto_rsa_key_type);//user_data_k.rsa_key_blob.key_material_len;

    rsa_import_key_req = TZ_KMALLOC_A(crypto_asym_import_keypair_cmd_t);
    rsa_import_key_resp = TZ_KMALLOC_A(crypto_asym_import_keypair_resp_t);
    key_material = kmalloc(key_material_len, GFP_ATOMIC);
    pkcs8_key = kmalloc(pkcs8_key_len, GFP_ATOMIC);

    if (!rsa_import_key_req || !rsa_import_key_resp || !key_material || !pkcs8_key)  {
        pr_err("%s(): GEN KEY: Memory allocation failed\n", __func__);
        rc = -ENOMEM;
        goto out;
    }

    rc = copy_from_user(pkcs8_key, user_data_k.in_pkcs8_key, pkcs8_key_len);
   
    rsa_import_key_req->cmd_id = CRYPTO_STORAGE_IMPORT_KEY;
    rsa_import_key_req->pkcs8_key = (int)SCM_BUFFER_PHYS(pkcs8_key);
    rsa_import_key_req->pkcs8_key_len = pkcs8_key_len;

    rsa_import_key_resp->key_blob.key_material = (void *)SCM_BUFFER_PHYS(key_material);
    rsa_import_key_resp->key_blob.key_material_len = key_material_len;


    scm_cmd_buf.addr = SCM_BUFFER_PHYS(rsa_import_key_req);
    scm_cmd_buf.size = SCM_BUFFER_SIZE(crypto_asym_import_keypair_cmd_t);
    scm_cmd_buf.resp_addr = SCM_BUFFER_PHYS(rsa_import_key_resp);
    scm_cmd_buf.resp_size = SCM_BUFFER_SIZE(crypto_asym_import_keypair_resp_t);

    //memset(rsa_import_key_resp, 0x00, sizeof(crypto_asym_import_keypair_resp_t));

    tz_flush_range(key_material, key_material_len);
    tz_flush_range(rsa_import_key_req, sizeof(crypto_asym_import_keypair_cmd_t));
    tz_flush_range(rsa_import_key_resp, sizeof(crypto_asym_import_keypair_resp_t));

    rc = scm_call(TZ_SVC_CRYPTO, TZ_SVC_RSA_ID,
        (void *)&scm_cmd_buf, SCM_BUFFER_SIZE(scm_cmd_buf), NULL, 0);
    if (rc) {
        pr_info("%s(): scm_call failed to export public RSA key: rc = %d\n", __func__, rc);
        goto out;
    }
    
    tz_inv_range(rsa_import_key_resp, sizeof(crypto_asym_import_keypair_cmd_t));
    tz_inv_range(rsa_import_key_resp->key_blob.key_material, key_material_len);

    pr_info("%s: rc = %d status = %d, key_material_len = %d\n", __func__, rc,
        rsa_import_key_resp->status, rsa_import_key_resp->key_blob.key_material_len);

    if (rsa_import_key_resp->status != 0) {
        pr_info("%s(): scm_call did not return correct status = %d\n",
            __func__, rsa_import_key_resp->status);
        rc = -1;
        goto out;
    }
    
    if (rsa_import_key_req->cmd_id != rsa_import_key_resp->cmd_id) {
        pr_info("%s(): req cmd_id: %d, resp cmd_id: %d. ERROR.\n",
            __func__, rsa_import_key_req->cmd_id, rsa_import_key_resp->cmd_id);
        rc = -1;
        goto out;
    }

    if (!rc) {
        put_user(rsa_import_key_resp->key_blob.key_material_len,
                 &user_data->rsa_key_blob.key_material_len
                );
        rc = copy_to_user(user_data_k.rsa_key_blob.key_material,
                          key_material,
                          key_material_len
                         );
    }
    
out:
    if (rsa_import_key_req)  kfree(rsa_import_key_req);
    if (rsa_import_key_resp) kfree(rsa_import_key_resp);
    if (key_material)        kfree(key_material);
    if (pkcs8_key)           kfree(pkcs8_key);
        
    return rc;
#else
    return -1;
#endif
}



static int ql_tz_crypto_rsa_export_pubkey(ql_tz_rsa_public_key_export_t *user_data)
{
    int rc;
    ql_tz_rsa_public_key_export_t user_data_k;
    struct tz_scm_cmd scm_cmd_buf;
    crypto_storage_rsa_export_key_cmd_t *rsa_export_key_req;
    crypto_storage_rsa_export_key_resp_t *rsa_export_key_resp;

    uint32 key_material_len;
    crypto_rsa_key_type *key_material;
    uint32 modulus_data_size;
    void  *modulus_data_ptr;
    uint32 public_exponent_size;
    void  *public_exponent_ptr;

    rc = copy_from_user(&user_data_k, user_data, sizeof(ql_tz_rsa_public_key_export_t));

    public_exponent_size = user_data_k.public_exponent_size;
    modulus_data_size = user_data_k.modulus_data_size;
    key_material_len = sizeof(crypto_rsa_key_type);

    rsa_export_key_req = TZ_KMALLOC_A(crypto_storage_rsa_export_key_cmd_t);
    rsa_export_key_resp = TZ_KMALLOC_A(crypto_storage_rsa_export_key_resp_t);
    key_material = kmalloc(key_material_len, GFP_ATOMIC);
    modulus_data_ptr = kmalloc(user_data_k.modulus_data_size, GFP_ATOMIC);
    public_exponent_ptr = kmalloc(user_data_k.public_exponent_size, GFP_ATOMIC);

    if (!rsa_export_key_req || !rsa_export_key_resp || !modulus_data_ptr
        || !public_exponent_ptr)  {
        pr_err("%s(): GEN KEY: Memory allocation failed\n", __func__);
        rc = -ENOMEM;
        goto out;
    }

    rc = copy_from_user(key_material, user_data_k.rsa_key_blob, key_material_len);

    rsa_export_key_req->cmd_id = CRYPTO_STORAGE_EXPORT_PUBKEY;
    rsa_export_key_req->key_blob.key_material = (void *)SCM_BUFFER_PHYS(key_material);
    rsa_export_key_req->key_blob.key_material_len = key_material_len;
    rsa_export_key_req->export_format = CRYPTO_STORAGE_FORMAT_RAW_BYTES; //fixed
    rsa_export_key_req->modulus = (uint8_t *)SCM_BUFFER_PHYS(modulus_data_ptr);
    rsa_export_key_req->modulus_size = modulus_data_size;
    rsa_export_key_req->public_exponent = (uint8_t *)SCM_BUFFER_PHYS(public_exponent_ptr);
    rsa_export_key_req->public_exponent_size = public_exponent_size;

    scm_cmd_buf.addr = SCM_BUFFER_PHYS(rsa_export_key_req);
    scm_cmd_buf.size = SCM_BUFFER_SIZE(crypto_storage_rsa_export_key_cmd_t);
    scm_cmd_buf.resp_addr = SCM_BUFFER_PHYS(rsa_export_key_resp);
    scm_cmd_buf.resp_size = SCM_BUFFER_SIZE(crypto_storage_rsa_export_key_resp_t);

    memset(rsa_export_key_resp, 0x00, sizeof(struct crypto_storage_rsa_export_key_resp));

    tz_flush_range(key_material, key_material_len);
    tz_flush_range(public_exponent_ptr, public_exponent_size);
    tz_flush_range(modulus_data_ptr, modulus_data_size);
    tz_flush_range(rsa_export_key_req, sizeof(crypto_storage_rsa_export_key_cmd_t));
    tz_flush_range(rsa_export_key_resp, sizeof(crypto_storage_rsa_export_key_resp_t));

    rc = scm_call(TZ_SVC_CRYPTO, TZ_SVC_RSA_ID,
        (void *)&scm_cmd_buf, SCM_BUFFER_SIZE(scm_cmd_buf), NULL, 0);
    if (rc) {
        pr_info("%s(): scm_call failed to export public RSA key: rc = %d\n", __func__, rc);
        goto out;
    }
    
    tz_inv_range(rsa_export_key_resp, sizeof(crypto_storage_rsa_export_key_resp_t));
    tz_inv_range(rsa_export_key_req->public_exponent, rsa_export_key_resp->public_exponent_size);
    tz_inv_range(rsa_export_key_req->modulus, rsa_export_key_resp->modulus_size);

    pr_info("%s: rc = %d status = %d, mod_size = %d, pub_exp_size = %d, exported_key_len = %d\n",
        __func__, rc, rsa_export_key_resp->status,rsa_export_key_resp->modulus_size,
        rsa_export_key_resp->public_exponent_size, rsa_export_key_resp->exported_key_len);

    if (rsa_export_key_resp->status != 0) {
        pr_info("%s(): scm_call did not return correct status for export pubKey status = %d\n",
            __func__, rsa_export_key_resp->status);
        rc = -1;
        goto out;
    }
    
    if (rsa_export_key_req->cmd_id != rsa_export_key_resp->cmd_id) {
        pr_info("%s(): req cmd_id: %d, resp cmd_id: %d. ERROR.\n",
            __func__, rsa_export_key_req->cmd_id, rsa_export_key_resp->cmd_id);
        rc = -1;
        goto out;
    }

    if (!rc) {
        put_user(rsa_export_key_resp->modulus_size, &user_data->modulus_data_size);
        rc = copy_to_user(user_data_k.modulus_data_ptr, modulus_data_ptr,
                          rsa_export_key_resp->modulus_size);

        put_user(rsa_export_key_resp->public_exponent_size, &user_data->public_exponent_size);
        rc = copy_to_user(user_data_k.public_exponent_ptr, public_exponent_ptr,
                          rsa_export_key_resp->public_exponent_size);
    }
    
out:
    if (rsa_export_key_req)  kfree(rsa_export_key_req);
    if (rsa_export_key_resp) kfree(rsa_export_key_resp);
    if (key_material)        kfree(key_material);
    if (modulus_data_ptr)    kfree(modulus_data_ptr);
    if (public_exponent_ptr) kfree(public_exponent_ptr);
        
    return rc;
}


static ssize_t ql_tzone_read(struct file *file, char __user *pbuf, size_t size, loff_t *ppos)
{
    return 0;
}


static int ql_tzone_open(struct inode *inode, struct file *file)
{
    return 0;
}


static int ql_tzone_release(struct inode *inode, struct file *file)
{
    return 0;
}


static int long ql_tzone_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int rc;

    switch (cmd) {
        case QL_TZ_CRYPTO_SVC_RANDOM_DATA_GEN:
            rc = ql_tz_crypto_random_gen((ql_tz_random_data_t *)arg);
            break;
        
        case QL_TZ_CRYPTO_SVC_AES_KEY_GEN:
            rc = ql_tz_crypto_aes_gen_key((ql_tz_aes_generate_key_t *)arg);
            break;

        case QL_TZ_CRYPTO_SVC_AES_KEY_IMPORT:
            rc = ql_tz_crypto_aes_import_key((ql_tz_aes_import_key_t *)arg);
            break;

        case QL_TZ_CRYPTO_SVC_AES_DATA_ENCRYPT:
            rc = ql_tz_crypto_aes_endec_data( TZ_STOR_SVC_SEAL_DATA,
                (ql_tz_aes_endecrypt_data_t *)arg);
            break;

        case QL_TZ_CRYPTO_SVC_AES_DATA_DECRYPT:
            rc = ql_tz_crypto_aes_endec_data( TZ_STOR_SVC_UNSEAL_DATA,
                (ql_tz_aes_endecrypt_data_t *)arg);
            break;

        case QL_TZ_CRYPTO_SVC_RSA_KEYPAIR_GEN:
            rc = ql_tz_crypto_rsa_gen_keypair((ql_tz_rsa_generate_key_t *)arg);
            break;
        
        case QL_TZ_CRYPTO_SVC_RSA_DATA_SIGN:
            rc = ql_tz_crypto_rsa_sign_data((ql_tz_rsa_sign_verify_data_t *)arg);
            break;
        
        case QL_TZ_CRYPTO_SVC_RSA_SINDATA_VERIFY:
            rc = ql_tz_crypto_rsa_verify_data((ql_tz_rsa_sign_verify_data_t *)arg);
            break;

        case QL_TZ_CRYPTO_SVC_RSA_KEYPAIR_IMPORT:
            rc = ql_tz_crypto_rsa_import_keypair((ql_tz_rsa_keypair_import_t *)arg);
            break;

        case QL_TZ_CRYPTO_SVC_RSA_PKCS8_IMPORT:
            rc = ql_tz_crypto_rsa_import_pkcs8((ql_tz_rsa_pkcs8_import_t *)arg);
            break;

        case QL_TZ_CRYPTO_SVC_RSA_PUBKEY_EXPORT:
            rc = ql_tz_crypto_rsa_export_pubkey((ql_tz_rsa_public_key_export_t *)arg);
            break;
  
#ifdef TZ_FUSE_SUPPORT
        case TRUST_ZONE_FUSE_READ:
            rc = tz_qfprom_read((tz_qfprom_read_row_t *)arg);
            break;
#endif
        //case TRUST_ZONE_SVC_VERSION:
        //    rc = scm_svc_version((uint32_t *)arg);
        //    break;

        default:
            rc = -EINVAL;
        break;
    }

    return rc;
}


static const struct file_operations tzone_fops = {
    .read = ql_tzone_read,
    .open = ql_tzone_open,
    .release = ql_tzone_release,
    .unlocked_ioctl = ql_tzone_unlocked_ioctl,
};

static struct miscdevice tzone_dev = {
    MISC_DYNAMIC_MINOR,
    "tzone",
    &tzone_fops
};

static int __init tzone_init(void)
{
    misc_register(&tzone_dev);
    return 0;
}

static void __exit tzone_exit(void)
{
    misc_deregister(&tzone_dev);
}


module_init(tzone_init);
module_exit(tzone_exit);
MODULE_LICENSE("GPL v2");
