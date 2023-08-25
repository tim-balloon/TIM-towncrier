/**
 * @file subscriber.h
 */
#ifndef TOWNCRIER_SUBSCRIBER_H
#define TOWNCRIER_SUBSCRIBER_H


#include <hiredis/hiredis.h>


/**
 * @brief Data structure representing a subscriber.
 *
 * A subscriber monitors the channel it is subscribed to for new messages.
 */
typedef struct sub {
    redisContext *ctx;  ///< Context to use to connect to the database.
    char *chn;          ///< Channel the subscriber is subscribe to.
} Subscriber;


/**
 * @brief Set up a subscriber.
 *
 * @param[in] ctx Context the publisher should use to communicate with the db.
 * @param[in] chn Name of the channel to subscribe to.
 * @return Pointer to the created subscriber, NULL if any error occurred.
 */
Subscriber *tim_sub_create(redisContext *ctx, const char *chn);


/**
 * @brief Tear down a subscriber.
 *
 * Deletes a subscriber relinquishing any memory that was allocated during its
 * creation.
 *
 * @param[in] sub Subscriber to tear down.
 */
void tim_sub_free(Subscriber *sub);


/**
 * @brief Subscribe to a channel.
 *
 * @param[in] sub Subscriber to initialize.
 * @param[in] chn Name of the channel to subscribe to.
 * @return Reply from the message broker.
 */
redisReply *tim_sub_subscribe(Subscriber *sub, const char *chn);


/**
 * @brief Unsubscribe from a channel.
 *
 * @param[in] sub Subscriber to initialize.
 * @param[in] chn Channel to unsubscribe from.
 * @return Reply from the message broker.
 */
redisReply *tim_sub_unsubscribe(Subscriber *sub, const char *chn);


/**
 * @brief Describe a subscriber.
 *
 * @param[in] sub Subscriber to describe.
 * @return Description of the subscriber.
 */
char *tim_sub_describe(Subscriber *sub);

#endif //TOWNCRIER_SUBSRIBER_H
