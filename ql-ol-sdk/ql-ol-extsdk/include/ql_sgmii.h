/**
 * @file ql_sgmii.h
 * @brief Quectel Openlinux SGMII public function declarations.
 * 
 * @note 
 *
 * @copyright Copyright (c) 2009-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef __QL_SGMII_H__
#define __QL_SGMII_H__

typedef enum {
	QL_SGMII_AUTONEG_OFF = 0,
	QL_SGMII_AUTONEG_ON
} ql_sgmii_autoneg_e;

typedef enum {
	QL_SGMII_SPEED_10MHZ = 0,
	QL_SGMII_SPEED_100MHZ,
	QL_SGMII_SPEED_1000MHZ /* this speed can not support */
} ql_sgmii_speed_e;

typedef enum {
	QL_SGMII_DUPLEX_FULL = 0,
	QL_SGMII_DUPLEX_HALF
} ql_sgmii_duplex_e;

struct ql_sgmii_info {
	ql_sgmii_autoneg_e autoneg;
	ql_sgmii_speed_e speed;
	ql_sgmii_duplex_e duplex;
	unsigned long tx_bytes;
	unsigned long rx_bytes;
	unsigned long tx_pkts;
	unsigned long rx_pkts;
};

/**
 * Enables the SGMII ethernet module.
 * 
 * @param None
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_sgmii_enable(void);

/**
 * Disable the SGMII ethernet module.
 * 
 * @param None
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int ql_sgmii_disable(void);

/**
 * Set the SGMII ethernet configuration: the ethernet auto negotiation configuration.
 * 
 * @param [in]         auto negotiation
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 * @dependencies
 *   the ql_sgmii_enable must be set enable
 *
 */
extern int ql_sgmii_autoneg_set(ql_sgmii_autoneg_e autoneg);

/**
 * Get the SGMII ethernet configuration: the ethernet auto negotiation configuration.
 * 
 * @param [out]         auto negotiation
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 * @dependencies
 *   the ql_sgmii_enable must be set enable
 *
 */
extern int ql_sgmii_autoneg_get(ql_sgmii_autoneg_e *autoneg);

/**
 * Set the SGMII ethernet configuration: the ethernet speed configuration.
 * 
 * @param [in]         ethernet speed
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 * @dependencies
 *   the ql_sgmii_enable must be set enable
 *
 */
extern int ql_sgmii_speed_set(ql_sgmii_speed_e speed);

/**
 * Get the SGMII ethernet configuration: the ethernet speed configuration.
 * 
 * @param [out]         the ethernet current speed
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 * @dependencies
 *   the ql_sgmii_enable must be set enable
 *
 */
extern int ql_sgmii_speed_get(ql_sgmii_speed_e *speed);

/**
 * Set the SGMII ethernet configuration: the ethernet duplex configuration.
 * 
 * @param [in]         ethernet duplex
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 * @dependencies
 *   the ql_sgmii_enable must be set enable
 *
 */
extern int ql_sgmii_duplex_set(ql_sgmii_duplex_e duplex);

/**
 * Get the SGMII ethernet configuration: the ethernet duplex configuration.
 * 
 * @param [out]         the ethernet current duplex
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 *
 * @dependencies
 *   the ql_sgmii_enable must be set enable
 * 
 */
extern int ql_sgmii_duplex_get(ql_sgmii_duplex_e *duplex);

/**
 * Get the sgmii ethernet information: transmit and receive bytes, transmit and receive packets, speed, duplex.
 * 
 * @param [out]         the ethernet information
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 *
 * @dependencies
 *   the ql_sgmii_enable must be set enable
 * 
 */
extern int ql_smgii_info_get(struct ql_sgmii_info *info);


#endif /* end of __QL_SGMII_H__ */
