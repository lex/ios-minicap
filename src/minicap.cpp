#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <signal.h>
#include <zconf.h>

#include <iostream>

#include "SimpleServer.hpp"
#include "FrameListener.hpp"
#include "Banner.hpp"
#include "StreamClient.h"

// MSG_NOSIGNAL does not exists on OS X
#if defined(__APPLE__) || defined(__MACH__)
# ifndef MSG_NOSIGNAL
#   define MSG_NOSIGNAL SO_NOSIGPIPE
# endif
#endif

static FrameListener gWaiter;

void print_usage(char **argv) {
    char *name = NULL;
    name = strrchr(argv[0], '/');

    printf("Usage: %s [OPTIONS]\n", (name ? name + 1: argv[0]));
    printf("Stream video from a device.\n");
    printf("  -u, --udid UDID\t\ttarget specific device by its 40-digit device UDID\n");
    printf("  -p, --port PORT\t\tport to run server on\n");
    printf("  -r, --resolution RESOLUTION\tdesired resolution <w>x<h>\n");
    printf("  -h, --help\t\t\tprints usage information\n");
    printf("\n");
}

bool parse_args(int argc, char **argv, const char **udid, int *port, const char **resolution) {
    if ( argc < 7 ) {
        // Currently the easiest way to make all arguments required
        print_usage(argv);
        return false;
    }
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-u") || !strcmp(argv[i], "--udid")) {
            i++;
            if (!argv[i]) {
                print_usage(argv);
                return false;
            }
            *udid = argv[i];
            continue;
        }
        else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port")) {
            i++;
            if (!argv[i]) {
                print_usage(argv);
                return false;
            }
            *port = atoi(argv[i]);
            continue;
        }
        else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--resolution")) {
            i++;
            if (!argv[i]) {
                print_usage(argv);
                return false;
            }
            *resolution = argv[i];
            continue;
        }
        else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            print_usage(argv);
            return false;

        }
        else {
            print_usage(argv);
            return false;
        }
    }
    return true;
}

static void signal_handler(int signum) {
    switch (signum) {
        case SIGINT:
            printf("Received SIGINT, stopping\n");
            gWaiter.stop();
            break;
        case SIGTERM:
            printf("Received SIGTERM, stopping\n");
            gWaiter.stop();
            break;
        default:
            abort();
    }
}

static void setup_signal_handler() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    // we want to just ignore the SIGPIPE and get a EPIPE when socket is closed
    signal(SIGPIPE, SIG_IGN);
}

void parseResolution(const char* resolution, uint32_t* width, uint32_t* height) {
    std::string _resolution(resolution);
    size_t sep = _resolution.find("x");
    *width = std::stoul(_resolution.substr(0, sep).c_str());
    *height = std::stoul(_resolution.substr(sep+1, _resolution.length()).c_str());
}


int main(int argc, char **argv) {
    const char *udid = NULL;
    const char *resolution = NULL;
    int port = 0;

    setup_signal_handler();
    if (!parse_args(argc, argv, &udid, &port, &resolution)) {
        return EXIT_FAILURE;
    }

    uint32_t width = 0, height = 0;
    parseResolution(resolution, &width, &height);

    StreamClient client;

    if (!client.setupDevice(udid)) {
        return EXIT_FAILURE;
    }

    client.setResolution(width, height);
    client.setFrameListener(&gWaiter);
    client.start();

    if (!gWaiter.waitForFrame()) {
        return EXIT_SUCCESS;
    }

    client.stop();

    Frame frame;

    client.lockFrame(&frame);

    std::cout << "resolution: " << frame.width << "x" << frame.height << std::endl;

    DeviceInfo realInfo, desiredInfo;
    realInfo.orientation = 0;
    realInfo.height = frame.height;
    realInfo.width = frame.width;
    desiredInfo.orientation = 0;
    desiredInfo.height = frame.height;
    desiredInfo.width = frame.width;

    Banner banner(realInfo, desiredInfo);
    client.releaseFrame(&frame);

    SimpleServer server;
    server.start(port);

    int socket;
    bool b = false;
    unsigned int c = 0;

    while (gWaiter.isRunning() and (socket = server.accept()) > 0) {
        std::cout << "New client connection" << std::endl;

        std::cout << "Starting client..." << std::endl;
        client.start();
        std::cout << "Started client." << std::endl;

        while (gWaiter.isRunning() and gWaiter.waitForFrame() > 0) {
            if (!b) {
                std::cout << "Started streaming for real." << std::endl;
                b = true;
            }

            // let's skip every other frame, who needs 60 fps anyways?

            if (c % 2 == 1) {
                c = 0;
                continue;
            }

            client.lockFrame(&frame);
            ssize_t w = send(socket, frame.data, frame.height * frame.bytesPerRow, 0);
            client.releaseFrame(&frame);

            if (w < 0) {
                break;
            }
        }

        client.stop();
    }

    return EXIT_SUCCESS;
}
