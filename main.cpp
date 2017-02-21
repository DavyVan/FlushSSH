#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include "FlushSSHConfig.h"
#include <libssh2.h>

#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <sys/types.h>

#define ARGC 2

void display_help()
{
    puts("Usage: FlushSSH hosts_file cmd_file");
    puts("       Ctrl+C will terminate elegantly");
    puts("hosts_file:");
    puts("    IPaddr username passwd");
    puts("cmd_file:");
    puts("    One command per line.");
}

int main(int argc, char *argv[])
{
    // Claims
    const char *hosts_file_path = NULL;
    const char *cmd_file_path = NULL;

    FILE *hosts_file = NULL;
    FILE *cmd_file = NULL;

    char *hostname = NULL;
    char *username = NULL;
    char *passwd = NULL;
    unsigned long hostaddr;

    int sock;
    sockaddr_in saddr_in;
    LIBSSH2_SESSION *session = NULL;
    LIBSSH2_CHANNEL *channel = NULL;

    int t;

    // Parse arguements
    // For now, no prefix, only compare the number of arguements
    if (argc != ARGC + 1)   // Wrong
    {
        display_help();
        return -1;
    }
    else    // Valid
    {
        hosts_file_path = argv[1];
        cmd_file_path = argv[2];
    }

    // Read hosts_file
    hosts_file = fopen(hosts_file_path, "r");
    if (NULL == hosts_file)
    {
        printf("Open file <%s> failed. Errno: %d\n", hosts_file_path, errno);
        return errno;
    }

    // Initiate Windows Socket API (If need)
#ifdef WIN32
    WSADATA wsadata;
    int err = WSAStartup(MAKEWORD(2, 0), &wsadata);
    if(0 != err)
    {
        printf("Windows Socket API initiating failed with errno: %d\n", err);
        return err;
    }
#endif

    // Initiate libssh2 library
    t = libssh2_init(0);
    if (0 != t)
    {
        printf("libssh2 initiating failed with errno: %d\n", t);
        return t;
    }

    printf("Started up. NOTICE: All hosts will be trusted.\n", );

    while(EOF != fscanf("%s %s %s", hostname, username, passwd))
    {
        printf("Connecting to %s@%s...\n", username, hostname);
        hostaddr = inet_addr(hostname);

        // Create a socket connection
        sock = socket(AF_INET, SOCK_STREAM, 0);
        saddr_in.sin_family = AF_INET;
        saddr_in.sin_port = htons(22);
        if (connect(sock, (sockaddr*)(&sin), sizeof(sockaddr_in)) != 0)
        {
            printf("Connection failed with errno: %d\n", errno);
            return errno;
        }

        // Create a session instance
        session = libssh2_session_init();
        if (NULL == session)
        {
            printf("SSH session initiating failed.\n");
            return -1;
        }

        // blocking session

        // Session start up
        while ((t = libssh2_session_handshake(session, sock)) == LIBSSH2_ERROR_EAGAIN);     // This while loop is for non-blocking session
        if (0 != t)
        {
            printf("SSH handshake failed with errno: %d\n", t);
            return t;
        }
    }
}
