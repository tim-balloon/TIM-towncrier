/**
 * @file subscriber.c
 */
#include <stdlib.h>
#include <string.h>
#include "subscriber.h"


Subscriber *tim_sub_create(redisContext *ctx, const char *chn) {
    Subscriber *sub = malloc(sizeof *sub);
    if (sub != NULL) {
        sub->ctx = ctx;
        sub->chn = strdup(chn);
        redisReply *reply = redisCommand(ctx, "SUBSCRIBE %s", chn);
        freeReplyObject(reply);
    }
    return sub;
}


void tim_sub_free(Subscriber *sub) {
    free(sub->chn);
    free(sub);
}


redisReply *tim_sub_subscribe(Subscriber *sub,  const char *chn) {
    sub->chn = strdup(chn);
    return redisCommand(sub->ctx, "SUBSCRIBE %s", *chn);
}


redisReply *tim_sub_unsubscribe(Subscriber *sub, const char *chn) {
    redisReply *reply = NULL;
    if (strcmp(sub->chn, chn) == 0) {
        reply = redisCommand(sub->ctx, "UNSUBSCRIBE %s", sub->chn);
        free(sub->chn);
        sub->chn = NULL;
    }
    return reply;
}

char *tim_sub_describe(Subscriber *sub) {
    return sub->chn;
}