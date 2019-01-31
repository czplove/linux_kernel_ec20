/**
 * @file quectel_file.c
 * @brief Quectel file public function declarations.
 *
 * @note
 *
 * @copyright Copyright (c) 2010-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef __QUECTEL_FILE_H__
#define __QUECTEL_FILE_H__

/**
 * writes the buffer pointed buf to the file
 * 
 * @param[in] file           file absolute path
 * @param[in] buf            the buffer pointed
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int quectel_file_write_string(const char *file, const char *buf);

/**
 * writes WiFi Mac address to wlan_mac.bin file
 * 
 * @param[in] mac            The WiFi Mac address, 6 bytes
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int quectel_wlan_mac_bin_write(char *mac);

/**
 * read WiFi Mac address from wlan_mac.bin file
 * 
 * @param[out] mac            The WiFi Mac address, 6 bytes
 *
 * @return 
 *   On success, 0 is returned.  On error, -1 is returned.
 * 
 */
extern int quectel_wlan_mac_bin_read(char *mac);

#endif /* end of __QUECTEL_FILE_H__ */
