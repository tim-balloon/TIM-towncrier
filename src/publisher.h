/**
 * @file publisher.h
 */
#ifndef TOWNCRIER_PUBLISHER_H
#define TOWNCRIER_PUBLISHER_H


#include "hiredis/hiredis.h"


/**
 * @brief Data structure representing a publisher.
 *
 * A publisher sends messages to a provided channel where they can be received
 * by subscribers.
 */
struct pub {
    redisContext *ctx;  ///< Context to use to connect to the database.
    char *chn;          ///< Channel to use to publish messages.
};
typedef struct pub Publisher;


/**
 * @brief Set up a publisher.
 *
 * @param[in] ctx Context the publisher should use to communicate with the db.
 * @param[in] chn Name of the channel to use to publish the messages to.
 * @return the pointer to the created publisher, NULL if any error occurred
 */
Publisher *tim_pub_create(redisContext *ctx, const char *chn);


/**
 * @brief Tear down a publisher.
 *
 * Deletes a publisher relinquishing any memory that was allocated during its
 * creation.
 *
 * @param[in] pub Publisher to tear down.
 */
void tim_pub_free(Publisher *pub);


/**
 * @brief Publish a message.
 *
 * @param pub Publisher to use
 * @param msg Message to send to channel owned by the publisher
 * @return Reply from the message broker.
 */
redisReply *tim_pub_send(Publisher *pub, const char *msg);


/**
 * @brief Describe publisher details.
 *
 * @param pub Publisher to describe.
 * @return Description of the publisher.
 */
char *tim_pub_describe(Publisher *pub);


#endif //TOWNCRIER_PUBLISHER_H
