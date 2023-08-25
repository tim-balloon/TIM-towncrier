/**
 * @file publisher.c
 */
#include <stdlib.h>
#include <string.h>

#include <hiredis/hiredis.h>

#include "publisher.h"


Publisher *tim_pub_create(redisContext *ctx, const char *chn) {
    Publisher *pub = malloc(sizeof *pub);
    if (pub != NULL) {
        pub->ctx = ctx;
        pub->chn = strdup(chn);
    }
    return pub;
}


void tim_pub_free(Publisher *pub) {
    free(pub->chn);
    free(pub);
}


redisReply *tim_pub_send(Publisher *pub, const char *msg) {
    return redisCommand(pub->ctx, "PUBLISH %s %s", pub->chn, msg);
}


