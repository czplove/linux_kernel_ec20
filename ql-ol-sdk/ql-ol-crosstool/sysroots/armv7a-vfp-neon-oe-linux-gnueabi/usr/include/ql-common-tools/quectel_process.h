/*
 * @file: quectel_process.h
 *
 */

#ifndef __QUECTEL_PROCESS_H__
#define __QUECTEL_PROCESS_H__

#include <sys/types.h>
#include <stdbool.h>

/**
 * Check if the process name exists
 *
 * @param[in] process_name       the process name
 *
 * @return On success, zero is returned. On error, -1 is returned.
 * 
 */
extern int quectel_check_process_by_name(const char *process_name);

/**
 * Check if the process name exists
 *
 * @param[in] pid                the process id
 * @param[in] process_name       the process name
 *
 * @return On success, zero is returned. On error, -1 is returned.
 * 
 */
extern int quectel_check_process_by_pid(pid_t pid, char *process_name);

/**
 * Check if the process name exists
 *
 * @param[in] pid_file           the process id file
 * @param[in] process_name       the process name
 *
 * @return On success, zero is returned. On error, -1 is returned.
 * 
 */
extern int quectel_check_process_by_pid_file(char *pid_file, char *process_name);

/**
 * Process pid write to the specified file
 *
 * @param[in] file       the process pid file
 *
 * @return On success, zero is returned. On error, -1 is returned.
 * 
 */
extern int quectel_write_pid_file(const char *file);

/**
 * Setting process can only allow one
 *
 * @param[in] lock_file       the lock file
 *
 * @return On success, zero is returned. On error, -1 is returned.
 * 
 */
extern int quectel_set_process_single_instance(const char *lock_file);

/**
 * Release the quectel_set_process_single_instance() function malloc resources
 *
 * @param[in] fd          the quectel_set_process_single_instance() return value
 *
 * @return None
 * 
 */
extern void quectel_release_process_single_instance(int fd);

/**
 * Check if the process has set a single instance
 *
 * @param[in] lock_file       the lock file
 *
 * @return On success, zero is returned. On error, -1 is returned.
 * 
 */
extern int quectel_check_process_singal_instance(const char *lock_file);

/**
 * Uses popen(3) to create a child process that executes the shell comand
 *
 * @param[in] fmt       the format of shell command
 *
 * @return On success, zero is returned. On error, -1 is returned.
 * 
 */
extern int quectel_do_system_call(const char *fmt, ...);

#endif /* end of __QUECTEL_PROCESS_H__ */
