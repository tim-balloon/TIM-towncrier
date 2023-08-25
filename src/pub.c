#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <unistd.h>

#include <hiredis/hiredis.h>

#include "publisher.h"
#include "utils.h"


#define SLEEP_INTERVAL 1


static bool keep_running = true;


static void handler(int sig) {
    keep_running = false;
}


// Display a brief command line help.
static void print_help() {
    fprintf(stderr, "Usage: pub [-h] [-H host] [-p port] chn\n");
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
                    tim_error("Option '-%c' requires an argument, "
                              "a host name (or an ip)", optopt);
                } else if (optopt == 'p') {
                    tim_error("Option '-%c' requires an argument, "
                              "a port number", optopt);
                } else {
                    tim_error("Unknown option '-%c'. "
                              "Run 'pub -h' for help",
                              optopt);
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
        tim_error("Channel not specified. Run 'pub -h' for help");
        return EXIT_FAILURE;
    }

    // Register signal handlers.
    signal(SIGINT, handler);
    signal(SIGTERM, handler);

    errno = 0;

    tim_info("Connecting to the message broker");
    redisContext *ctx = tim_broker_connect(host, port);
    if (ctx == NULL) {
        tim_error("Can't connect to the message broker:");
        return EXIT_FAILURE;
    }

    tim_info("Creating a publisher");
    Publisher *pub = tim_pub_create(ctx, chn);
    if (pub == NULL) {
        tim_error("Can't create the publisher:");
        status = EXIT_FAILURE;
        goto cleanup;
    }

    tim_info("Starting to send messages");
    char msg[] = "000000";
    for (int id = 0; keep_running; ++id) {
        errno = 0;
        snprintf(msg, strlen(msg) + 1, "%06d", id);
        redisReply *reply = tim_pub_send(pub, msg);
        if (reply != NULL) {
            switch (reply->type) {
                case REDIS_REPLY_INTEGER:
                    tim_info("Message '%s' sent, no. of subscribers: %lld",
                             msg, reply->integer);
                    break;
                default:
                    tim_info("reply type: %d\n", reply->type);
                    break;
            }
            freeReplyObject(reply);
        } else {
            tim_warn("Sending message failed: %s", ctx->errstr);
            ctx = tim_broker_connect(host, port);
            if (ctx == NULL) {
                tim_error("Attempt to reconnect to the broker failed:");
                status = EXIT_FAILURE;
                goto cleanup;
            }
        }
        sleep(SLEEP_INTERVAL);
    }
    tim_info("Execution interrupted, tearing the publisher down");

    // Relinquish manually allocated resources (i.e. memory) if any.
cleanup:
    tim_pub_free(pub);

    return status;
}
