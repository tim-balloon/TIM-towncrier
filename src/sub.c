#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <unistd.h>

#include "subscriber.h"
#include "utils.h"


static bool keep_running = true;


static void handler(int sig) {
    keep_running = false;
}


// Display a brief command line help.
void print_help() {
    fprintf(stderr, "Usage: pub [-h] [-H host] [-p port] chn");
}


int main(int argc, char **argv) {
    char *host = "127.0.0.1";
    int port = 6379;
    int status = EXIT_SUCCESS;

    // Parse command line arguments.
    int c;
    while ((c = getopt(argc, argv, "H:hp:")) != -1) {
        switch (c) {
            case 'H':
                host = optarg;
                break;
            case 'h':
                print_help();
                return EXIT_SUCCESS;
            case 'p':
                port = atoi(optarg);
                break;
            case '?':
                if (optopt == 'H') {
                    tim_error("[ERROR]: Option '-%c' needs an argument, "
                              "the name (or IP) of the server with the broker",
                              optopt);
                } else if (optopt == 'p') {
                    tim_error("[ERROR]: Option '-%c' needs an argument,"
                              "port number to use to connect to the broker",
                              optopt);
                } else {
                    tim_error("[ERROR] Unknown option '-%c'. "
                              "Run 'sub -h' for help\n", optopt);
                }
                return EXIT_FAILURE;
            default:
                print_help();
                return EXIT_FAILURE;
        }
    }
    char *chn = NULL;
    if (optind < argc) {
        chn = argv[optind];
    } else {
        tim_error("Channel not specified");
        return EXIT_FAILURE;
    }

    errno = 0;

    // Register signal handlers.
    signal(SIGINT, handler);
    signal(SIGTERM, handler);

    tim_info("Connecting to the message broker");
    redisContext *ctx = tim_broker_connect(host, port);
    if (ctx == NULL) {
        tim_error("Can't connect to the message broker:");
        return EXIT_FAILURE;
    }

    tim_info("Creating a subscriber");
    Subscriber *sub = tim_sub_create(ctx, chn);
    if (sub == NULL) {
        tim_error("Can't create the subscriber:");
        return EXIT_FAILURE;
    }

    tim_info("Starting to monitor channel '%s'", sub->chn);
    redisReply *reply;
    while (keep_running
           && redisGetReply(sub->ctx, (void **) &reply) == REDIS_OK) {
        errno = 0;
        if (reply != NULL) {
            switch (reply->type) {
                case REDIS_REPLY_ARRAY:
                    tim_info("Received message '%s' from channel '%s'",
                             reply->element[2]->str, reply->element[1]->str);
                    break;
                default:
                    tim_warn("Handling replies of type %d not implemented",
                             reply->type);
                    break;
            }
        } else {
            tim_warn("Reading message failed:");
            ctx = tim_broker_connect(host, port);
            if (ctx == NULL) {
                status = EXIT_FAILURE;
                goto cleanup;
            }
        }
        freeReplyObject(reply);
    }
    tim_info("Execution interrupted, tearing the subscriber down");

    // Relinquish manually allocated resources (i.e. memory) if any.
cleanup:
    tim_sub_unsubscribe(sub, chn);
    tim_sub_free(sub);

    return EXIT_SUCCESS;
}