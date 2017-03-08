#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <memory.h>
#include <signal.h>
#include "FlushSSHConfig.h"
#include "clean.h"
#include "utils.h"
#include <libssh2.h>

#include <sys/types.h>

#define ARGC 2
#define READ_FILE_BUFSIZE 200
#define READ_CHANNEL_BUFSIZE 32000



void CTRL_C_handler(int sig)
{
    // Ctrl + C: do clean-up and quit
    printf("User abort, do nothing and quit. Goodbye.\n");
}

int main(int argc, char *argv[])
{
    // Register signal handler
    signal(SIGINT, CTRL_C_handler);

    // Claims
    const char *hosts_file_path = NULL;
    const char *cmd_file_path = NULL;

    FILE *hosts_file = NULL;
    FILE *cmd_file = NULL;

    char hostname[100];
    char username[100];
    char kpflag;
    char passwd[100];
    char pub_key_file[100];
    char prv_key_file[100];
    unsigned long hostaddr;

    char cmd_file_buf[READ_FILE_BUFSIZE];
    int line_count = 0;


    FILE *echo_file = NULL;
    char echo_read_buf[READ_CHANNEL_BUFSIZE];

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

    printf("Started up. NOTICE: Known-host check will be skiped.\n");

    // Start iterate each host
    while(EOF != fscanf(hosts_file, "%s %s %c", hostname, username, kpflag))    // TODO: edit to here last time
    {
        int sock;
        sockaddr_in saddr_in;
        LIBSSH2_SESSION *session = NULL;
        LIBSSH2_CHANNEL *channel = NULL;

        printf("============================================\n");
        printf("Connecting to %s@%s...\n", username, hostname);
        hostaddr = inet_addr(hostname);

        // Create a socket connection
        sock = socket(AF_INET, SOCK_STREAM, 0);
        saddr_in.sin_family = AF_INET;
        saddr_in.sin_port = htons(22);
        saddr_in.sin_addr.s_addr = hostaddr;
        if (connect(sock, (sockaddr*)(&saddr_in), sizeof(sockaddr_in)) != 0)
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

        // Authenticate via passwd
        while ((t = libssh2_userauth_password(session, username, passwd)) == LIBSSH2_ERROR_EAGAIN);     // This while loop is for non-blocking session
        if (0 != t)
        {
            printf("Authentication failed.\n");
            clean_up_session(session);
            clean_up_sock_libssh2(sock);
            return t;
        }

        // Create channel blocking
        channel = libssh2_channel_open_session(session);
        if (NULL == channel)
        {
            printf("Create channel failed\n");
            clean_up_session(session);
            clean_up_sock_libssh2(sock);
            return -1;
        }

        // Read cmd_file
        // For now, the file for each host is same, but they will be different in the future.
        cmd_file = fopen(cmd_file_path, "r");
        if (NULL == cmd_file)
        {
            printf("Open file <%s> failed. Errno: %d\n", cmd_file_path, errno);
            clean_up_channel(channel);
            clean_up_session(session);
            clean_up_sock_libssh2(sock);
            return errno;
        }

        // Open a file to hold the echo content from this host
        // The file is named after host's IP
        char echo_file_name[100] = {0};
        strcpy(echo_file_name, hostname);
        strcat(echo_file_name, ".echo.txt");
        echo_file = fopen(echo_file_name, "a+");
        if (NULL == echo_file)
        {
            printf("Open file <%s> failed. Errno: %d\n", echo_file_name, errno);
            clean_up_channel(channel);
            clean_up_session(session);
            clean_up_sock_libssh2(sock);
            return errno;
        }
        printf("Echo content will be stored in %s\n", echo_file_name);

        // Execute each command
        while (NULL != fgets(cmd_file_buf, READ_FILE_BUFSIZE, cmd_file))
        {
            line_count++;
            t = libssh2_channel_exec(channel, cmd_file_buf);
            if (t != 0)
            {
                printf("Exec failed on line: %d.\n", line_count);
                clean_up_channel(channel);
                clean_up_session(session);
                clean_up_sock_libssh2(sock);
                return t;
            }

            // Read echo
            do
            {
                t = libssh2_channel_read(channel, echo_read_buf, READ_CHANNEL_BUFSIZE);
                if (t > 0)
                {
                    printf("We got %d bytes echo.\n", t);
                    fwrite(echo_read_buf, t, 1, echo_file);
                }
                memset(echo_read_buf, 0, READ_CHANNEL_BUFSIZE);
            } while (t > 0);
        }
        line_count = 0;
        fclose(cmd_file);
        fclose(echo_file);

        // This host done.
        clean_up_channel(channel);
        clean_up_session(session);
        clean_up_sock_libssh2(sock);
        printf("Host: %s done.\n", hostname);
    }
    fclose(hosts_file);
    printf("All done.\n");
    return 0;
}
