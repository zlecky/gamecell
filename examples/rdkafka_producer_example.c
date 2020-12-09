#include <stdio.h>
#include <signal.h>
#include <memory.h>

#include <librdkafka/rdkafka.h>

static volatile sig_atomic_t run = 1;

/*
 * @brief signal terminal of program
 */
static void stop(int sig) {
    run = 0;
    fclose(stdin);
}

static void dr_msg_cb(rd_kafka_t* rk, const rd_kafka_message_t* msg, void* opque) {
    if (msg->err) {
        fprintf(stderr, "%% Message delivery failed: %s\n", rd_kafka_err2str(msg->err));
    } else {
        fprintf(stdout, "%% Message delivered %zd bytes, partition %d", msg->len, msg->partition);
    }

    /* The msg is destroyed automatically by librdkafka */
}

int main(int argc, char* argv[]) {
    rd_kafka_t* rk        = NULL;
    rd_kafka_conf_t* conf = NULL;

    char errstr[512];
    bzero(errstr, sizeof(errstr));
    char buf[512];
    bzero(buf, sizeof(buf));

    const char* brokers = NULL;
    const char* topic   = NULL;

    if (argc < 3) {
        fprintf(stderr, "%% Usage: %s <brokers> <topic>\n", argv[0]);
        return 1;
    }

    brokers = argv[1];
    topic   = argv[2];

    // Create kafka client configuration.
    conf = rd_kafka_conf_new();

    // Set bootstrap.server brokers.
    if (rd_kafka_conf_set(conf, "bootstrap.servers", brokers, errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%% %s\n", errstr);
        return 1;
    }

    // Set the delivery report callback.
    rd_kafka_conf_set_dr_msg_cb(conf, dr_msg_cb);

    // Set producer instance.
    rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (!rk) {
        fprintf(stderr, "%% Failed to create a new producer: %s\n", errstr);
        return 1;
    }

    // Signal handle for clean shutdown
    signal(SIGINT, stop);

    fprintf(stderr,
            "%% Type some text and hit enter to produce message\n"
            "%% Or just hit enter to only serve delivery reports\n"
            "%% Press Ctrl-C or Ctrl-D to exit\n");

    while (run && fgets(buf, sizeof(buf), stdin)) {
        rd_kafka_resp_err_t err;

        size_t len = sizeof(buf);
        if (buf[len - 1] == '\n') {
            buf[--len] = '\0';
        }

        if (0 == len) {
            rd_kafka_poll(rk, 0);
            continue;
        }

    retry:

        err = rd_kafka_producev(rk,
                                RD_KAFKA_V_TOPIC(topic),
                                RD_KAFKA_V_MSGFLAGS(RD_KAFKA_MSG_F_COPY),
                                RD_KAFKA_V_VALUE(buf, len),
                                RD_KAFKA_V_OPAQUE(NULL),
                                RD_KAFKA_V_END);
        // Failed to *enqueue* message for producing.
        if (err) {
            fprintf(stderr, "%% Failed to produce to topic %s: %s\n", topic, rd_kafka_err2str(err));

            if (RD_KAFKA_RESP_ERR__QUEUE_FULL == err) {
                rd_kafka_poll(rk, 1000);
                goto retry;
            }
        } else {
            fprintf(stdout, "%% Enqueued msssage (%zd bytes) for topic %s\n", len, topic);
        }

        rd_kafka_poll(rk, 0);
    }

    // Wait for final messages to be delivered or fail.
    fprintf(stdout, "%% Flushing final message ...\n");
    rd_kafka_flush(rk, 10 * 1000);

    // If the output queue is still not empty there is an issue with producing messages to the clusters.
    if (rd_kafka_outq_len(rk) > 0) {
        fprintf(stderr, "%% %d message(s) were not delivered\n", rd_kafka_outq_len(rk));
    }

    /* Destroy the producer instance */
    rd_kafka_destroy(rk);

    return 0;
}
