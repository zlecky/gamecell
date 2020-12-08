#include <string>
#include <iostream>

#include <librdkafka/rdkafkacpp.h>

static volatile sig_atomic_t run = 1;

static void sigterm(int sig) {
   run = 0;
}

class ExampleDeliveryReportCB : public RdKafka::DeliveryReportCb {
public:
    void dr_cb(RdKafka::Message& msg) {
        if (msg.err()) {
            std::cerr << "% message delivery failed" << msg.errstr() << std::endl;
        } else {
            std::cout << "% Message delivered to topic " << msg.topic_name()
            << " [" << msg.partition() << "] at offset " << msg.offset() << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <brokers> <topic>" << std::endl;
        return -1;
    }

    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    if (nullptr == conf) {
        std::cerr << "Create conf failed" << std::endl;
        return -1;
    }

    const std::string brokers = argv[1];
    const std::string topic   = argv[2];

    std::string err_str;
    if (conf->set("bootstrap.servers", brokers, err_str) != RdKafka::Conf::CONF_OK) {
        std::cerr << "Set brokers failed: %" << err_str << std::endl;
        return -1;
    }

    ExampleDeliveryReportCB cb;
    if (conf->set("dr_cb", &cb, err_str) != RdKafka::Conf::CONF_OK) {
        std::cerr << "Set callback failed: %" << err_str << std::endl;
        return -1;
    }

    RdKafka::Producer* producer = RdKafka::Producer::create(conf, err_str);
    if (nullptr == producer) {
        std::cerr << "Create producer failed: %" << err_str << std::endl;
        return -1;
    }

    delete conf;
    signal(SIGINT, sigterm);
    signal(SIGTERM, sigterm);

    std::cout << "Type message value and hit enter to produce message." << std::endl;

    for (std::string line; run && std::getline(std::cin, line);) {
        if (line.empty()) {
            producer->poll(0);
            continue;
        }

        /*
         * Send/Produce message.
         * This is an asynchronous call, on success it will only
         * enqueue the message on the internal producer queue.
         * The actual delivery attempts to the broker are handled
         * by background threads.
         * The previously registered delivery report callback
         * is used to signal back to the application when the message
         * has been delivered (or failed permanently after retries).
         */
        retry:
        RdKafka::ErrorCode err = producer->produce(
                        /* Topic name */
                        topic,
                        /* Any Partition: the builtin partitioner will be
                         * used to assign the message to a topic based
                         * on the message key, or random partition if
                         * the key is not set. */
                        RdKafka::Topic::PARTITION_UA,
                        /* Make a copy of the value */
                        RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
                        /* Value */
                        const_cast<char *>(line.c_str()), line.size(),
                        /* Key */
                        nullptr, 0,
                        /* Timestamp (defaults to current time) */
                        0,
                        /* Message headers, if any */
                        nullptr,
                        /* Per-message opaque value passed to
                         * delivery report */
                        nullptr);

        if (err != RdKafka::ERR_NO_ERROR) {
            std::cerr << "% Failed to produce to topic " << topic << ": " << RdKafka::err2str(err) << std::endl;

            if (err == RdKafka::ERR__QUEUE_FULL) {
                /* If the internal queue is full, wait for
                 * messages to be delivered and then retry.
                 * The internal queue represents both
                 * messages to be sent and messages that have
                 * been sent or failed, awaiting their
                 * delivery report callback to be called.
                 *
                 * The internal queue is limited by the
                 * configuration property
                 * queue.buffering.max.messages */
                producer->poll(1000/*block for max 1000ms*/);
                goto retry;
            }

        } else {
            std::cerr << "% Enqueued message (" << line.size() << " bytes) " << "for topic " << topic << std::endl;
        }

        /* A producer application should continually serve
         * the delivery report queue by calling poll()
         * at frequent intervals.
         * Either put the poll call in your main loop, or in a
         * dedicated thread, or call it after every produce() call.
         * Just make sure that poll() is still called
         * during periods where you are not producing any messages
         * to make sure previously produced messages have their
         * delivery report callback served (and any other callbacks
         * you register). */
        producer->poll(0);
    }

    /* Wait for final messages to be delivered or fail.
   * flush() is an abstraction over poll() which
   * waits for all messages to be delivered. */
    std::cerr << "% Flushing final messages..." << std::endl;
    producer->flush(10*1000 /* wait for max 10 seconds */);

    if (producer->outq_len() > 0)
        std::cerr << "% " << producer->outq_len() << " message(s) were not delivered" << std::endl;

    delete producer;

    return 0;
}

