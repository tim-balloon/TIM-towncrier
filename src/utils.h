/**
 * @file utils.h
 */
#ifndef TOWNCRIER_UTILS_H
#define TOWNCRIER_UTILS_H


#include <stdarg.h>

#include <hiredis/hiredis.h>


/**
 * @brief Connect to the message broker.
 *
 * @param host Hostname or IP address of the server the message broker runs on.
 * @param port Port to use to connect to the message broker.
 * @return Context to use to communicate with the broker.
 *         NULL if any errors occurred.
 */
redisContext *tim_broker_connect(const char *host, int port);


/**
 * @brief Print a message with a given severity level.
 *
 * Errors and warnings are printed to stderr, ordinary messages are printed
 * to stdout.
 *
 * If the format specifier end with `:` (colon), the `tim_error()` and
 * `tim_warn()` will call the standard C function `strerror()` to include
 * any additional system error information that may be available.  It has no
 * effect if `tim_info()` is used.
 *
 * All functions automatically append the new line character to the provided
 * format specifier.
 *
 * @param fmt format specifier to use to print the message
 */
void tim_error(const char* fmt, ...);
void tim_warn(const char* fmt, ...);
void tim_info(const char* fmt, ...);


#endif //TOWNCRIER_UTILS_H
