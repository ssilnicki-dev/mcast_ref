#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int sockfd;
    struct sockaddr_in addr;
    struct group_req mreq;
    const char *multicast_addr = "239.0.0.1";
    const int port = 12345;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    // Enable reuse address
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt SO_REUSEADDR");
        close(sockfd);
        return 1;
    }

    // Bind to any address and specified port
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    // Set up multicast group using MCAST_JOIN_GROUP
    memset(&mreq, 0, sizeof(mreq));
#ifdef __FreeBSD__
    mreq.gr_interface = 1; // FIXME: requires particular mcast capable interface - adjust to your setup
#endif
#ifdef __linux__
    mreq.gr_interface = 0; // use default behavior
#endif
    struct sockaddr_in *group_addr = (struct sockaddr_in *)&mreq.gr_group;
    group_addr->sin_family = AF_INET;
    group_addr->sin_addr.s_addr = inet_addr(multicast_addr);
    group_addr->sin_port = 0; // Port not needed for joining group
#ifdef __FreeBSD__
    group_addr->sin_len = sizeof(struct sockaddr_in);
#endif
    if (setsockopt(sockfd, IPPROTO_IP, MCAST_JOIN_GROUP, &mreq, sizeof(mreq)) < 0) {
        printf("setsockopt MCAST_JOIN_GROUP %d\n", errno);
        close(sockfd);
        return 1;
    }

    printf("Successfully joined multicast group %s on port %d\n", multicast_addr, port);
    sleep(30);

    close(sockfd);
    return 0;
}
