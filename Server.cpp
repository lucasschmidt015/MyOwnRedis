#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>


static void msg(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

static void do_something(int connfd) {
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        msg("read() error");
        return;
    }
    printf("client says: %s\n", rbuf);

    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}

int main() {
    //Creating TCP Server

    //AF_INET is the address family for IPv4
    //SOCK_STREAM is the type of socket, in this case, a TCP socket
    //The third is useless in our case
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if(fd < 0) {
        die("socket()");
    }


    int val = 1;
    // The second and the third argument specifies which options to set
    // The 4th argument is the option value 
    // The option value is arbitrary bytes, so its length must be specified.
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    //Now, lets bind the address 0.0.0.0:1234.

    // struct sockaddr_in holds an IPv4 address and port. You must initialize the structure as shown in the sample code. 
    // The ntohs() and ntohl() functions convert numbers to the required big endian format.
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(0);
    int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));
    if (rv) {
        die("bind()");
    }

     // listen
     // After the listen() syscall, the OS will automatically handle TCP handshakes and place established connections in a queue. 
     // The application can then retrieve them via accept(). The backlog argument is the size of the queue, which in our case is 
     // SOMAXCONN. SOMAXCONN is defined as 128 on Linux, which is sufficient for us.
    rv = listen(fd, SOMAXCONN);
    if (rv) {
        die("listen()");
    }

    // The accept() syscall also returns the peer’s address. The addrlen argument is both the input size and the output size.
    while (true) {
        // accept
        struct sockaddr_in client_addr = {};
        socklen_t socklen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
        if (connfd < 0) {
            continue;   // error
        }

        do_something(connfd);
        close(connfd);
    }

    return 0;
}

