/**
 * @file utils.c
 */
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

#include "utils.h"


redisContext *tim_broker_connect(const char *ip, int port) {
    redisContext *ctx = redisConnect(ip, port);
    if (ctx == NULL || ctx->err) {
        if (ctx != NULL) {
            redisFree(ctx);
            ctx = NULL;
        }
    }
    return ctx;
}


void tim_error(const char *fmt, ...) {
    fflush(stderr);
    fprintf(stderr, "[ERROR] ");

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    if (fmt[0] != '\0' && fmt[strlen(fmt)-1] == ':') {
        fprintf(stderr, " %s", strerror(errno));
    }
    fprintf(stderr, "\n");
}


void tim_warn(const char *fmt, ...) {
    fflush(stderr);
    fprintf(stderr, "[WARNING] ");

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    if (fmt[0] != '\0' && fmt[strlen(fmt)-1] == ':') {
        fprintf(stderr, " %s", strerror(errno));
    }
    fprintf(stderr, "\n");
}


void tim_info(const char *fmt, ...) {
    fflush(stdout);
    fprintf(stdout, "[INFO] ");

    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);

    fprintf(stdout, "\n");
}
