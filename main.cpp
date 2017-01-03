#include <cstdio>
#include <libssh/libssh.h>
#include "FlushSSHConfig.h"

int verify_knownhost(ssh_session session)
{
    int state = ssh_is_server_known(session);
    switch (state)
    {
        case SSH_SERVER_KNOWN_OK:
            return 0;

        case SSH_SERVER_ERROR:
            printf("Error: %s\n", ssh_get_error(session));
            return -1;

        case SSH_SERVER_KNOWN_CHANGED:
            printf("Host key for server was changed, updated.\n");
            break;

        case SSH_SERVER_FOUND_OTHER:
            printf("The host key for this server was not found but an other type of key exists, updated.\n");
            break;

        case SSH_SERVER_FILE_NOT_FOUND:
            printf("Could not find known host file. Created.\n");
            break;

        case SSH_SERVER_NOT_KNOWN:
            printf("This is a new server. Trusted.\n");
            break;
    }
    if(ssh_write_knownhost(session) < 0)
    {
        printf("Error: write known host failed.\n");
        return -1;
    }
    return 0;
}

int main(int argc, char const *argv[]) {
    printf("adsfsfsdfsfd\n");
    printf("Hello world!! Version:%d.%d\n", FlushSSH_VERSION_MAJOR, FlushSSH_VERSION_MINOR);

    // libssh test
    // new session and connection
    ssh_session session = ssh_new();
    if(session == NULL)
    {
        printf("ssh_session creation failed!\n");
        exit(-1);
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, "192.168.0.100");
    ssh_options_set(session, SSH_OPTIONS_USER, "fanquan");

    int connection = ssh_connect(session);
    if(connection != SSH_OK)
    {
        printf("Connection failed!\n");
        printf("Error MSG: %s\n", ssh_get_error(session));
        exit(-1);
    }

    // Authentication
    if(verify_knownhost(session) < 0)
    {
        ssh_disconnect(session);
        ssh_free(session);
        exit(-1);
    }
    char *password = "fanquan";
    connection = ssh_userauth_password(session, NULL, password);
    if(connection != SSH_AUTH_SUCCESS)
    {
        printf("Authenticate failed.\nError: %s", ssh_get_error(session));
        ssh_disconnect(session);
        ssh_free(session);
        exit(-1);
    }

    ssh_disconnect(session);
    ssh_free(session);

    return 0;
}
