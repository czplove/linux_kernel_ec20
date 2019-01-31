/**
 * @file quectel_string.h
 * @brief Quectel string public function declarations.
 *
 * @note
 *
 * @copyright Copyright (c) 2010-2017 @ Quectel Wireless Solutions Co., Ltd.
 */

#ifndef __QUECTEL_STRING_H__
#define __QUECTEL_STRING_H__

/**
 * Checks string for a digit (0 through 9).
 *
 * @param[in] str    Character string
 *
 * @return On success, Zero is returned. On error, -1 is returned.
 * 
 */
extern int quectel_string_isdigit(const char *str);

/**
 * checks for an alphanumeric character (A through Z).
 *
 * @param[in] letter    Character.
 *
 * @return On success, Zero is returned. On error, -1 is returned.
 * 
 */
extern int quectel_isalpha_upper(char letter);

/**
 * checks string for an alphanumeric character (A through Z).
 *
 * @param[in] str    Character string
 *
 * @return On success, Zero is returned. On error, -1 is returned.
 * 
 */
extern int quectel_string_isalpha_upper(const char *str);

/**
 * converts the IPv4 numbers-and-dots natation into from the internet host address
 *
 * @param[in] ip    The internet host address
 *
 * @return the IPv4 numbers-and-dots natation into
 * 
 */
extern char *quectel_ip_int_to_string(int ip);

/**
 * converts the IPv6 numbers-and-dots natation into from the internet host address
 *
 * @param[in] addr6    The internet IPv6 host address
 *
 * @return the IPv6 numbers-and-dots natation into
 * 
 */
extern char *quectel_ipv6_ntop(struct in6_addr addr6);

/**
 * converts the IPv4 numbers-and-dots natation into from the internet host address
 *
 * @param[in] addr4    The internet IPv4 host address
 *
 * @return the IPv4 numbers-and-dots natation into
 * 
 */
extern char *quectel_ipv4_ntoa(unsigned int addr4);

/**
 * Verify the IPv4 address format
 *
 * @param[in] addr4    The numbers-and-dots IPv4 host address
 *
 * @return On success, Zero is returned. On error, -1 is returned.
 * 
 */
extern int quectel_ipv4_check(char *addr4);

/**
 * convert a string to an integer
 *
 * @param[in] nptr     string pointer
 * @param[out] val     converts the initial portion of the string pointed to val
 *
 * @return On success, Zero is returned. On error, -1 is returned.
 * 
 */
extern int quectel_atoi(const char *nptr, int *val);

/**
 * The function returns a pointer to a new string which is a duplicate of the string.
 * Memory for the new string is obtained with malloc(3), and can be freed with free(3).
 *
 * @param[in] fmt     the string format
 *
 * @return On success, the function returns a pointer to the duplicated string.
 *         It returns NULL if insufficient memory was available
 * 
 */
extern char *quectel_strdup(const char *fmt, ...);

/**
 * The function remove the string before and after the double quotation marks
 *
 * @param[out] dest     the string remove before and after the double quotation marks
 * @param[in] src       source string
 *
 * @return On success, Zero is returned. On error, -1 is returned.
 * 
 */
extern int quectel_double_quotes_to_string(char *dest, const char *src);

#endif /* end of __QUECTEL_STRING_H__ */
