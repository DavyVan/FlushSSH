#include <cstdio>
#include "FlushSSHConfig.h"
#include "clean.h"

void clean_up_session(LIBSSH2_SESSION *session)
{
    libssh2_session_disconnect(session, "Shuting down, thank you.");
    libssh2_session_free(session);
    session = NULL;
}

void clean_up_sock_libssh2(int sock)
{
#ifdef WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    libssh2_exit();
    printf("Terminated.\n");
}

void clean_up_channel(LIBSSH2_CHANNEL *channel)
{
    int exitcode;
    int t;
    char *exitsignal = NULL;

    t = libssh2_channel_close(channel);
    if (0 == t)
    {
        exitcode = libssh2_channel_get_exit_status(channel);
        libssh2_channel_get_exit_signal(channel, &exitsignal, NULL, NULL, NULL, NULL, NULL);
    }
    if (exitsignal)
        printf("Got signal: %s\n", exitsignal);
    else
        printf("Abort: %d\n", exitcode);
    libssh2_channel_free(channel);
    channel = NULL;
}
